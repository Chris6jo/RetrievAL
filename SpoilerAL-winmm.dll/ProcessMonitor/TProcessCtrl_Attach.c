#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TMainForm.h"

#define ALLOCATE_SUPPORT 1

typedef struct {
	DWORD  Id;
	LPVOID Address;
	size_t Size;
	DWORD  Protect;
} PROCESSMEMORYBLOCK, *PPROCESSMEMORYBLOCK;

extern size_t             nNumberOfProcessMemory;
extern PROCESSMEMORYBLOCK *lpProcessMemory;
extern FILETIME           ftProcessCreationTime;
extern TSSGSubject        dummySSGS;

extern unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);

BOOL   IsProcessAttached = FALSE;
string ProcessAttachCode = { NULL };
vector *ProcessAttachAttribute = NULL;
string ProcessDetachCode = { NULL };
vector *ProcessDetachAttribute = NULL;

//---------------------------------------------------------------------
//���s�t�@�C���������ɁA�Ώۃv���Z�X����������
//
//	�����������true��Ԃ��܂���
//	�EOpenProcess���̑O�ɁA�K���s������
//	�E���s�t�@�C�����́A�啶������������ʂ��܂���B
//---------------------------------------------------------------------
BOOLEAN __cdecl TProcessCtrl_AttachByProcessName(TProcessCtrl *this, string ProcessName)
{
	static __inline void OnProcessAttach();

	BOOLEAN         Result;
	PROCESSENTRY32A PE32;

	if (Result = TProcessCtrl_FindProcess(this, &ProcessName, &PE32) == 0)
	{
		this->entry = PE32;

		// �擾�\�������t�@�C������ێ�
		string_assign(&this->attachedProcessName, &ProcessName);

		// �����Ȃ�q�[�v�u���b�N/���W���[���܂ŏ������Ă���
		TProcessCtrl_LoadHeapList(this);
		TProcessCtrl_LoadModuleList(this);

		OnProcessAttach();
	}
	string_dtor(&ProcessName);
	return Result;
}
//---------------------------------------------------------------------
//���s�t�@�C���������ɁA�Ώۃv���Z�X����������
//
//	�����������true��Ԃ��܂���
//	�E���O�ɁASetProcessNameVec()�Ńx�N�^���w�肵�Ă������ƁB
//	�EOpenProcess���̑O�ɁA�K���s������
//	�E���s�t�@�C�����́A�啶������������ʂ��܂���B
//	�E�Y������v���Z�X���̃v���Z�X����ł������true��Ԃ��܂��B
//---------------------------------------------------------------------
BOOLEAN __cdecl TProcessCtrl_Attach(TProcessCtrl *this)
{
	void __cdecl OnProcessDetach();

	if (IsProcessAttached && ((LPDWORD)&this)[-1] == 0x004A61B0)// TProcessCtrl::Open
	{// reset process infomation when lost target
		vector_string backup = this->processNameVec;
		this->processNameVec = (const vector_string) { NULL };
		TProcessCtrl_Clear(this);
#if ALLOCATE_SUPPORT
		if (lpProcessMemory && nNumberOfProcessMemory)
		{
			if (ftProcessCreationTime.dwLowDateTime || ftProcessCreationTime.dwHighDateTime)
			{
				size_t i = nNumberOfProcessMemory;
				do
					if (lpProcessMemory[--i].Protect)
						lpProcessMemory[i].Address = NULL;
				while (i);
			}
			ftProcessCreationTime.dwLowDateTime = 0;
			ftProcessCreationTime.dwHighDateTime = 0;
		}
#endif
		OnProcessDetach();// do here for prevent circular attach, pid will be 0
		this->processNameVec = backup;
	}

	// �x�N�^�ɐς܂ꂽ�v���Z�X�������X�Ƀ`�F�b�N
	for (string *it = vector_begin(&this->processNameVec); it != vector_end(&this->processNameVec); it++)
	{
		string ProcessName;

		string_ctor_assign(&ProcessName, it);
		if (TProcessCtrl_AttachByProcessName(this, ProcessName))
			// Attach�ɐ����������_�ŁAtrue��Ԃ�
			return TRUE;
	}

	return FALSE;
}
//---------------------------------------------------------------------
static __inline void OnProcessAttach()
{
	if (!IsProcessAttached)
	{
		IsProcessAttached = TRUE;
		if (!string_empty(&ProcessAttachCode))
		{
			dummySSGS.type = -1;
			dummySSGS.attribute = ProcessAttachAttribute;
			Parsing(&MainForm->ssgCtrl, &dummySSGS, &ProcessAttachCode, 0);
			dummySSGS.type = stNONE;
			dummySSGS.attribute = NULL;
		}
	}
}
//---------------------------------------------------------------------
void __cdecl OnProcessDetach()
{
	if (IsProcessAttached)
	{
		IsProcessAttached = FALSE;
		if (!string_empty(&ProcessDetachCode))
		{
			dummySSGS.type = -1;
			dummySSGS.attribute = ProcessDetachAttribute;
			Parsing(&MainForm->ssgCtrl, &dummySSGS, &ProcessDetachCode, 0);
			dummySSGS.type = stNONE;
			dummySSGS.attribute = NULL;
		}
	}
}
