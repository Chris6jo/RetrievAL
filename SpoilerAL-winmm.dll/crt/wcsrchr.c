#include <windows.h>

#ifndef _M_IX86
wchar_t * __cdecl wcsrchr(const wchar_t *string, wint_t c)
{
	wchar_t *p, c2;

	if (!c)
		return (wchar_t *)string + wcslen(string);
	p = NULL;
	while (c2 = *(string++))
		if (c2 == c)
			p = (wchar_t *)string - 1;
	return p;
}
#else
#pragma function(wcslen)

static wchar_t * __cdecl wcsrchrSSE2(const wchar_t *string, wint_t c);
static wchar_t * __cdecl wcsrchr386(const wchar_t *string, wint_t c);
static wchar_t * __cdecl wcsrchrCPUDispatch(const wchar_t *string, wint_t c);

static wchar_t *(__cdecl * wcsrchrDispatch)(const wchar_t *string, wint_t c) = wcsrchrCPUDispatch;

__declspec(naked) wchar_t * __cdecl wcsrchr(const wchar_t *string, wint_t c)
{
	__asm
	{
		jmp     dword ptr [wcsrchrDispatch]
	}
}

__declspec(naked) static wchar_t * __cdecl wcsrchrSSE2(const wchar_t *string, wint_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     dx, word ptr [c]
		mov     eax, dword ptr [string]
		test    dx, dx
		jnz     chr_is_not_null
		push    eax
		push    eax
		call    wcslen
		pop     edx
		pop     ecx
		lea     eax, [ecx + eax * 2]
		ret

		align   16
	chr_is_not_null:
		push    ebx
		xor     ebx, ebx
		pxor    xmm1, xmm1
		movd    xmm2, dx
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		test    eax, 1
		jnz     unaligned
		test    eax, 15
		jz      aligned_loop
		mov     ecx, eax
		and     eax, -16
		and     ecx, 15
		or      ebx, -1
		shl     ebx, cl
		movdqa  xmm0, xmmword ptr [eax]
		add     eax, 16
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		pmovmskb ecx, xmm1
		pmovmskb edx, xmm0
		pxor    xmm1, xmm1
		and     ecx, ebx
		and     edx, ebx
		xor     ebx, ebx
		or      edx, ecx
		jnz     aligned_is_null

		align   16
	aligned_loop:
		movdqa  xmm0, xmmword ptr [eax]
		add     eax, 16
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      aligned_loop
		pmovmskb ecx, xmm1
	aligned_is_null:
		test    ecx, ecx
		jnz     null_found
		bsr     edx, edx
		lea     ebx, [eax + edx - 16]
		jmp     aligned_loop

		align   16
	unaligned:
		mov     ecx, eax
		and     eax, -16
		inc     ecx
		dec     eax
		and     ecx, 15
		jz      unaligned_loop
		mov     ebx, -1
		add     eax, 16
		shl     ebx, cl
		movdqa  xmm0, xmmword ptr [eax - 15]
		pslldq  xmm0, 1
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		pmovmskb ecx, xmm1
		pmovmskb edx, xmm0
		pxor    xmm1, xmm1
		and     ecx, ebx
		and     edx, ebx
		xor     ebx, ebx
		or      edx, ecx
		jnz     unaligned_is_null

		align   16
	unaligned_loop:
		movdqu  xmm0, xmmword ptr [eax]
		add     eax, 16
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      unaligned_loop
		pmovmskb ecx, xmm1
	unaligned_is_null:
		test    ecx, ecx
		jnz     null_found
		bsr     edx, edx
		lea     ebx, [eax + edx - 16]
		jmp     unaligned_loop

		align   16
	null_found:
		xor     edx, ecx
		jz      epilogue
		bsf     ecx, ecx
		xor     ecx, 15
		sub     eax, 16
		shl     edx, cl
		sub     eax, ecx
		and     edx, 7FFFH
		jz      epilogue
		bsr     edx, edx
		add     eax, edx
		pop     ebx
		ret

		align   16
	epilogue:
		mov     eax, ebx
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static wchar_t * __cdecl wcsrchr386(const wchar_t *string, wint_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     dx, word ptr [c]
		mov     ecx, dword ptr [string]
		test    dx, dx
		jz      chr_is_null
		push    ebx
		sub     ecx, 2
		xor     ebx, ebx

		align   16
	main_loop:
		mov     ax, word ptr [ecx + 2]
		add     ecx, 2
		cmp     ax, dx
		jne     is_null
		mov     ebx, ecx
		jmp     main_loop
	is_null:
		test    ax, ax
		jnz     main_loop
		mov     eax, ebx
		pop     ebx
		ret

		align   16
	chr_is_null:
		push    ecx
		push    ecx
		call    wcslen
		pop     edx
		pop     ecx
		lea     eax, [ecx + eax * 2]
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static wchar_t * __cdecl wcsrchrCPUDispatch(const wchar_t *string, wint_t c)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcsrchrDispatch], offset wcsrchr386
		jmp     wcsrchr386
	L1:
		mov     dword ptr [wcsrchrDispatch], offset wcsrchrSSE2
		jmp     wcsrchrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
