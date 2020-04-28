#include <wchar.h>
#ifndef _M_IX86

wchar_t * __cdecl _wmemrchr(const wchar_t *buffer, wchar_t c, size_t count)
{
	while (count--)
		if (buffer[count] == c)
			return (wchar_t *)buffer + count;
	return NULL;
}
#else
#include <xmmintrin.h>

wchar_t * __cdecl wmemrchrSSE2(const wchar_t *buffer, wchar_t c, size_t count);
wchar_t * __vectorcall internal_wmemrchrSSE2(const wchar_t *buffer, __m128 c, size_t count);
static wchar_t * __cdecl wmemrchr386(const wchar_t *buffer, wchar_t c, size_t count);
static wchar_t * __cdecl wmemrchrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count);

static wchar_t *(__cdecl *wmemrchrDispatch)(const wchar_t *buffer, wchar_t c, size_t count) = wmemrchrCPUDispatch;

__declspec(naked) wchar_t * __cdecl _wmemrchr(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		jmp     dword ptr [wmemrchrDispatch]
	}
}

__declspec(naked) wchar_t * __cdecl wmemrchrSSE2(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                      // edx = count
		mov     ecx, dword ptr [buffer]                     // ecx = buffer
		test    edx, edx                                    // check if count == 0
		jz      retnull                                     // if count == 0, leave
		movd    xmm0, dword ptr [c]                         // xmm0 = search char
		pshuflw xmm0, xmm0, 0
		movlhps xmm0, xmm0
		jmp     internal_wmemrchrSSE2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) wchar_t * __vectorcall internal_wmemrchrSSE2(const wchar_t *buffer, __m128 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      xmm0
		#define count  edx

		push    ebx                                         // preserve ebx
		push    esi                                         // preserve esi
		mov     eax, edx                                    // eax = count
		lea     ebx, [ecx + edx * 2 - 2]                    // ebx = last word of buffer
		add     edx, edx                                    // edx = count * 2
		and     ebx, -16 or 1                               // ebx = last xmmword of buffer
		add     ecx, edx                                    // ecx = end of buffer
		sub     ebx, edx                                    // ebx = last xmmword of buffer - count
		and     ecx, 15
		jz      aligned_loop
		test    ecx, 1
		jnz     unaligned
		movdqa  xmm1, xmmword ptr [ebx + eax * 2]
		pcmpeqw xmm1, xmm0
		pmovmskb edx, xmm1
		mov     esi, 3FFFH
		xor     ecx, 14
		shr     esi, cl
		and     edx, esi
		jnz     has_char_at_last_xmmword
		sub     ebx, ecx
		xor     ecx, 14
		shr     ecx, 1
		sub     ebx, 2
		sub     eax, ecx
		ja      aligned_loop
		jmp     retnull

		align   16
	aligned_loop:
		movdqa  xmm1, xmmword ptr [ebx + eax * 2]
		pcmpeqw xmm1, xmm0
		pmovmskb edx, xmm1
		test    edx, edx
		jnz     has_char
		sub     eax, 8
		ja      aligned_loop
		jmp     retnull

		align   16
	unaligned:
		dec     ecx
		jz      unaligned_loop
		movdqa  xmm1, xmmword ptr [ebx + eax * 2 - 1]
		psrldq  xmm1, 1
		pcmpeqw xmm1, xmm0
		pmovmskb edx, xmm1
		mov     esi, 3FFFH
		xor     ecx, 14
		shr     esi, cl
		and     edx, esi
		jnz     has_char_at_last_xmmword
		sub     ebx, ecx
		xor     ecx, 14
		shr     ecx, 1
		sub     ebx, 2
		sub     eax, ecx
		ja      unaligned_loop
		jmp     retnull

		align   16
	unaligned_loop:
		movdqu  xmm1, xmmword ptr [ebx + eax * 2]
		pcmpeqw xmm1, xmm0
		pmovmskb edx, xmm1
		test    edx, edx
		jnz     has_char
		sub     eax, 8
		ja      unaligned_loop
	retnull:
		xor     eax, eax
		pop     esi                                         // restore esi
		pop     ebx                                         // restore ebx
		ret

		align   16
	has_char_at_last_xmmword:
		xor     ecx, 14
		lea     esi, [eax + eax]
		sub     ecx, esi
		jbe     found
		or      esi, -1
		jmp     mask_first_xmmword

		align   16
	has_char:
		cmp     eax, 8
		jae     found
		mov     ecx, ebx
		or      esi, -1
		and     ecx, 14
		jz      found
	mask_first_xmmword:
		shl     esi, cl
		and     edx, esi
		jz      retnull
	found:
		bsr     edx, edx
		pop     esi                                         // restore esi
		lea     eax, [ebx + eax * 2 - 1]
		pop     ebx                                         // restore ebx
		add     eax, edx
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static wchar_t * __cdecl wmemrchr386(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     eax, dword ptr [count]                      // eax = count
		mov     edx, dword ptr [buffer]                     // edx = buffer
		test    eax, eax
		jz      retnull
		push    ebx                                         // preserve ebx
		mov     ecx, eax
		add     eax, eax
		mov     bx, word ptr [c + 4]                        // cx = search char
		add     eax, edx

		align   16
	loop_begin:
		mov     dx, word ptr [eax - 2]
		sub     eax, 2
		cmp     dx, bx
		je      found
		dec     ecx
		jnz     loop_begin
		xor     eax, eax
	found:
		pop     ebx                                         // restore ebx
	retnull:
		ret                                                 // __cdecl return

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static wchar_t * __cdecl wmemrchrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wmemrchrDispatch], offset wmemrchr386
		jmp     wmemrchr386

	L1:
		mov     dword ptr [wmemrchrDispatch], offset wmemrchrSSE2
		jmp     wmemrchrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
