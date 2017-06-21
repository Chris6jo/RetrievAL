#include <windows.h>
#include "TSSGCtrl.h"

#define ssgCtrl_reNO_ERROR 0

__declspec(naked) void __cdecl TSSBitList_Write_CheckFunnel()
{
	__asm
	{
		#define ReturnAddress 004BB70EH
		#define SSGC          (ebp + 0CH)
		#define SSGS          ebx
		#define Val           (ebp - 90H)

		mov     ecx, dword ptr [Val]
		mov     eax, dword ptr [SSGC]
		push    ReturnAddress
		push    ecx
		push    SSGS
		push    eax
		call    dword ptr [TSSGCtrl_CheckFunnel]
		add     esp, 12
		xor     eax, eax
		ret

		#undef ReturnAddress
		#undef SSGC
		#undef SSGS
		#undef Val
	}
}

