#include <windows.h>

#ifndef _M_IX86
unsigned char * __cdecl _mbsupr(unsigned char *string)
{
	LCMapStringA(GetThreadLocale(), LCMAP_UPPERCASE, string, INT_MAX, string, INT_MAX);
	return string;
}
#else
__declspec(naked) unsigned char * __cdecl _mbsupr(unsigned char *string)
{
	__asm
	{
		#define string (esp + 4)

		call    GetThreadLocale
		mov     ecx, dword ptr [string]
		push    07FFFFFFFH
		push    ecx
		push    07FFFFFFFH
		push    ecx
		push    LCMAP_UPPERCASE
		push    eax
		call    LCMapStringA
		mov     eax, dword ptr [string]
		ret

		#undef string
	}
}
#endif
