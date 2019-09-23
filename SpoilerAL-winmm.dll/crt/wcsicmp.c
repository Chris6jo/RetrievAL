#include <wchar.h>

#ifndef _M_IX86
int __cdecl _wcsicmp(const wchar_t *string1, const wchar_t *string2)
{
	for (; ; )
	{
		unsigned int i1, i2;

		i1 = (unsigned short)*(string1++);
		i2 = (unsigned short)*(string2++);
		if (!(i1 -= i2))
			if (i2)
				continue;
			else
				break;
		if (i1 == 'A' - 'a')
		{
			if ((i2 - 'a') <= 'z' - 'a')
				continue;
		}
		else if (i1 == 'a' - 'A')
		{
			if ((i2 - 'A') <= 'Z' - 'A')
				continue;
		}
		i1 += (i2 -= 'A');
		if (i1 <= 'Z' - 'A')
			i1 += 'a' - 'A';
		if (i2 <= 'Z' - 'A')
			i2 += 'a' - 'A';
		return i1 - i2;
	}
	return 0;
}
#else
__declspec(naked) int __cdecl _wcsicmp(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    ebx
		xor     eax, eax
		mov     ecx, dword ptr [string1 + 4]
		mov     ebx, dword ptr [string2 + 4]
		sub     ebx, ecx

		align   16
	loop_begin:
		xor     edx, edx
		mov     ax, word ptr [ecx]
		mov     dx, word ptr [ecx + ebx]
		add     ecx, 2
		sub     eax, edx
		jnz     compare_insensitive
		test    edx, edx
		jnz     loop_begin
		pop     ebx
		ret

		align   16
	compare_insensitive:
		cmp     eax, 'a' - 'A'
		je      compare_above
		cmp     eax, 'A' - 'a'
		jne     return_not_equal
		sub     edx, 'a'
		xor     eax, eax
		cmp     edx, 'z' - 'a'
		jbe     loop_begin
		mov     eax, edx
		add     edx, 'a' - 'A'
		jmp     secondary_to_lower

		align   16
	compare_above:
		sub     edx, 'A'
		xor     eax, eax
		cmp     edx, 'Z' - 'A'
		jbe     loop_begin
		lea     eax, [edx + 'a' - 'A']
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
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}
#endif
