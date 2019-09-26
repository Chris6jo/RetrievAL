#include <stddef.h>
#include <ctype.h>
#include "PageSize.h"

#ifndef _M_IX86
int __cdecl _memicmp(const void *buffer1, const void *buffer2, size_t count)
{
	int ret;

	(char *)buffer1 += count;
	(char *)buffer2 += count;
	count ^= -1;
	ret = 0;
	while (++count)
		if (ret = tolower(((char *)buffer1)[count]) - tolower(((char *)buffer2)[count]))
			break;
	return ret;
}
#else
__declspec(naked) int __cdecl _memicmp(const void *buffer1, const void *buffer2, size_t count)
{
	__asm
	{
		#define buffer1 (esp + 4)
		#define buffer2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     esi, dword ptr [buffer1 + 16]           // esi = buffer1
		mov     edi, dword ptr [buffer2 + 16]           // edi = buffer2
		mov     ebp, dword ptr [count + 16]             // ebp = count
		xor     eax, eax
		add     esi, ebp                                // esi = end of buffer1
		add     edi, ebp                                // edi = end of buffer2
		xor     ebp, -1                                 // ebp = -count - 1
		xor     edx, edx
		jmp     byte_loop_increment

		align   16
	byte_loop:
		mov     al, byte ptr [esi + ebp]
		mov     dl, byte ptr [edi + ebp]
		sub     eax, edx
		jnz     byte_compare_insensitive
	byte_loop_increment:
		inc     ebp
		jz      return_equal
		lea     ebx, [edi + ebp]
		lea     ecx, [esi + ebp]
		and     ebx, 3
		jnz     byte_loop
		shl     ecx, 32 - BSF_PAGE_SIZE
		jmp     dword_loop

		align   16
	byte_compare_insensitive:
		cmp     eax, 'a' - 'A'
		je      byte_compare_above
		cmp     eax, 'A' - 'a'
		jne     return_not_equal
		xor     eax, eax
		lea     ebx, [edx - 'a']
		cmp     ebx, 'z' - 'a'
		jbe     byte_loop_increment
		sub     edx, 'A'
		mov     eax, ebx
		jmp     secondary_to_lower

		align   16
	byte_compare_above:
		xor     eax, eax
		lea     ebx, [edx - 'A']
		cmp     ebx, 'Z' - 'A'
		jbe     byte_loop_increment
		mov     edx, ebx
		lea     eax, [ebx + 'a' - 'A']
		jmp     primary_to_lower

		align   16
	dword_loop:
		cmp     ecx, (PAGE_SIZE - 4) shl (32 - BSF_PAGE_SIZE)
		ja      byte_loop                               // cross pages
		mov     ecx, dword ptr [esi + ebp]
		mov     ebx, dword ptr [edi + ebp]
		sub     ecx, ebx
		jnz     dword_compare_insensitive
	dword_loop_increment:
		add     ebp, 4
		jc      return_equal
		lea     ecx, [esi + ebp]
		shl     ecx, 32 - BSF_PAGE_SIZE
		jmp     dword_loop

		align   16
	dword_compare_insensitive:
		test    cl, cl
		jz      dword_compare_byte_1
		cmp     cl, 'A' - 'a'
		je      dword_compare_byte_0_borrow
		cmp     cl, 'a' - 'A'
		jne     dword_unmatch_byte_0
		sub     ebx, 'A'
		cmp     bl, 'Z' - 'A'
		ja      dword_unmatch_byte_even_above
		add     ebx, 'A'
		jmp     dword_compare_byte_1

	dword_compare_byte_0_borrow:
		sub     ebx, 'a'
		cmp     bl, 'z' - 'a'
		ja      dword_unmatch_byte_even_borrow
		add     ecx, 0100H
		add     ebx, 'a'

	dword_compare_byte_1:
		test    ch, ch
		jnz     dword_compare_byte_1_above
		shr     ecx, 16
		shr     ebx, 16
		jmp     dword_compare_byte_2

	dword_compare_byte_1_above:
		cmp     ch, 'A' - 'a'
		je      dword_compare_byte_1_borrow
		cmp     ch, 'a' - 'A'
		jne     dword_unmatch_byte_1
		sub     ebx, 'A' shl 8
		cmp     bh, 'Z' - 'A'
		ja      dword_unmatch_byte_odd_above
		shr     ecx, 16
		add     ebx, 'A' shl 8
		shr     ebx, 16
		jmp     dword_compare_byte_2

	dword_compare_byte_1_borrow:
		sub     ebx, 'a' shl 8
		cmp     bh, 'z' - 'a'
		ja      dword_unmatch_byte_odd_borrow
		shr     ecx, 16
		add     ebx, 'a' shl 8
		shr     ebx, 16
		inc     ecx

	dword_compare_byte_2:
		test    cl, cl
		jz      dword_compare_byte_3
		cmp     cl, 'A' - 'a'
		je      dword_compare_byte_2_borrow
		cmp     cl, 'a' - 'A'
		jne     dword_unmatch_byte_2
		sub     ebx, 'A'
		cmp     bl, 'Z' - 'A'
		ja      dword_unmatch_byte_even_above
		add     ebx, 'A'
		jmp     dword_compare_byte_3

	dword_compare_byte_2_borrow:
		sub     ebx, 'a'
		cmp     bl, 'z' - 'a'
		ja      dword_unmatch_byte_even_borrow
		add     ecx, 0100H
		add     ebx, 'a'

	dword_compare_byte_3:
		test    ch, ch
		jz      dword_loop_increment
		cmp     ch, 'A' - 'a'
		je      dword_compare_byte_3_borrow
		cmp     ch, 'a' - 'A'
		jne     dword_unmatch_byte_3
		sub     ebx, 'A' shl 8
		cmp     bh, 'Z' - 'A'
		jbe     dword_loop_increment
		jmp     dword_unmatch_byte_odd_above

	dword_compare_byte_3_borrow:
		sub     ebx, 'a' shl 8
		cmp     bh, 'z' - 'a'
		jbe     dword_loop_increment
		jmp     dword_unmatch_byte_odd_borrow

	dword_unmatch_byte_2:
		add     ebp, 2
		jc      return_equal

	dword_unmatch_byte_0:
		mov     edx, ebx
		lea     eax, [ecx + ebx]
		jmp     dword_unmatch

	dword_unmatch_byte_1:
		inc     ebp
		jnz     dword_unmatch_byte_odd
		jmp     return_equal

	dword_unmatch_byte_3:
		add     ebp, 3
		jc      return_equal

	dword_unmatch_byte_odd:
		mov     edx, ebx
		lea     eax, [ecx + ebx]
		shr     eax, 8
		shr     edx, 8
		jmp     dword_unmatch

	dword_unmatch_byte_odd_borrow:
		shr     ebx, 8
	dword_unmatch_byte_even_borrow:
		lea     eax, [ebx + 'A']
		lea     edx, [ebx + 'a']
	dword_unmatch:
		and     eax, 0FFH
		and     edx, 0FFH
		sub     eax, 'A'
		sub     edx, 'A'
		jmp     secondary_to_lower

	dword_unmatch_byte_odd_above:
		shr     ebx, 8
	dword_unmatch_byte_even_above:
		lea     eax, [ebx + 'a']
		lea     edx, [ebx + 'A']
		and     eax, 0FFH
		and     edx, 0FFH
		sub     eax, 'A'
		sub     edx, 'A'
		jmp     primary_to_lower

		align   16
	return_not_equal:
		lea     eax, [eax + edx - 'A']
		sub     edx, 'A'
	secondary_to_lower:
		cmp     edx, 'Z' - 'A'
		ja      primary_to_lower
		add     edx, 'a' - 'A'
	primary_to_lower:
		cmp     eax, 'Z' - 'A'
		ja      difference
		add     eax, 'a' - 'A'
	difference:
		sub     eax, edx
	return_equal:
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef buffer1
		#undef buffer2
		#undef count
	}
}
#endif
