#include <string.h>

#pragma warning(disable:4414)

#ifndef _M_IX86
char * __cdecl _strrichr(const char *string, int c)
{
	char *p, c1, c2;

	c1 = (char)c | ('a' - 'A');
	if ((unsigned)(c1 - 'a') > 'z' - 'a')
		return strrchr(string, c);
	p = NULL;
	do
		if (((c2 = *(string++)) | ('a' - 'A')) == c1)
			p = (char *)string - 1;
	while (c2);
	return p;
}
#else
extern const char xmmconst_casebitA[16];
#define casebit xmmconst_casebitA

static char * __cdecl strrichrSSE2(const char *string, int c);
static char * __cdecl strrichr386(const char *string, int c);
static char * __cdecl strrichrCPUDispatch(const char *string, int c);

static char *(__cdecl *strrichrDispatch)(const char *string, int c) = strrichrCPUDispatch;

__declspec(naked) char * __cdecl _strrichr(const char *string, int c)
{
	__asm
	{
		jmp     dword ptr [strrichrDispatch]
	}
}

__declspec(naked) static char * __cdecl strrichrSSE2(const char *string, int c)
{
	extern char * __cdecl strrchrSSE2(const char *string, int c);

	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     eax, dword ptr [c]
		mov     edx, dword ptr [string]
		or      eax, 'a' - 'A'
		xor     ecx, ecx
		mov     cl, al
		sub     eax, 'a'
		cmp     al, 'z' - 'a'
		ja      strrchrSSE2
		push    ebx
		push    esi
		push    edi
		xor     eax, eax
		movd    xmm2, ecx
		punpcklbw xmm2, xmm2
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		movdqa  xmm3, xmmword ptr [casebit]
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
		pxor    xmm1, xmm1
		pcmpeqb xmm1, xmm0
		por     xmm0, xmm3
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
		jz      epilogue
		bsr     edx, esi
	return_pointer:
		add     eax, edx
	epilogue:
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static char * __cdecl strrichr386(const char *string, int c)
{
	extern char * __cdecl strrchr386(const char *string, int c);

	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     edx, dword ptr [c]                      // dl = search char
		mov     ecx, dword ptr [string]                 // ecx = string
		or      edx, 'a' - 'A'
		xor     eax, eax
		mov     al, dl
		sub     edx, 'a'
		cmp     dl, 'z' - 'a'
		ja      strrchr386
		                                                // set all 4 bytes of ebx to [value]
		mov     edx, eax                                // u edx = 0/0/0/c
		push    ebx                                     // v preserve ebx
		shl     eax, 8                                  // u eax = 0/0/c/0
		push    ebp                                     // v preserve ebp
		or      eax, edx                                // u eax = 0/0/c/c
		push    esi                                     // v preserve esi
		mov     ebx, eax                                // u ebx = 0/0/c/c
		push    edi                                     // v preserve edi
		shl     eax, 16                                 // u eax = c/c/0/0
		xor     ebp, ebp                                // v ebp = NULL
		or      ebx, eax                                // u ebx = all 4 bytes = [search char]
		mov     eax, ecx                                // v eax = string
		and     ecx, 3
		jz      loop_begin
		xor     ecx, 3
		jz      modulo3
		dec     ecx
		jz      modulo2
		mov     cl, byte ptr [eax]
		inc     eax
		test    cl, cl
		jz      process_stored_pointer
		cmp     cl, bl
		jne     modulo2
		lea     ebp, [eax - 1]
	modulo2:
		mov     cl, byte ptr [eax]
		inc     eax
		test    cl, cl
		jz      process_stored_pointer
		cmp     cl, bl
		jne     modulo3
		lea     ebp, [eax - 1]
	modulo3:
		mov     cl, byte ptr [eax]
		inc     eax
		test    cl, cl
		jz      process_stored_pointer
		cmp     cl, bl
		jne     loop_begin
		lea     ebp, [eax - 1]
		jmp     loop_begin

		align   16
	loop_begin:
		mov     ecx, dword ptr [eax]
		add     eax, 4
		mov     esi, ecx
		or      ecx, 20202020H
		mov     edx, ecx
		xor     ecx, ebx
		sub     esi, 01010101H
		lea     edi, [ecx + 7EFEFEFFH]
		xor     edx, -1
		xor     ecx, -1
		and     esi, edx
		xor     ecx, edi
		and     esi, 80808080H
		jnz     null_is_found
		and     ecx, 81010100H
		jz      loop_begin
		and     ecx, 01010100H
		jnz     char_is_found
		test    edi, edi
		js      loop_begin
	char_is_found:
		mov     ebp, eax
		jmp     loop_begin

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
		jz      epilogue
		test    eax, 3                                  // append 3 byte (and ebp,imm -> test eax,imm)
		jnz     epilogue
		mov     ecx, dword ptr [eax - 4]
		bswap   ecx
		or      ecx, 20202020H
		align   16                                      // padding 8 byte
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
		jmp     epilogue

	byte_1:
		sub     eax, 3
		jmp     epilogue

	byte_2:
		sub     eax, 2
		jmp     epilogue

	byte_3:
		dec     eax

		align   16                                      // already aligned
	epilogue:
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static char * __cdecl strrichrCPUDispatch(const char *string, int c)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [strrichrDispatch], offset strrichr386
		jmp     strrichr386

	L1:
		mov     dword ptr [strrichrDispatch], offset strrichrSSE2
		jmp     strrichrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
