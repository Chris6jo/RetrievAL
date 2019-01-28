#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"
#include "SSGSubjectProperty.h"

extern DWORD RepeatDepth;

void __stdcall repeat_ReadSSRFile(
	TSSGCtrl     *this,
	LPVOID       ParentStack,
	LPVOID       ADJElem,
	const string *LineS,
	DWORD        RepeatIndex,
	DWORD        ParentRepeat,
	TSSGSubject  *SSGS);

#if 0
__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_repeat_ReadSSRFile()
{
	static TSSGSubject SSGS = { TSSGSubject_VTable, 0 };
	__asm
	{
		#define ReturnAddress   004EB508H
		#define this            (ebp +   8H)
		#define ParentStack     (ebp +  10H)
		#define ADJElem         (ebp +  14H)
		#define RepeatIndex     (ebp +  18H)
		#define ParentRepeat    (ebp +  1CH)
		#define LineS           (ebp -  38H)

		mov     edx, dword ptr [ParentRepeat]
		mov     ecx, dword ptr [RepeatIndex]
		push    offset SSGS
		lea     eax, [LineS]
		push    edx
		push    ecx
		push    eax
		mov     edx, dword ptr [ADJElem]
		mov     ecx, dword ptr [ParentStack]
		mov     eax, dword ptr [this]
		push    edx
		push    ecx
		push    eax
		push    ReturnAddress
		jmp     repeat_ReadSSRFile

		#undef ReturnAddress
		#undef this
		#undef ParentStack
		#undef ADJElem
		#undef RepeatIndex
		#undef ParentRepeat
		#undef LineS
	}
}
#endif

void __stdcall repeat_ReadSSRFile(
	TSSGCtrl     *this,
	LPVOID       ParentStack,
	LPVOID       ADJElem,
	const string *LineS,
	DWORD        RepeatIndex,
	DWORD        ParentRepeat,
	TSSGSubject  *SSGS)
{
	vector_string tmpV;
	vector_dword  indices;

	vector_ctor(&indices);
	TSSGCtrl_ReadSSRFile(&tmpV, this, LineS, &indices, SSGS);
	if (!vector_empty(&tmpV))
	{
		if (!vector_empty(&indices))
		{
			DWORD               parent;
			TSSGSubjectProperty *prop;
			string              *it;
			LPDWORD             repeat;
			size_t              elementSize;

			if (!RepeatDepth)
				parent = MAXDWORD;
			else if (SSGS && SSGS->type == stDIR)
				parent = SSGS->propertyIndex;
			else if (prop = GrowSubjectProperty(&parent))
			{
				prop->RepeatDepth  = RepeatDepth;
				prop->RepeatIndex  = RepeatIndex;
				prop->ParentRepeat = ParentRepeat;
			}
			RepeatDepth++;
			it = vector_begin(&tmpV);
			repeat = vector_begin(&indices);
			elementSize = vector_byte_size(&tmpV) / vector_size(&indices);
			do
			{
				vector_string constElem;

				vector_begin(&constElem) = it;
				(LPBYTE)it += elementSize;
				vector_end_of_storage(&constElem) = vector_end(&constElem) = (string *)it;
				TSSGCtrl_EnumReadSSG(this, &constElem, ParentStack, ADJElem, *(repeat++), parent);
			} while (it != vector_end(&tmpV));
			RepeatDepth--;
		}
		else
		{
			TSSGCtrl_EnumReadSSG(this, &tmpV, ParentStack, ADJElem, RepeatIndex, ParentRepeat);
		}
	}
	vector_dtor(&indices);
	vector_string_dtor(&tmpV);
}

