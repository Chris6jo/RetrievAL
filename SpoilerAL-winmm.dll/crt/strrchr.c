#include <string.h>

#ifndef _M_IX86
char * __cdecl strrchr(const char *string, int c)
{
	char *p, c2;

	if (!(char)c)
		return (char *)string + strlen(string);
	string--;
	p = NULL;
	while (c2 = *(++string))
		if (c2 == (char)c)
			p = (char *)string;
	return p;
}
#else
#pragma function(strlen)

char * __cdecl strrchrSSE2(const char *string, int c);
char * __cdecl strrchr386(const char *string, int c);
static char * __cdecl strrchrCPUDispatch(const char *string, int c);

static char *(__cdecl *strrchrDispatch)(const char *string, int c) = strrchrCPUDispatch;

__declspec(naked) char * __cdecl strrchr(const char *string, int c)
{
	__asm
	{
		jmp     dword ptr [strrchrDispatch]
	}
}

__declspec(naked) char * __cdecl strrchrSSE2(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     ecx, dword ptr [c]
		mov     edx, dword ptr [string]
		test    cl, cl
		jnz     char_is_not_null
		push    edx
		push    edx
		call    strlen
		pop     edx
		pop     ecx
		add     eax, ecx
		ret

		align   16
	char_is_not_null:
		push    ebx
		push    esi
		push    edi
		xor     eax, eax
		pxor    xmm1, xmm1
		movd    xmm2, ecx
		punpcklbw xmm2, xmm2
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		mov     ecx, edx
		or      edi, -1
		and     ecx, 15
		jz      loop_entry
		shl     edi, cl
		sub     edx, ecx
		jmp     loop_entry

		align   16
	loop_begin:
		and     ebx, edi
		jz      loop_increment
		mov     eax, edx
		mov     esi, ebx
	loop_increment:
		add     edx, 16
		or      edi, -1
	loop_entry:
		movdqa  xmm0, xmmword ptr [edx]
		pcmpeqb xmm1, xmm0
		pcmpeqb xmm0, xmm2
		pmovmskb ecx, xmm1
		pmovmskb ebx, xmm0
		and     ecx, edi
		jz      loop_begin
		bsf     ecx, ecx
		and     ebx, edi
		xor     ecx, 15
		shl     ebx, cl
		and     ebx, 7FFFH
		jz      process_stored_pointer
		bsr     eax, ebx
		sub     edx, ecx
		jmp     return_pointer

		align   16
	process_stored_pointer:
		test    eax, eax
		jz      epilog
		bsr     edx, esi
	return_pointer:
		add     eax, edx
	epilog:
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) char * __cdecl strrchr386(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		push    ebx
		xor     eax, eax
		mov     al, byte ptr [c + 4]                        // eax = search char
		mov     ecx, dword ptr [string + 4]                 // ecx = string
		test    al, al
		jz      char_is_null
		                                                    // set all 4 bytes of ebx to [value]
		push    ebp                                         // u preserve ebp
		push    esi                                         // v preserve esi
		push    edi                                         // u preserve edi
		mov     edx, eax                                    // v edx = 0/0/0/c
		shl     eax, 8                                      // u eax = 0/0/c/0
		mov     ebx, edx                                    // v ebx = 0/0/0/c
		or      edx, eax                                    // u eax = 0/0/c/c
		or      ebx, eax                                    // v ebx = 0/0/c/c
		shl     edx, 16                                     // u edx = c/c/0/0
		mov     eax, ecx                                    // v eax = string
		or      ebx, edx                                    // u ebx = all 4 bytes = [search char]
		xor     ebp, ebp                                    // v ebp = NULL
		and     ecx, 3
		jz      loop_begin
		dec     ecx
		jz      modulo1
		dec     ecx
		jz      modulo2
		mov     ch, byte ptr [eax]
		inc     eax
		jmp     modulo3

		align   16
	char_is_null:
		push    ecx
		push    ecx
		call    strlen
		pop     edx
		pop     ecx
		pop     ebx
		add     eax, ecx
		ret

		align   16
	modulo1:
		mov     cl, byte ptr [eax]
		inc     eax
		test    cl, cl
		jz      process_stored_pointer
		cmp     cl, bl
		jne     modulo2
		lea     ebp, [eax - 1]
	modulo2:
		mov     cx, word ptr [eax]
		add     eax, 2
		test    cl, cl
		jz      process_stored_pointer
		cmp     cl, bl
		jne     modulo3
		lea     ebp, [eax - 2]
	modulo3:
		test    ch, ch
		jz      process_stored_pointer
		cmp     ch, bl
		jne     loop_begin
		lea     ebp, [eax - 1]
		jmp     loop_begin

		align   16
	loop_begin:
		mov     ecx, dword ptr [eax]
		add     eax, 4
		mov     edx, ecx
		xor     ecx, ebx
		lea     esi, [edx - 01010101H]
		lea     edi, [ecx - 01010101H]
		xor     edx, -1
		xor     ecx, -1
		and     esi, edx
		and     ecx, edi
		and     esi, 80808080H
		jnz     null_is_found
		and     ecx, 80808080H
		jz      loop_begin
		mov     ebp, eax
		jmp     loop_begin

		align   16
	null_is_found:
		and     ecx, 00808080H
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
		nop                                                 // padding 1 byte
		cmp     dl, bl
		je      byte_1
	compare_byte_0:
		cmp     dh, bl
		je      byte_0

		align   16                                          // already aligned
	process_stored_pointer:
		mov     eax, ebp
		test    ebp, ebp
		jz      epilog
		and     ebp, 3
		jnz     epilog
		mov     ecx, dword ptr [eax - 4]
		bswap   ecx
		cmp     cl, bl
		je      byte_3
		cmp     ch, bl
		je      byte_2
		shr     ecx, 16
		nop                                                 // padding 1 byte
		cmp     cl, bl
		je      byte_1

		align   16                                          // already aligned
	byte_0:
		sub     eax, 4
		jmp     epilog

	byte_1:
		sub     eax, 3
		jmp     epilog

	byte_2:
		sub     eax, 2
		jmp     epilog

	byte_3:
		dec     eax

		align   16                                          // already aligned
	epilog:
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
