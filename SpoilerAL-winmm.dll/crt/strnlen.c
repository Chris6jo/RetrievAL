#include <string.h>

#ifndef _M_IX86
size_t __cdecl _tcsnlen(const char *string, size_t maxlen)
{
	size_t length;

	if (length = maxlen)
	{
		while (*(string++) && --length);
		length = maxlen - length;
	}
	return length;
}
#else
static size_t __cdecl strnlenSSE2(const char *string, size_t maxlen);
static size_t __cdecl strnlen386(const char *string, size_t maxlen);
static size_t __cdecl strnlenCPUDispatch(const char *string, size_t maxlen);

static size_t(__cdecl * strnlenDispatch)(const char *string, size_t maxlen) = strnlenCPUDispatch;

__declspec(naked) size_t __cdecl strnlen(const char *string, size_t maxlen)
{
	__asm
	{
		jmp     dword ptr [strnlenDispatch]
	}
}

__declspec(naked) static size_t __cdecl strnlenSSE2(const char *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov         eax, dword ptr [maxlen]
		mov         ecx, dword ptr [string]
		test        eax, eax
		jz          L1
		push        ebx
		mov         ebx, ecx
		and         ecx, 15
		and         ebx, -16
		pxor        xmm0, xmm0
		movdqa      xmm1, xmmword ptr [ebx]
		pcmpeqb     xmm1, xmm0
		pmovmskb    edx, xmm1
		shr         edx, cl
		xor         ecx, -1
		test        edx, edx
		jz          L2
		bsf         ecx, edx
		cmp         eax, ecx
		pop         ebx
		cmova       eax, ecx
	L1:
		ret

		align       16
	L2:
		add         ecx, 17
		add         ebx, 16
		sub         ecx, eax
		jae         L4
		sub         ebx, ecx

		align       16
	L3:
		movdqa      xmm1, xmmword ptr [ecx + ebx]
		pcmpeqb     xmm1, xmm0
		pmovmskb    edx, xmm1
		test        edx, edx
		jnz         L5
		add         ecx, 16
		jnc         L3
	L4:
		pop         ebx
		ret

		align       16
	L5:
		bsf         edx, edx
		add         ecx, edx
		pop         ebx
		add         ecx, eax
		cmovc       eax, ecx
		ret

		#undef string
		#undef maxlen
	}
}

__declspec(naked) static size_t __cdecl strnlen386(const char *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov     eax, dword ptr [maxlen]
		mov     edx, dword ptr [string]
		test    eax, eax
		jz      L6
		push    ebx
		push    esi
		mov     esi, edx
		mov     ebx, eax
		and     esi, -4
		and     edx, 3
		jz      L2
		mov     ecx, dword ptr [esi]
		add     esi, 4
		add     ebx, edx
		or      ecx, 00000001H
		xor     edx, 3
		jz      L3
		or      ecx, 00000100H
		dec     edx
		jz      L3
		or      ecx, 00010000H
		jmp     L3

		align   16
	L1:
		sub     ebx, 4
		jbe     L5
	L2:
		mov     ecx, dword ptr [esi]
		add     esi, 4
	L3:
		lea     edx, [ecx - 01010101H]
		xor     ecx, -1
		and     edx, 80808080H
		and     ecx, edx
		jz      L1
		test    cx, cx
		jnz     L4
		sub     ebx, 2
		jbe     L5
		shr     ecx, 16
	L4:
		inc     eax
		add     cl, cl
		sbb     eax, ebx
	L5:
		pop     esi
		pop     ebx
	L6:
		ret

		#undef string
		#undef maxlen
	}
}

__declspec(naked) static size_t __cdecl strnlenCPUDispatch(const char *string, size_t maxlen)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [strnlenDispatch], offset strnlen386
		jmp     strnlen386
	L1:
		mov     dword ptr [strnlenDispatch], offset strnlenSSE2
		jmp     strnlenSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
