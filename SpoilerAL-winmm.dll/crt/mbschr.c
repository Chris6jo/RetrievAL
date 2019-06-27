#include <windows.h>

#ifndef _M_IX86
unsigned char * __cdecl _mbschr(const unsigned char *string, unsigned int c)
{
	unsigned char c2;

	if (!(c & ~0xFF)) {
		if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c)) {
			string--;
			do
				if ((c2 = *(++string)) == (unsigned char)c)
					goto DONE;
			while (c2 && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2) || *(++string)));
		}
	} else if (!(c & ~0xFFFF) && (c & 0xFF) && IsDBCSLeadByteEx(CP_THREAD_ACP, c >> 8)) {
		for (string--; ; ) {
			if ((c2 = *(++string)) != (unsigned char)(c >> 8))
				if (!c2)
					break;
				else if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2))
					continue;
				else if (!string[1])
					break;
			else if ((c2 = string[1]) == (unsigned char)c)
				goto DONE;
			else if (!c2)
				break;
			string++;
		}
	}
	string = NULL;
DONE:
	return (unsigned char *)string;
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
		test    ebx, not 0FFH
		jnz     L2
		push    ebx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		mov     ecx, eax
		xor     eax, eax
		test    ecx, ecx
		jnz     L6
		dec     esi

		align   16
	L1:
		mov     al, byte ptr [esi + 1]
		inc     esi
		cmp     al, bl
		je      L5
		test    al, al
		jz      L6
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		test    cl, cl
		jz      L6
		inc     esi
		jmp     L1

		align   16
	L2:
		mov     ecx, ebx
		xor     eax, eax
		test    ecx, not 0FFFFH
		jnz     L6
		test    ecx, 0FFH
		jz      L6
		shr     ecx, 8
		dec     esi
		push    ecx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L6
		xor     eax, eax

		align   16
	L3:
		mov     al, byte ptr [esi + 1]
		inc     esi
		cmp     al, bh
		je      L4
		test    al, al
		jz      L6
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L3
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		test    cl, cl
		jz      L6
		inc     esi
		jmp     L3

		align   16
	L4:
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		cmp     cl, bl
		je      L5
		test    cl, cl
		jz      L6
		inc     esi
		jmp     L3

		align   16
	L5:
		mov     eax, esi
	L6:
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}
#endif
