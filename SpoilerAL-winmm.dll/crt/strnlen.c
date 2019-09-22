#include <string.h>

#ifndef _M_IX86
size_t __cdecl strnlen(const char *string, size_t maxlen)
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

		mov     eax, dword ptr [maxlen]                 // eax = maxlen
		mov     edx, dword ptr [string]                 // edx = string
		test    eax, eax                                // check if maxlen=0
		jz      retzero                                 // if maxlen=0, leave
		pxor    xmm1, xmm1                              // xmm1 = zero clear
		push    ebx                                     // preserve ebx
		lea     ebx, [edx + eax]                        // ebx = end of string
		mov     ecx, edx
		and     edx, -16
		and     ecx, 15
		jz      negate_count
		movdqa  xmm0, xmmword ptr [edx]
		pcmpeqb xmm0, xmm1
		pmovmskb edx, xmm0
		shr     edx, cl
		xor     ecx, 15
		test    edx, edx
		lea     ecx, [ecx + 1]
		jz      negate_count
		xor     ecx, ecx
		sub     ecx, eax                                // ecx = negative count
		jmp     found

	negate_count:
		sub     ecx, eax                                // ecx = negative count
		jae     epilogue

		align   16                                      // already aligned
	loop_begin:
		movdqa  xmm0, xmmword ptr [ebx + ecx]
		pcmpeqb xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jnz     found
		add     ecx, 16
		jnc     loop_begin
		pop     ebx                                     // restore ebx
	retzero:
		ret

		align   16
	found:
		bsf     edx, edx
		add     ecx, edx
		jc      epilogue
		add     eax, ecx
	epilogue:
		pop     ebx                                     // restore ebx
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

		mov     eax, dword ptr [maxlen]                 // 00000000 _ 8B. 44 24, 08
		mov     edx, dword ptr [string]                 // 00000004 _ 8B. 54 24, 04
		test    eax, eax                                // 00000008 _ 85. C0
		jz      L1                                      // 0000000A _ 74, 1D
		push    ebx                                     // 0000000C _ 53
		push    esi                                     // 0000000D _ 56
		mov     esi, edx                                // 0000000E _ 8B. F2
		mov     ebx, eax                                // 00000010 _ 8B. D8
		and     edx, 3                                  // 00000012 _ 83. E2, 03
		jz      L5                                      // 00000015 _ 74, 59
		dec     edx                                     // 00000017 _ 4A
		jz      L4                                      // 00000018 _ 74, 36
		dec     edx                                     // 0000001A _ 4A
		jz      L3                                      // 0000001B _ 74, 13
		mov     cl, byte ptr [esi]                      // 0000001D _ 8A. 0E
		inc     esi                                     // 0000001F _ 46
		test    cl, cl                                  // 00000020 _ 84. C9
		jz      L2                                      // 00000022 _ 74, 06
		dec     ebx                                     // 00000024 _ 4B
		jnz     L5                                      // 00000025 _ 75, 49
		pop     esi                                     // 00000027 _ 5E
		pop     ebx                                     // 00000028 _ 5B
	L1:
		ret                                             // 00000029 _ C3

	L2:
		xor     eax, eax                                // 0000002A _ 33. C0
		pop     esi                                     // 0000002C _ 5E
		pop     ebx                                     // 0000002D _ 5B
		ret                                             // 0000002E _ C3

		align   16                                      // 0000002F _ 90
	L3:
		mov     ecx, dword ptr [esi - 2]                // 00000030 _ 8B. 4E, FE
		add     esi, 2                                  // 00000033 _ 83. C6, 02
		lea     edx, [ecx - 01010000H]                  // 00000036 _ 8D. 91, FEFF0000
		xor     ecx, -1                                 // 0000003C _ 83. F1, FF
		and     edx, 80800000H                          // 0000003F _ 81. E2, 80800000
		and     ecx, edx                                // 00000045 _ 23. CA
		jnz     L9                                      // 00000047 _ 75, 57
		sub     ebx, 2                                  // 00000049 _ 83. EB, 02
		ja      L5                                      // 0000004C _ 77, 22
		jmp     L6                                      // 0000004E _ EB, 3D

		align   16
	L4:
		mov     ecx, dword ptr [esi - 1]                // 00000050 _ 8B. 4E, FF
		add     esi, 3                                  // 00000053 _ 83. C6, 03
		mov     edx, ecx                                // 00000056 _ 8B. D1
		xor     ecx, -1                                 // 00000058 _ 83. F1, FF
		sub     edx, 01010100H                          // 0000005B _ 81. EA, 01010100
		and     ecx, 80808000H                          // 00000061 _ 81. E1, 80808000
		and     ecx, edx                                // 00000067 _ 23. CA
		jnz     L7                                      // 00000069 _ 75, 25
		sub     ebx, 3                                  // 0000006B _ 83. EB, 03
		jbe     L6                                      // 0000006E _ 76, 1D

		align   16
	L5:
		mov     ecx, dword ptr [esi]                    // 00000070 _ 8B. 0E
		add     esi, 4                                  // 00000072 _ 83. C6, 04
		lea     edx, [ecx - 01010101H]                  // 00000075 _ 8D. 91, FEFEFEFF
		xor     ecx, -1                                 // 0000007B _ 83. F1, FF
		and     edx, 80808080H                          // 0000007E _ 81. E2, 80808080
		and     ecx, edx                                // 00000084 _ 23. CA
		jnz     L8                                      // 00000086 _ 75, 0B
		sub     ebx, 4                                  // 00000088 _ 83. EB, 04
		ja      L5                                      // 0000008B _ 77, E3
	L6:
		pop     esi                                     // 0000008D _ 5E
		pop     ebx                                     // 0000008E _ 5B
		ret                                             // 0000008F _ C3

		align   16
	L7:
		shr     ecx, 8                                  // 00000090 _ C1. E9, 08
	L8:
		test    cx, cx                                  // 00000093 _ 66: 85. C9
		jnz     L10                                     // 00000096 _ 75, 0B
		sub     ebx, 2                                  // 00000098 _ 83. EB, 02
		ja      L9                                      // 0000009B _ 77, 03
		pop     esi                                     // 0000009D _ 5E
		pop     ebx                                     // 0000009E _ 5B
		ret                                             // 0000009F _ C3

		align   16
	L9:
		shr     ecx, 16                                 // 000000A0 _ C1. E9, 10
	L10:
		sub     eax, ebx                                // 000000A3 _ 2B. C3
		cmp     cl, 80H                                 // 000000A5 _ 80. F9, 80
		pop     esi                                     // 000000A8 _ 5E
		adc     eax, 0                                  // 000000A9 _ 83. D0, 00
		pop     ebx                                     // 000000AC _ 5B
		ret                                             // 000000AD _ C3

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
