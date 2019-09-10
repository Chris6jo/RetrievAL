#include <windows.h>

#ifndef _M_IX86
size_t __cdecl _mbslen(const unsigned char *string)
{
	size_t        length;
	unsigned char c;

	length = (ptrdiff_t)-1;
	do
		length++;
	while ((c = *(string++)) && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c) || *(string++)));
	return length;
}
#else
__declspec(naked) size_t __cdecl _mbslen(const unsigned char *string)
{
	__asm
	{
		#define string (esp + 4)

		push    ebx
		push    esi
		or      esi, -1
		mov     ebx, dword ptr [string + 8]

		align   16
	L1:
		inc     esi
		xor     eax, eax
		mov     al, byte ptr [ebx]
		inc     ebx
		test    eax, eax
		jz      L2
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
		mov     al, byte ptr [ebx]
		inc     ebx
		test    al, al
		jnz     L1
	L2:
		mov     eax, esi
		pop     esi
		pop     ebx
		ret

		#undef string
	}
}
#endif
