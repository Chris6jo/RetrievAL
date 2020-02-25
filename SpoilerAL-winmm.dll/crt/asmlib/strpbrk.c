#define INVALID_PAGE 1
#if !INVALID_PAGE
extern int __cdecl InstructionSet();

static char * __cdecl strpbrkSSE42(const char *string, const char *control);
static char * __cdecl strpbrkGeneric(const char *string, const char *control);
static char * __cdecl strpbrkCPUDispatch(const char *string, const char *control);

static char *(__cdecl *strpbrkDispatch)(const char *string, const char *control) = strpbrkCPUDispatch;

// function dispatching
__declspec(naked) char * __cdecl strpbrk(const char *string, const char *control)
{
	__asm
	{
		jmp     dword ptr [strpbrkDispatch]                 // Go to appropriate version, depending on instruction set
	}
}

// SSE4.2 version
__declspec(naked) static char * __cdecl strpbrkSSE42(const char *string, const char *control)
{
#error Contains a bug that reads invalid page. The end of the string may be on a page boundary.
	__asm
	{
		push    esi
		mov     ecx, dword ptr [esp + 8]                    // str
		mov     esi, dword ptr [esp + 12]                   // set

	str_next2:
		movdqu  xmm2, xmmword ptr [ecx]                     // str
		movdqu  xmm1, xmmword ptr [esi]                     // set
		pcmpistrm xmm1, xmm2, 00110000B                     // find in set, invert valid bits, return bit mask in xmm0
		movd    eax, xmm0
		jns     set_extends2

	set_finished2:
		cmp     ax, -1
		jne     str_finished2
		// first 16 characters matched, continue with next 16 characters (a terminating zero would never match)
		add     ecx, 16                                     // next 16 bytes of str
		jmp     str_next2

	str_finished2:
		xor     eax, -1
		xor     edx, edx
		bsf     eax, eax
		mov     dl, byte ptr [eax + ecx]
		add     eax, ecx
		test    dl, dl
		pop     esi
		cmovz   eax, edx
		ret

	set_loop2:
		and     eax, edx                                    // accumulate matches

	set_extends2:
		// the set is more than 16 bytes
		add     esi, 16
		movdqu  xmm1, xmmword ptr [esi]                     // next part of set
		pcmpistrm xmm1, xmm2, 00110000B                     // find in set, invert valid bits, return bit mask in xmm0
		movd    edx, xmm0
		jns     set_loop2
		mov     esi, dword ptr [esp + 16]                   // restore set pointer
		and     eax, edx                                    // accumulate matches
		jmp     set_finished2
	}
}
#endif

// Generic version
#if !INVALID_PAGE
__declspec(naked) static char * __cdecl strpbrkGeneric(const char *string, const char *control)
#else
__declspec(naked) char * __cdecl strpbrk(const char *string, const char *control)
#endif
{
	__asm
	{
		push    esi
		push    edi
		mov     esi, dword ptr [esp + 12]                   // str pointer
		xor     eax, eax
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
		mov     eax, esi
		jmp     str_finished20

	set_finished20:
		// end of set, mismatch found
		inc     esi
		jmp     str_next20

	str_finished20:
		// end of str, all match
		pop     edi
		pop     esi
		ret
	}
}

#if !INVALID_PAGE
// CPU dispatching for strpbrk. This is executed only once
__declspec(naked) static char * __cdecl strpbrkCPUDispatch(const char *string, const char *control)
{
	__asm
	{
		// get supported instruction set
		call    InstructionSet

		// Point to generic version of strpbrk
		mov     ecx, offset strpbrkGeneric

		cmp     eax, 10                                     // check SSE4.2
		jb      Q200

		// SSE4.2 supported
		// Point to SSE4.2 version of strpbrk
		mov     ecx, offset strpbrkSSE42

	Q200:
		mov     dword ptr [strpbrkDispatch], ecx

		// Continue in appropriate version
		jmp     ecx
	}
}
#endif
