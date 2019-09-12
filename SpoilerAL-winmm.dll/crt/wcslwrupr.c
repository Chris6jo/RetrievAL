#include <wchar.h>

#ifndef _M_IX86
wchar_t * __cdecl _wcslwr(wchar_t *string)
{
	wchar_t *p, c;

	if (c = *(p = string))
		do
			if (c >= L'A' && c <= L'Z')
				*p = c + (L'a' - L'A');
		while (c = *(++p));
	return string;
}

wchar_t * __cdecl _wcsupr(wchar_t *string)
{
	wchar_t *p, c;

	if (c = *(p = string))
		do
			if (c >= L'a' && c <= L'z')
				*p = c - (L'a' - L'A');
		while (c = *(++p));
	return string;
}
#else
#include <emmintrin.h>

extern const wchar_t casebitW[8];
extern const char maskbit[32];

static wchar_t * __cdecl wcslwrSSE2(wchar_t *string);
static wchar_t * __cdecl wcsuprSSE2(wchar_t *string);
static wchar_t * __cdecl wcslwruprSSE2(wchar_t *string);
static wchar_t * __cdecl wcslwr386(wchar_t *string);
static wchar_t * __cdecl wcsupr386(wchar_t *string);
static wchar_t * __cdecl wcslwrupr386(wchar_t *string);
static wchar_t * __cdecl wcslwrCPUDispatch(wchar_t *string);
static wchar_t * __cdecl wcsuprCPUDispatch(wchar_t *string);

static wchar_t *(__cdecl * wcslwrDispatch)(wchar_t *string) = wcslwrCPUDispatch;
static wchar_t *(__cdecl * wcsuprDispatch)(wchar_t *string) = wcsuprCPUDispatch;

__declspec(align(16)) static const wchar_t underalow[8] = {
	#define x60 ('a' - 1)
	x60, x60, x60, x60, x60, x60, x60, x60
	#undef x60
};
__declspec(align(16)) static const wchar_t underahigh[8] = {
	#define x40 ('A' - 1)
	x40, x40, x40, x40, x40, x40, x40, x40
	#undef x40
};
__declspec(align(16)) static const wchar_t zhigh[8] = {
	'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z'
};
__declspec(align(16)) static const wchar_t zlow[8] = {
	'z', 'z', 'z', 'z', 'z', 'z', 'z', 'z'
};

__declspec(naked) wchar_t * __cdecl _wcslwr(wchar_t *string)
{
	__asm
	{
		jmp     dword ptr [wcslwrDispatch]
	}
}

__declspec(naked) wchar_t * __cdecl _wcsupr(wchar_t *string)
{
	__asm
	{
		jmp     dword ptr [wcsuprDispatch]
	}
}

__declspec(naked) static wchar_t * __cdecl wcslwrSSE2(wchar_t *string)
{
	__asm
	{
		movdqa  xmm5, xmmword ptr [underahigh]
		movdqa  xmm6, xmmword ptr [zhigh]
		jmp     wcslwruprSSE2
	}
}

__declspec(naked) static wchar_t * __cdecl wcsuprSSE2(wchar_t *string)
{
	__asm
	{
		movdqa  xmm5, xmmword ptr [underalow]
		movdqa  xmm6, xmmword ptr [zlow]
		jmp     wcslwruprSSE2
	}
}

__declspec(naked) static wchar_t * __cdecl wcslwruprSSE2(wchar_t *string)
{
	__asm
	{
		mov     eax, dword ptr [esp + 4]                // string
		push    edi
		mov     ecx, eax
		mov     edi, eax
		pxor    xmm3, xmm3                              // set to zero
		movdqa  xmm4, xmmword ptr [casebitW]            // bit to change
		and     ecx, 15
		jz      L2
		test    eax, 1
		jnz     L3
		and     edi, -16
		xor     ecx, 15
		movdqa  xmm0, xmmword ptr [edi]                 // enter 16 byte
		movdqu  xmm1, xmmword ptr [maskbit + ecx + 1]
		movdqa  xmm2, xmm0                              // copy
		por     xmm1, xmm0                              // fill the non target bits to 1
		movdqa  xmm0, xmm1                              // copy
		pcmpeqw xmm1, xmm3                              // compare 8 words with zero
		pmovmskb ecx, xmm1                              // get one bit for each byte result
		movdqa  xmm1, xmm0                              // copy
		psubusw xmm0, xmm5                              // all word less than 'A'
		psubusw xmm1, xmm6                              // and 'Z' will be reset
		pcmpeqw xmm0, xmm3                              // xmm0 = (word <  'A') ? 0xFFFF : 0x0000
		pcmpeqw xmm1, xmm3                              // xmm1 = (word <= 'Z') ? 0xFFFF : 0x0000
		pandn   xmm0, xmm1                              // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pand    xmm0, xmm4                              // assign a mask for the appropriate words
		pxor    xmm0, xmm2                              // negation of the 5th bit - lowercase letters
		test    ecx, ecx
		jnz     L6

		align   16
	L1:
		movdqa  xmmword ptr [edi], xmm0
		add     edi, 16
	L2:
		movdqa  xmm0, xmmword ptr [edi]                 // enter 16 byte
		movdqa  xmm1, xmm0                              // copy
		movdqa  xmm2, xmm0                              //
		psubusw xmm0, xmm5                              // all word less than 'A'
		psubusw xmm1, xmm6                              // and 'Z' will be reset
		pcmpeqw xmm0, xmm3                              // xmm0 = (word <  'A') ? 0xFFFF : 0x0000
		pcmpeqw xmm1, xmm3                              // xmm1 = (word <= 'Z') ? 0xFFFF : 0x0000
		pandn   xmm0, xmm1                              // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		movdqa  xmm1, xmm2                              // copy
		pand    xmm0, xmm4                              // assign a mask for the appropriate words
		pcmpeqw xmm1, xmm3                              // compare 8 words with zero
		pxor    xmm0, xmm2                              // negation of the 5th bit - lowercase letters
		pmovmskb ecx, xmm1                              // get one bit for each byte result
		test    ecx, ecx
		jz      L1
		jmp     L6

		align   16
	L3:
		cmp     ecx, 15
		je      L5
		and     edi, -16
		xor     ecx, 15
		movdqa  xmm0, xmmword ptr [edi]                 // enter 16 byte
		movdqu  xmm1, xmmword ptr [maskbit + ecx]
		movdqa  xmm2, xmm0                              // copy
		pslldq  xmm0, 1                                 // adjust xmm value for compare
		por     xmm1, xmm0                              // fill the non target bits to 1
		movdqa  xmm0, xmm1                              // copy
		pcmpeqw xmm1, xmm3                              // compare 8 words with zero
		pmovmskb ecx, xmm1                              // get one bit for each byte result
		movdqa  xmm1, xmm0                              // copy
		psubusw xmm0, xmm5                              // all word less than 'A'
		psubusw xmm1, xmm6                              // and 'Z' will be reset
		pcmpeqw xmm0, xmm3                              // xmm0 = (word <  'A') ? 0xFFFF : 0x0000
		pcmpeqw xmm1, xmm3                              // xmm1 = (word <= 'Z') ? 0xFFFF : 0x0000
		pandn   xmm0, xmm1                              // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pand    xmm0, xmm4                              // assign a mask for the appropriate words
		psrldq  xmm0, 1                                 // adjust mask value to 16 byte alignment
		pxor    xmm0, xmm2                              // negation of the 5th bit - lowercase letters
		shr     ecx, 1
		jnz     L6
		movdqa  xmmword ptr [edi], xmm0
		add     edi, 15
		jmp     L5

		align   16
	L4:
		movdqu  xmmword ptr [edi], xmm0
		add     edi, 16
	L5:
		movdqu  xmm0, xmmword ptr [edi]                 // enter 16 byte
		movdqa  xmm1, xmm0                              // copy
		movdqa  xmm2, xmm0                              //
		psubusw xmm0, xmm5                              // all word less than 'A'
		psubusw xmm1, xmm6                              // and 'Z' will be reset
		pcmpeqw xmm0, xmm3                              // xmm0 = (word <  'A') ? 0xFFFF : 0x0000
		pcmpeqw xmm1, xmm3                              // xmm1 = (word <= 'Z') ? 0xFFFF : 0x0000
		pandn   xmm0, xmm1                              // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		movdqa  xmm1, xmm2                              // copy
		pand    xmm0, xmm4                              // assign a mask for the appropriate words
		pcmpeqw xmm1, xmm3                              // compare 8 words with zero
		pxor    xmm0, xmm2                              // negation of the 5th bit - lowercase letters
		pmovmskb ecx, xmm1                              // get one bit for each byte result
		test    ecx, ecx
		jz      L4

		align   16
	L6:
		shr     ecx, 1
		jc      L7
		bsf     ecx, ecx
		xor     ecx, 15
		movdqu  xmm1, xmmword ptr [maskbit + ecx]
		maskmovdqu xmm0, xmm1
	L7:
		pop     edi
		ret
	}
}

__declspec(naked) static wchar_t * __cdecl wcslwr386(wchar_t *string)
{
	__asm
	{
		mov     ecx, 'A'
		jmp     wcslwrupr386
	}
}

__declspec(naked) static wchar_t * __cdecl wcsupr386(wchar_t *string)
{
	__asm
	{
		mov     ecx, 'a'
		jmp     wcslwrupr386
	}
}

__declspec(naked) static wchar_t * __cdecl wcslwrupr386(wchar_t *string)
{
	__asm
	{
		mov     edx, dword ptr [esp + 4]                // string
		push    ebx

		align   16
	A100:
		// loop
		mov     ax, word ptr [edx]
		add     edx, 2
		test    ax, ax
		jz      A900                                    // end of string

	A200:
		mov     bx, ax                                  // check case
		sub     ax, cx
		cmp     ax, 'Z' - 'A'
		ja      A100

		// convert case
		mov     ax, word ptr [edx]
		xor     bx, 'a' - 'A'
		mov     word ptr [edx - 2], bx
		add     edx, 2
		test    ax, ax
		jnz     A200                                    // loop to next character

	A900:
		mov     eax, dword ptr [esp + 8]                // string
		pop     ebx
		ret
	}
}

__declspec(naked) static wchar_t * __cdecl wcslwrCPUDispatch(wchar_t *string)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcslwrDispatch], offset wcslwr386
		jmp     wcslwr386
	L1:
		mov     dword ptr [wcslwrDispatch], offset wcslwrSSE2
		jmp     wcslwrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}

__declspec(naked) static wchar_t * __cdecl wcsuprCPUDispatch(wchar_t *string)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcsuprDispatch], offset wcsupr386
		jmp     wcsupr386
	L1:
		mov     dword ptr [wcsuprDispatch], offset wcsuprSSE2
		jmp     wcsuprSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
