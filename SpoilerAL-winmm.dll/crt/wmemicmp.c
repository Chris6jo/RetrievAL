#include <wchar.h>
#include "PageSize.h"

#ifndef _M_IX86
int __cdecl _wmemicmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count)
{
	int ret;

	(wchar_t *)buffer1 += count;
	(wchar_t *)buffer2 += count;
	count ^= -1;
	while (++count)
		if (ret = towlower(buffer1[count]) - towlower(buffer2[count]))
			return ret;
	return 0;
}
#else
static int __cdecl wmemicmpSSE2(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
static int __cdecl wmemicmp386(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
static int __cdecl wmemicmpCPUDispatch(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);

static int(__cdecl * wmemicmpDispatch)(const wchar_t *buffer1, const wchar_t *buffer2, size_t count) = wmemicmpCPUDispatch;

extern const wchar_t xmm_ahighW[8];
extern const wchar_t xmm_azrangeW[8];
extern const wchar_t xmm_casebitW[8];
#define ahigh   xmm_ahighW
#define azrange xmm_azrangeW
#define casebit xmm_casebitW

__declspec(naked) int __cdecl _wmemicmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count)
{
	__asm
	{
		jmp     dword ptr [wmemicmpDispatch]
	}
}

__declspec(naked) static int __cdecl wmemicmpSSE2(const wchar_t *buffer1, const wchar_t *buffer2, size_t count)
{
	__asm
	{
		#define buffer1 (esp + 4)
		#define buffer2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		push    edi
		xor     eax, eax                                // eax = NULL
		mov     esi, dword ptr [buffer1 + 12]           // esi = buffer1
		mov     edi, dword ptr [buffer2 + 12]           // edi = buffer2
		mov     ebx, dword ptr [count + 12]             // ebx = count
		lea     esi, [esi + ebx * 2]                    // esi = end of buffer1
		lea     edi, [edi + ebx * 2]                    // edi = end of buffer2
		xor     ebx, -1                                 // ebx = -count - 1
		movdqa  xmm4, xmmword ptr [ahigh]
		movdqa  xmm5, xmmword ptr [azrange]
		pxor    xmm6, xmm6                              // set to zero
		movdqa  xmm7, xmmword ptr [casebit]             // bit to change
		jmp     word_loop_increment

		align   16
	word_loop:
		movzx   eax, word ptr [esi + ebx * 2]
		movzx   edx, word ptr [edi + ebx * 2]
		sub     eax, 'A'
		sub     edx, 'A'
		cmp     eax, 'Z' - 'A' + 1
		lea     ecx, [eax + 'a' - 'A']
		cmovb   eax, ecx
		cmp     edx, 'Z' - 'A' + 1
		lea     ecx, [edx + 'a' - 'A']
		cmovb   edx, ecx
		sub     eax, edx
		jnz     epilogue
	word_loop_increment:
		inc     ebx
		jz      epilogue
		lea     edx, [edi + ebx * 2 + 1]
		lea     ecx, [esi + ebx * 2]
		and     edx, 14
		jnz     word_loop
		mov     edx, edi
		and     ecx, PAGE_SIZE - 1
		and     edx, 1
		jnz     unaligned_xmmword_loop

		align   16
	aligned_xmmword_loop:
		cmp     ecx, PAGE_SIZE - 15
		ja      word_loop                               // jump if cross pages
		movdqu  xmm0, xmmword ptr [esi + ebx * 2]       // load 16 byte
		movdqa  xmm1, xmmword ptr [edi + ebx * 2]       //
		movdqa  xmm2, xmm0                              // copy
		movdqa  xmm3, xmm1                              //
		psubw   xmm0, xmm4                              // all words less than 'A'
		psubw   xmm1, xmm4                              //
		psubusw xmm0, xmm5                              // and 'Z' will be reset
		psubusw xmm1, xmm5                              //
		pcmpeqw xmm0, xmm6                              // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pcmpeqw xmm1, xmm6                              //
		pand    xmm0, xmm7                              // assign a mask for the appropriate words
		pand    xmm1, xmm7                              //
		pxor    xmm0, xmm2                              // negation of the 5th bit - lowercase letters
		pxor    xmm1, xmm3                              //
		pcmpeqw xmm0, xmm1                              // compare
		pmovmskb edx, xmm0                              // get one bit for each byte result
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		add     ebx, 8
		jc      epilogue
		lea     ecx, [esi + ebx * 2]
		and     ecx, PAGE_SIZE - 1
		jmp     aligned_xmmword_loop

		align   16
	unaligned_xmmword_loop:
		cmp     ecx, PAGE_SIZE - 15
		ja      word_loop                               // jump if cross pages
		movdqu  xmm0, xmmword ptr [esi + ebx * 2]       // load 16 byte
		movdqu  xmm1, xmmword ptr [edi + ebx * 2]       //
		movdqa  xmm2, xmm0                              // copy
		movdqa  xmm3, xmm1                              //
		psubw   xmm0, xmm4                              // all words less than 'A'
		psubw   xmm1, xmm4                              //
		psubusw xmm0, xmm5                              // and 'Z' will be reset
		psubusw xmm1, xmm5                              //
		pcmpeqw xmm0, xmm6                              // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pcmpeqw xmm1, xmm6                              //
		pand    xmm0, xmm7                              // assign a mask for the appropriate words
		pand    xmm1, xmm7                              //
		pxor    xmm0, xmm2                              // negation of the 5th bit - lowercase letters
		pxor    xmm1, xmm3                              //
		pcmpeqw xmm0, xmm1                              // compare
		pmovmskb edx, xmm0                              // get one bit for each byte result
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		add     ebx, 8
		jc      epilogue
		lea     ecx, [esi + ebx * 2]
		and     ecx, PAGE_SIZE - 1
		jmp     unaligned_xmmword_loop

		align   16
	xmmword_not_equal:
		bsf     edx, edx
		shr     edx, 1
		add     ebx, edx
		jc      epilogue
		movzx   eax, word ptr [esi + ebx * 2]
		movzx   edx, word ptr [edi + ebx * 2]
		sub     eax, 'A'
		sub     edx, 'A'
		cmp     eax, 'Z' - 'A' + 1
		lea     ecx, [eax + 'a' - 'A']
		cmovb   eax, ecx
		cmp     edx, 'Z' - 'A' + 1
		lea     ecx, [edx + 'a' - 'A']
		cmovb   edx, ecx
		sub     eax, edx
	epilogue:
		pop     esi
		pop     edi
		pop     ebx
		ret

		#undef buffer1
		#undef buffer2
		#undef count
	}
}

__declspec(naked) static int __cdecl wmemicmp386(const wchar_t *buffer1, const wchar_t *buffer2, size_t count)
{
	__asm
	{
		#define buffer1 (esp + 4)
		#define buffer2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		push    edi
		xor     eax, eax
		mov     esi, dword ptr [buffer1 + 12]
		mov     edi, dword ptr [buffer2 + 12]
		mov     ecx, dword ptr [count + 12]
		xor     edx, edx
		lea     esi, [esi + ecx * 2]
		lea     edi, [edi + ecx * 2]
		xor     ecx, -1

		align   16
	loop_begin:
		inc     ecx
		jz      return_equal
		mov     ax, word ptr [esi + ecx * 2]
		mov     dx, word ptr [edi + ecx * 2]
		sub     eax, edx
		jz      loop_begin
		cmp     eax, 'a' - 'A'
		je      compare_above
		cmp     eax, 'A' - 'a'
		jne     return_not_equal
		xor     eax, eax
		lea     ebx, [edx - 'a']
		cmp     ebx, 'z' - 'a'
		jbe     loop_begin
		dec     eax
		jmp     epilogue

		align   16
	compare_above:
		xor     eax, eax
		lea     ebx, [edx - 'A']
		cmp     ebx, 'Z' - 'A'
		jbe     loop_begin
		inc     eax
		jmp     epilogue

		align   16
	return_not_equal:
		lea     eax, [eax + edx - 'A']
		sub     edx, 'A'
		cmp     eax, 'Z' - 'A'
		ja      secondary_to_lower
		add     eax, 'a' - 'A'
	secondary_to_lower:
		cmp     edx, 'Z' - 'A'
		ja      difference
		add     edx, 'a' - 'A'
	difference:
		sub     eax, edx
	return_equal:
	epilogue:
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef buffer1
		#undef buffer2
		#undef count
	}
}

__declspec(naked) static int __cdecl wmemicmpCPUDispatch(const wchar_t *buffer1, const wchar_t *buffer2, size_t count)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wmemicmpDispatch], offset wmemicmp386
		jmp     wmemicmp386
	L1:
		mov     dword ptr [wmemicmpDispatch], offset wmemicmpSSE2
		jmp     wmemicmpSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
