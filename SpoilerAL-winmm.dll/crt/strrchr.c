#include <string.h>

#ifndef _M_IX86
char * __cdecl strrchr(const char *string, int c)
{
	char *p, c2;

	if (!c)
		return (char *)string + strlen(string);
	p = NULL;
	while (c2 = *(string++))
		if (c2 == (char)c)
			p = (char *)string - 1;
	return p;
}
#else
#pragma function(strlen)

static char * __cdecl strrchrSSE2(const char *string, int c);
static char * __cdecl strrchr386(const char *string, int c);
static char * __cdecl strrchrCPUDispatch(const char *string, int c);

static char *(__cdecl * strrchrDispatch)(const char *string, int c) = strrchrCPUDispatch;

__declspec(naked) char * __cdecl strrchr(const char *string, int c)
{
	__asm
	{
		jmp     dword ptr [strrchrDispatch]
	}
}

__declspec(naked) static char * __cdecl strrchrSSE2(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     ecx, dword ptr [c]
		mov     edx, dword ptr [string]
		test    cl, cl
		jnz     chr_is_not_null
		push    eax
		push    eax
		call    strlen
		pop     edx
		pop     ecx
		add     eax, ecx
		ret

		align   16
	chr_is_not_null:
		push    ebx
		push    esi
		xor     eax, eax
		or      esi, -1
		pxor    xmm1, xmm1
		movd    xmm2, ecx
		punpcklbw xmm2, xmm2
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		mov     ecx, edx
		and     edx, -16
		and     ecx, 15
		jz      main_loop_entry
		shl     esi, cl
		movdqa  xmm0, xmmword ptr [edx]
		pcmpeqb xmm1, xmm0
		pcmpeqb xmm0, xmm2
		pmovmskb ecx, xmm1
		pmovmskb ebx, xmm0
		pxor    xmm1, xmm1
		and     ecx, esi
		and     ebx, esi
		or      ebx, ecx
		jz      main_loop_increment
		test    ecx, ecx
		jnz     null_is_found

		align   16
	main_loop:
		mov     eax, edx
		mov     esi, ebx
	main_loop_increment:
		add     edx, 16
	main_loop_entry:
		movdqa  xmm0, xmmword ptr [edx]
		pcmpeqb xmm1, xmm0
		pcmpeqb xmm0, xmm2
		pmovmskb ecx, xmm1
		pmovmskb ebx, xmm0
		or      ebx, ecx
		jz      main_loop_increment
		test    ecx, ecx
		jz      main_loop
	null_is_found:
		xor     ebx, ecx
		jz      process_stored_pointer
		bsf     ecx, ecx
		xor     ecx, 15
		shl     ebx, cl
		and     ebx, 7FFFH
		jz      process_stored_pointer
		bsr     ebx, ebx
		pop     esi
		lea     eax, [edx + ebx]
		pop     ebx
		sub     eax, ecx
		ret

		align   16
	process_stored_pointer:
		test    eax, eax
		jz      epilogue
		bsr     ecx, esi
		add     eax, ecx
	epilogue:
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static char * __cdecl strrchr386(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     eax, dword ptr [string]
		xor     ecx, ecx
		mov     cl, byte ptr [c]
		test    cl, cl
		jz      chr_is_null
		mov     edx, ecx
		push    ebx
		shl     ecx, 8
		push    ebp
		or      ecx, edx
		push    esi
		mov     ebx, ecx
		push    edi
		shl     ecx, 16
		xor     ebp, ebp
		or      ebx, ecx

		align   16
	misaligned_loop:
		test    eax, 3
		jz      main_loop
		mov     cl, byte ptr [eax]
		inc     eax
		cmp     cl, bl
		jne     is_null
		lea     ebp, [eax - 1]
		jmp     misaligned_loop
	is_null:
		test    cl, cl
		jnz     misaligned_loop
		mov     eax, ebp
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		align   16
	chr_is_null:
		push    eax
		push    eax
		call    strlen
		pop     edx
		pop     ecx
		add     eax, ecx
		ret

		align   16
	main_loop:
		mov     ecx, dword ptr [eax]
		add     eax, 4
		mov     edx, ecx
		xor     ecx, ebx
		lea     esi, [edx - 01010101H]
		lea     edi, [ecx + 7EFEFEFFH]
		xor     edx, -1
		xor     ecx, -1
		and     esi, edx
		xor     ecx, edi
		and     esi, 80808080H
		jnz     null_is_found
		and     ecx, 81010100H
		jz      main_loop
		and     ecx, 01010100H
		jnz     chr_is_found
		test    edi, edi
		js      main_loop
	chr_is_found:
		mov     ebp, eax
		jmp     main_loop

		align   16
	null_is_found:
		and     ecx, 01010100H
		jz      process_stored_pointer
		not     edx
		bswap   edx
		add     esi, esi
		jz      compare_byte_2
		and     esi, 00010100H
		jz      compare_byte_1
		shr     edx, 16
		jmp     compare_byte_0

		align   16
	compare_byte_2:
		cmp     dh, bl
		je      byte_2
	compare_byte_1:
		shr     edx, 16
		nop                                             // padding 1 byte
		cmp     dl, bl
		je      byte_1
	compare_byte_0:
		cmp     dh, bl
		je      byte_0

		align   16                                      // already aligned
	process_stored_pointer:
		mov     eax, ebp
		test    ebp, ebp
		jz      restore_register
		and     ebp, 3
		jnz     restore_register
		mov     ecx, dword ptr [eax - 4]
		bswap   ecx
		cmp     cl, bl
		je      byte_3
		cmp     ch, bl
		je      byte_2
		shr     ecx, 16
		nop                                             // padding 1 byte
		cmp     cl, bl
		je      byte_1

		align   16                                      // already aligned
	byte_0:
		sub     eax, 4
		jmp     restore_register

	byte_1:
		sub     eax, 3
		jmp     restore_register

	byte_2:
		sub     eax, 2
		jmp     restore_register

	byte_3:
		dec     eax

		align   16                                      // already aligned
	restore_register:
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static char * __cdecl strrchrCPUDispatch(const char *string, int c)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [strrchrDispatch], offset strrchr386
		jmp     strrchr386
	L1:
		mov     dword ptr [strrchrDispatch], offset strrchrSSE2
		jmp     strrchrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
