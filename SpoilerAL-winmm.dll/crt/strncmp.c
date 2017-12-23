#include <stddef.h>

__declspec(naked) int __cdecl strncmp(const char *string1, const char *string2, size_t count)
{
	#define PAGE_SIZE 4096

	__asm
	{
		push    ebx
		push    esi
		mov     ecx, dword ptr [esp + 12]
		mov     edx, dword ptr [esp + 16]
		mov     ebx, dword ptr [esp + 20]
		sub     ecx, edx
		test    ebx, -1 ; alignment for L1
		jz      L3
		test    edx, 3
		jz      L2
	L1:
		mov     al, byte ptr [edx + ecx]
		cmp     al, byte ptr [edx]
		jne     L4
		test    al, al
		jz      L3
		inc     edx
		sub     ebx, 1
		jbe     L3
		test    dl, 3
		jnz     L1
	L2:
		lea     eax, [edx + ecx]
		and     ax, PAGE_SIZE - 1
		cmp     ax, PAGE_SIZE - 4
		ja      L1
		mov     eax, dword ptr [edx + ecx]
		cmp     eax, dword ptr [edx]
		jne     L1
		sub     ebx, 4
		jbe     L3
		lea     esi, [eax - 1010101H]
		add     edx, 4
		not     eax
		and     eax, esi
		test    eax, 80808080H
		jz      L2
	L3:
		xor     eax, eax
		pop     esi
		pop     ebx
		ret

		align   16
	L4:
		sbb     eax, eax
		pop     esi
		or      eax, 1
		pop     ebx
		ret
	}

	#undef PAGE_SIZE
}
