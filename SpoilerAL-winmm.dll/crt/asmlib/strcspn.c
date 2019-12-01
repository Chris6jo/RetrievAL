#include <stddef.h>

extern int __cdecl InstructionSet();

static size_t __cdecl strcspnSSE42(const char *string, const char *control);
static size_t __cdecl strcspnGeneric(const char *string, const char *control);
static size_t __cdecl strcspnCPUDispatch(const char *string, const char *control);

static size_t(__cdecl * strcspnDispatch)(const char *string, const char *control) = strcspnCPUDispatch;

// function dispatching
__declspec(naked) size_t __cdecl strcspn(const char *string, const char *control)
{
	__asm
	{
		jmp     dword ptr [strcspnDispatch]                 // Go to appropriate version, depending on instruction set
	}
}

// SSE4.2 version
__declspec(naked) static size_t __cdecl strcspnSSE42(const char *string, const char *control)
{
	__asm
	{
		push    esi
		push    edi
		mov     esi, dword ptr [esp + 12]                   // str
		mov     edi, dword ptr [esp + 16]                   // set
		xor     ecx, ecx                                    // span counter

	str_next2:
		movdqu  xmm2, xmmword ptr [esi]                     // str
		movdqu  xmm1, xmmword ptr [edi]                     // set
		pcmpistrm xmm1, xmm2, 00110000B                     // find in set, invert valid bits, return bit mask in xmm0
		movd    eax, xmm0
		jns     set_extends2

	set_finished2:
		cmp     ax, -1
		jne     str_finished2
		// first 16 characters matched, continue with next 16 characters (a terminating zero would never match)
		add     esi, 16                                     // next 16 bytes of str
		add     ecx, 16                                     // count span
		jmp     str_next2

	str_finished2:
		not     eax
		bsf     eax, eax
		add     eax, ecx
		pop     edi
		pop     esi
		ret

	set_loop2:
		and     eax, edx                                    // accumulate matches

	set_extends2:
		// the set is more than 16 bytes
		add     edi, 16
		movdqu  xmm1, xmmword ptr [edi]                     // next part of set
		pcmpistrm xmm1, xmm2, 00110000B                     // find in set, invert valid bits, return bit mask in xmm0
		movd    edx, xmm0
		jns     set_loop2
		mov     edi, dword ptr [esp + 16]                   // restore set pointer
		and     eax, edx                                    // accumulate matches
		jmp     set_finished2
	}
}

// Generic version
__declspec(naked) static size_t __cdecl strcspnGeneric(const char *string, const char *control)
{
#if 0
	__asm
	{
		push    esi
		push    edi
		mov     esi, dword ptr [esp + 12]                   // str pointer
	str_next20:
		mov     edi, dword ptr [esp + 16]                   // set pointer
		mov     al, dword ptr [esi]                         // read one byte from str
		test    al, al
		jz      str_finished20                              // str finished
	set_next20:
		mov     dl, dword ptr [edi]
		test    dl, dl
		jz      set_finished20
		inc     edi
		cmp     al, dl
		jne     set_next20
		// character match found, stop search
		jmp     str_finished20

	set_finished20:
		// end of set, mismatch found
		inc     esi
		jmp     str_next20

	str_finished20:
		// end of str, all match
		sub     esi, dword ptr [esp + 12]                   // calculate position
		mov     eax, esi
		pop     edi
		pop     esi
		ret
	}
#else
	// Algorithm:
	//	int __cdecl strcspn(const char *string, const char *control)
	//	{
	//		unsigned char map[256 / 8] = { 1 }, c;
	//		size_t        index;
	//
	//		while (c = *(control++))
	//			map[c >> 3] |= (1 << (c & 7));
	//		index = -1;
	//		string++;
	//		do
	//			c = string[index++];
	//		while (!(map[c >> 3] & (1 << (c & 7))));
	//		return index;
	//	}
	//
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		mov     edx, dword ptr [string]                     // edx = string
		mov     eax, dword ptr [control]                    // eax = control
		xor     ecx, ecx                                    // create and zero out char bit map
		push    0                                           // 256
		push    ecx                                         // 224
		push    ecx                                         // 192
		push    ecx                                         // 160
		push    ecx                                         // 128
		push    ecx                                         //  96
		push    ecx                                         //  64
		push    1                                           //  32
		jmp     listinit

		#define map     (esp)

		// Set control char bits in map
		align   16
	listnext:
		bts     dword ptr [map], ecx                        // init char bit map
	listinit:
		mov     cl, byte ptr [eax]
		inc     eax
		test    cl, cl
		jnz     listnext

		mov     eax, edx                                    // eax = string
		inc     edx                                         // edx = string + 1

		// Loop through comparing source string with control bits
		align   16
	dstnext:
		mov     cl, byte ptr [eax]
		inc     eax
		bt      dword ptr [map], ecx
		jnc     dstnext                                     // did not find char, continue

		// Return code
		add     esp, 32
		sub     eax, edx
		ret                                                 // __cdecl return

		#undef string
		#undef control
		#undef map
	}
#endif
}

// CPU dispatching for strcspn. This is executed only once
__declspec(naked) static size_t __cdecl strcspnCPUDispatch(const char *string, const char *control)
{
	__asm
	{
		// get supported instruction set
		call    InstructionSet
		// Point to generic version of strcspn
		mov     ecx, offset strcspnGeneric
		cmp     eax, 10                                     // check SSE4.2
		jb      Q200
		// SSE4.2 supported
		// Point to SSE4.2 version of strcspn
		mov     ecx, offset strcspnSSE42
	Q200:
		mov     dword ptr [strcspnDispatch], ecx
		// Continue in appropriate version
		jmp     ecx
	}
}
