#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TMainForm.h"

extern unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);

extern TSSGSubject dummySSGS;

BOOL   IsProcessAttached = 0;
string ProcessAttachCode = { NULL };
string ProcessDetachCode = { NULL };

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

	string *it;

	// �x�N�^�ɐς܂ꂽ�v���Z�X�������X�Ƀ`�F�b�N
	for (it = vector_begin(&this->processNameVec); it != vector_end(&this->processNameVec); it++)
	{
		string ProcessName;

		string_ctor_assign(&ProcessName, it);
		if (TProcessCtrl_AttachByProcessName(this, ProcessName))
			// Attach�ɐ����������_�ŁAtrue��Ԃ�
			return TRUE;
	}

	OnProcessDetach();

	// all attaches failed, then clear cached process information
	TProcessCtrl_Clear(this);

	return FALSE;
}
//---------------------------------------------------------------------
static __inline void OnProcessAttach()
{
	if (IsProcessAttached)
		return;
	IsProcessAttached = TRUE;
	if (string_empty(&ProcessAttachCode))
		return;
	Parsing(&MainForm->ssgCtrl, &dummySSGS, &ProcessAttachCode, 0);
}
//---------------------------------------------------------------------
void __cdecl OnProcessDetach()
{
	if (!IsProcessAttached)
		return;
	IsProcessAttached = FALSE;
	if (string_empty(&ProcessDetachCode))
		return;
	Parsing(&MainForm->ssgCtrl, &dummySSGS, &ProcessDetachCode, 0);
}
