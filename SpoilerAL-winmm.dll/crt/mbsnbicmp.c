#include <windows.h>

#ifndef _M_IX86
int __cdecl _mbsnbicmp(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	int ret;

	ret = CompareStringA(GetThreadLocale(), NORM_IGNORECASE, string1, count, string2, count);
	return ret ? ret - CSTR_EQUAL : _NLSCMPERROR;
}
#else
__declspec(naked) int __cdecl _mbsnbicmp(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		mov     ecx, dword ptr [count]
		mov     edx, dword ptr [string2]
		mov     eax, dword ptr [string1]
		push    ecx
		push    edx
		push    ecx
		push    eax
		call    GetThreadLocale
		push    NORM_IGNORECASE
		push    eax
		call    CompareStringA
		xor     ecx, ecx
		sub     eax, 1
		adc     ecx, -1
		and     eax, 7FFFFFFFH
		add     eax, ecx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}
#endif
