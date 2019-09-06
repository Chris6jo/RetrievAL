#include <string.h>

#pragma warning(disable:4414)

#ifndef _M_IX86
char * __cdecl _strichr(const char *string, int c)
{
	const char *p;
	char       c1, c2;

	c1 = (char)c | ('a' - 'A');
	if ((unsigned char)(c1 - 'a') > 'z' - 'a')
		return strchr(string, c);
	p = string - 1;
	do
		if (((c2 = *(++p)) | ('a' - 'A')) == c1)
			goto DONE;
	while (c2);
	p = NULL;
DONE:
	return (char *)p;
}
#else
extern const char casebitA[16];

static char * __cdecl strichrSSE2(const char *string, int c);
static char * __cdecl strichr386(const char *string, int c);
static char * __cdecl strichrCPUDispatch(const char *string, int c);

static char *(__cdecl * strichrDispatch)(const char *string, int c) = strichrCPUDispatch;

__declspec(naked) char * __cdecl _strichr(const char *string, int c)
{
	__asm
	{
		jmp     dword ptr [strichrDispatch]
	}
}

__declspec(naked) static char * __cdecl strichrSSE2(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     cl, byte ptr [c]
		mov     eax, dword ptr [string]
		or      cl, 'a' - 'A'
		xor     edx, edx
		mov     dl, cl
		sub     cl, 'a'
		cmp     cl, 'z' - 'a'
		ja      strchr
		pxor    xmm1, xmm1
		movd    xmm2, edx
		punpcklbw xmm2, xmm2
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		movdqa  xmm3, xmmword ptr [casebitA]
		test    eax, 15
		jz      main_loop_entry
		mov     ecx, eax
		and     eax, -16
		and     ecx, 15
		or      edx, -1
		shl     edx, cl
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqb xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqb xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb ecx, xmm0
		and     edx, ecx
		jnz     epilogue
		pxor    xmm1, xmm1

		align   16
	main_loop:
		add     eax, 16
	main_loop_entry:
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqb xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqb xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      main_loop
	epilogue:
		bsf     edx, edx
		mov     cl, byte ptr [eax + edx]
		add     eax, edx
		xor     edx, edx
		test    cl, cl
		cmovz   eax, edx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static char * __cdecl strichr386(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     dl, byte ptr [c]
		mov     eax, dword ptr [string]
		or      dl, 'a' - 'A'
		xor     ecx, ecx
		mov     cl, dl
		sub     dl, 'a'
		cmp     dl, 'z' - 'a'
		ja      strchr
		mov     edx, ecx
		push    ebx
		shl     ecx, 8
		push    esi
		or      ecx, edx
		push    edi
		mov     ebx, ecx
		shl     ecx, 16
		or      ebx, ecx
		jmp     is_aligned

		align   16
	misaligned_loop:
		mov     cl, byte ptr [eax]
		inc     eax
		mov     dl, cl
		or      cl, 'a' - 'A'
		cmp     cl, bl
		je      found
		test    dl, dl
		jz      retnull
	is_aligned:
		test    eax, 3
		jnz     misaligned_loop

		align   16
	main_loop:
		mov     ecx, dword ptr [eax]
		add     eax, 4
		mov     edx, ecx
		or      ecx, 20202020H
		mov     edi, ecx
		xor     ecx, ebx
		lea     esi, [ecx + 7EFEFEFFH]
		xor     ecx, -1
		xor     ecx, esi
		sub     edx, 01010101H
		and     edx, 80808080H
		xor     edi, -1
		test    edx, edi
		jnz     null_is_found
		and     ecx, 81010100H
		jz      main_loop
		and     ecx, 01010100H
		jnz     byte_0_to_2
		test    esi, esi
		js      main_loop
	found:
		dec     eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	retnull:
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	null_is_found:
		and     ecx, 01010100H
		jz      retnull
		test    dl, dl
		jnz     retnull
		test    ch, ch
		jnz     byte_0
		test    dh, dh
		jnz     retnull
		shl     ecx, 16
		jc      byte_1
		shr     edx, 24
		jc      retnull
		sub     eax, 2
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	byte_0_to_2:
		test    ch, ch
		jnz     byte_0
		shl     ecx, 16
		pop     edi
		sbb     eax, 2
		pop     esi
		pop     ebx
		ret

		align   16
	byte_0:
		sub     eax, 4
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	byte_1:
		sub     eax, 3
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static char * __cdecl strichrCPUDispatch(const char *string, int c)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [strichrDispatch], offset strichr386
		jmp     strichr386
	L1:
		mov     dword ptr [strichrDispatch], offset strichrSSE2
		jmp     strichrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
