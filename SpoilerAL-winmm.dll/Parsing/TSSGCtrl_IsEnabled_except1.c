#include <windows.h>
#include "TSSGCtrl.h"

static void __fastcall TSSGCtrl_IsEnabled_except1(TSSGSubject *SSGS, LPCVOID ReturnAddress)
{
	switch ((size_t)ReturnAddress)
	{
	case 0x004B90B1:// TSSBitList::Read
	case 0x004BD6F2:// TSSBundleCalc::Read
	case 0x004BF3D0:// TSSBundleList::Read
	case 0x004C01BD:// TSSBundleToggle::Read
	case 0x004C1E6D:// TSSCalc::Read
	case 0x004C41D3:// TSSDoubleList::Read
	case 0x004C862B:// TSSDoubleToggle::Read
	case 0x004CE381:// TSSFloatCalc::Read
	case 0x0052A0D1:// TSSList::Read
	case 0x0052AF69:// TSSString::Read
	case 0x0052C47F:// TSSToggle::Read
	case 0x0052DD26:// TSSBundleFloatCalc::Read
		SSGS->evaluateAtRead = TRUE;
		break;
	default:
		SSGS->evaluateAtRead = FALSE;
		break;
	}
}

__declspec(naked) BOOLEAN __cdecl TSSGCtrl_IsEnabled(TSSGCtrl *this, TSSGSubject *SSGS)
{
	static const DWORD X005111E6 = 0x005111E6;

	__asm
	{
		mov     edx, dword ptr [esp    ]
		mov     ecx, dword ptr [esp + 8]
		call    TSSGCtrl_IsEnabled_except1
		push    ebp
		mov     ebp, esp
		sub     esp, 64
		jmp     dword ptr [X005111E6]
		ud2
	}
}

