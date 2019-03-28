#include <windows.h>
#include <limits.h>

#ifndef _M_IX86
unsigned char * __cdecl _mbschr(const unsigned char *string, unsigned int c)
{
	if (c <= USHRT_MAX)
		if (c <= UCHAR_MAX) {
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c)) {
				unsigned char c2;

				do
					if ((c2 = *(string++)) == (unsigned char)c)
						return (unsigned char *)string - 1;
				while (c2 && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2) || *(string++)));
			}
		} else {
			unsigned int c2;

			if ((c2 = (unsigned short)c << 8) && IsDBCSLeadByteEx(CP_THREAD_ACP, c >>= 8)) {
				c |= c2;
				while (c2 = *(string++))
					if (IsDBCSLeadByteEx(CP_THREAD_ACP, c2))
						if (!(((unsigned char *)&c2)[1] = *(string++)))
							break;
						else if (c2 == c)
							return (unsigned char *)string - 2;
			}
		}
	return NULL;
}
#else
__declspec(naked) unsigned char * __cdecl _mbschr(const unsigned char *string, unsigned int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		push    ebx
		push    esi
		mov     ebx, dword ptr [c + 8]
		mov     esi, dword ptr [string + 8]
		test    ebx, 0FFFF0000H
		jnz     L1
		test    bh, bh
		jnz     L5
		push    ebx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
	L1:
		xor     eax, eax
		jmp     L3

		align   16
	L2:
		mov     al, byte ptr [esi]
		inc     esi
		cmp     al, bl
		je      L4
		test    al, al
		jz      L3
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
		mov     al, byte ptr [esi]
		inc     esi
		and     eax, 0FFH
		jnz     L2
	L3:
		pop     esi
		pop     ebx
		ret

		align   16
	L4:
		lea     eax, [esi - 1]
		pop     esi
		pop     ebx
		ret

		align   16
	L5:
		push    edi
		mov     eax, ebx
		shl     ax, 8
		jz      L8
		shr     ebx, 8
		mov     edi, eax
		push    ebx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L8
		or      edi, ebx

		align   16
	L6:
		mov     bl, byte ptr [esi]
		inc     esi
		and     ebx, 0FFH
		jz      L7
		push    ebx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L6
		mov     bh, byte ptr [esi]
		inc     esi
		test    bh, bh
		jz      L7
		cmp     ebx, edi
		jne     L6
		pop     edi
		lea     eax, [esi - 2]
		pop     esi
		pop     ebx
		ret

		align   16
	L7:
		xor     eax, eax
	L8:
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}
#endif
