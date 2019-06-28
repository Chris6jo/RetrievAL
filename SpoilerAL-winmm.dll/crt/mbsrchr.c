#include <windows.h>
#include <mbstring.h>

#pragma warning(disable:4414)

#ifndef _M_IX86
unsigned char * __cdecl _mbsrchr(const unsigned char *string, unsigned int c)
{
	const unsigned char *p;
	size_t              n;

	if (!c)
		return _mbschr(string, c);
	p = NULL;
	if (!(c & ~0xFFFF))
		for (n = c <= 0xFF ? 1 : 2; string = _mbschr(string, c); string += n)
			p = string;
	return (unsigned char *)p;
}
#else
__declspec(naked) unsigned char * __cdecl _mbsrchr(const unsigned char *string, unsigned int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		push    ebx
		push    esi
		mov     ebx, dword ptr [c + 8]
		mov     esi, dword ptr [string + 8]
		test    ebx, ebx
		jz      L1
		push    edi
		xor     edi, edi
		test    ebx, not 0FFFFH
		jnz     L4
		push    ebx
		push    esi
		mov     esi, 2
		cmp     ebx, 100H
		sbb     esi, 0
		jmp     L3

		align   16
	L1:
		pop     esi
		pop     ebx
		jmp     _mbschr

		align   16
	L2:
		mov     edi, eax
		add     eax, esi
		mov     dword ptr [esp    ], eax
		mov     dword ptr [esp + 4], ebx
	L3:
		call    _mbschr
		test    eax, eax
		jnz     L2
		add     esp, 8

		align   16
	L4:
		mov     eax, edi
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}
#endif
