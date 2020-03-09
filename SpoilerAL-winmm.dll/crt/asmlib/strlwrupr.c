#include <emmintrin.h>

extern const char xmmconst_upperA[16];
extern const char xmmconst_lowerA[16];
extern const char xmmconst_azrangeA[16];
extern const char xmmconst_casebitA[16];
extern const char xmmconst_maskbit[32];
#define upper   xmmconst_upperA
#define lower   xmmconst_lowerA
#define azrange xmmconst_azrangeA
#define casebit xmmconst_casebitA
#define maskbit xmmconst_maskbit

extern int __cdecl InstructionSet();

__declspec(align(16)) static const char azhigh[16] = {      // define range for upper case
	'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z'
};
__declspec(align(16)) static const char azlow[16] = {       // define range for lower case
	'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z'
};

static char * __cdecl strlwrSSE42(char *string);
static char * __cdecl struprSSE42(char *string);
static char * __cdecl strlwruprSSE42(char *string);
#if 1
static char * __cdecl strlwrSSE2(char *string);
static char * __cdecl struprSSE2(char *string);
static char * __cdecl strlwruprSSE2(char *string);
#endif
static char * __cdecl strlwrGeneric(char *string);
static char * __cdecl struprGeneric(char *string);
#if 1
static char * __cdecl strlwruprGeneric(char *string);
#endif
static char * __cdecl strlwrCPUDispatch(char *string);
static char * __cdecl struprCPUDispatch(char *string);

// Pointer to appropriate version. Initially points to dispatcher
static char *(__cdecl *strlwrDispatch)(char *string) = strlwrCPUDispatch;
static char *(__cdecl *struprDispatch)(char *string) = struprCPUDispatch;

__declspec(naked) char * __cdecl _strlwr(char *string)
{
	__asm
	{
		jmp     dword ptr [strlwrDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

__declspec(naked) char * __cdecl _strupr(char *string)
{
	__asm
	{
		jmp     dword ptr [struprDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

// SSE4.2 version
__declspec(naked) static char * __cdecl strlwrSSE42(char *string)
{
	__asm
	{
		movdqa  xmm1, xmmword ptr [azhigh]                  // define range A-Z
		movdqa  xmm3, xmmword ptr [casebit]                 // bit to change
		jmp     strlwruprSSE42
	}
}

// SSE4.2 version
__declspec(naked) static char * __cdecl struprSSE42(char *string)
{
	__asm
	{
		movdqa  xmm1, xmmword ptr [azlow]                   // define range a-z
		movdqa  xmm3, xmmword ptr [casebit]                 // bit to change
		jmp     strlwruprSSE42
	}
}

__declspec(naked) static char * __cdecl strlwruprSSE42(char *string)
{
	__asm
	{
		// common code for strupr and strlwr
#if 0
#error Contains a bug that reads invalid page. The end of string may be on a page boundary.
		mov     edx, dword ptr [esp + 4]                    // string

	next:
		// loop
		movdqu  xmm2, xmmword ptr [edx]                     // read 16 bytes from string
		pcmpistrm xmm1, xmm2, 01000100B                     // find bytes in range A-Z or a-z, return mask in xmm0
		jz      last                                        // string ends in this paragraph
		pand    xmm0, xmm3                                  // mask AND case bit
		pxor    xmm2, xmm0                                  // change case bit in masked bytes of string
		movdqu  xmmword ptr [edx], xmm2                     // write changed value
		add     edx, 16
		jmp     next                                        // next 16 bytes
#else
		mov     ecx, dword ptr [esp + 4]                    // string
		mov     edx, ecx
		and     ecx, 15
		jz      next
		xor     ecx, 15
		and     edx, -16
		movdqu  xmm4, xmmword ptr [maskbit + ecx + 1]
		movdqa  xmm2, xmmword ptr [edx]                     // read 16 bytes from string
		por     xmm4, xmm2                                  // fill the non target bits to 1
		pcmpistrm xmm1, xmm4, 01000100B                     // find bytes in range A-Z or a-z, return mask in xmm0
		jnz     loop_entry
		jmp     last                                        // string ends in this paragraph

		align   16
	next:
		// loop
		movdqa  xmm2, xmmword ptr [edx]                     // read 16 bytes from string
		pcmpistrm xmm1, xmm2, 01000100B                     // find bytes in range A-Z or a-z, return mask in xmm0
		jz      last                                        // string ends in this paragraph
	loop_entry:
		pand    xmm0, xmm3                                  // mask AND case bit
		pxor    xmm2, xmm0                                  // change case bit in masked bytes of string
		movdqa  xmmword ptr [edx], xmm2                     // write changed value
		add     edx, 16
		jmp     next                                        // next 16 bytes
#endif

	last:
		// Write last 0-15 bytes
		// While we can read past the end of the string if precautions are made, we cannot write
		// past the end of the string, even if the value is unchanged, because the value may have
		// been changed in the meantime by another thread
		jnc     finish                                      // nothing changed, no need to write
		pand    xmm3, xmm0                                  // mask and case bit
		pxor    xmm2, xmm3                                  // change case bit

		// less elegant alternative, but probably faster if data needed again soon
		// write 8-4-2-1 bytes, if necessary
		pmovmskb eax, xmm0                                  // create bit mask
		cmp     eax, 10000000B
		jb      L10

		// there are at least 8 bytes to write
		movq    qword ptr [edx], xmm2
		psrldq  xmm2, 8
		add     edx, 8
		shr     eax, 8

	L10:
		cmp     eax, 1000B
		jb      L20

		// there are at least 4 bytes to write
		movd    dword ptr [edx], xmm2
		psrldq  xmm2, 4
		add     edx, 4
		shr     eax, 4

	L20:
		movd    ecx, xmm2                                   // use ecx for last 3 bytes
		cmp     eax, 10B
		jb      L30

		// there are at least 2 bytes to write
		mov     word ptr [edx], cx
		shr     ecx, 16
		add     edx, 2
		shr     eax, 2

	L30:
		cmp     eax, 1
		jb      finish

		// there is one more byte to write
		mov     byte ptr [edx], cl

	finish:
		mov     eax, dword ptr [esp + 4]                    // string
		ret
	}
}

#if 1
// SSE2 version
__declspec(naked) static char * __cdecl strlwrSSE2(char *string)
{
	__asm
	{
		mov     ecx, dword ptr [esp + 4]                    // string
		movdqa  xmm2, xmmword ptr [upper]
		jmp     strlwruprSSE2
	}
}

// SSE2 version
__declspec(naked) static char * __cdecl struprSSE2(char *string)
{
	__asm
	{
		mov     ecx, dword ptr [esp + 4]                    // string
		movdqa  xmm2, xmmword ptr [lower]
		jmp     strlwruprSSE2
	}
}

__declspec(naked) static char * __cdecl strlwruprSSE2(char *string)
{
	__asm
	{
		mov     eax, ecx
		mov     edx, ecx
		pxor    xmm3, xmm3                                  // set to zero
		movdqa  xmm4, xmmword ptr [azrange]
		movdqa  xmm5, xmmword ptr [casebit]                 // bit to change
		and     ecx, 15
		jz      loop_entry
		xor     ecx, 15
		and     edx, -16
		movdqu  xmm0, xmmword ptr [maskbit + ecx + 1]
		movdqa  xmm1, xmmword ptr [edx]                     // load 16 byte
		por     xmm0, xmm1                                  // fill the non target bits to 1
		pcmpeqb xmm3, xmm0                                  // compare 16 bytes with zero
		paddb   xmm0, xmm2                                  // all bytes greater than 'Z' if negative
		pmovmskb ecx, xmm3                                  // get one bit for each byte result
		pcmpgtb xmm0, xmm4                                  // xmm0 = (byte >= 'A' && byte <= 'Z') ? 0xFF : 0x00
		pand    xmm0, xmm5                                  // assign a mask for the appropriate bytes
		pxor    xmm0, xmm1                                  // negation of the 5th bit - lowercase letters
		test    ecx, ecx
		jnz     store_last_xmmword

		align   16
	loop_begin:
		movdqa  xmmword ptr [edx], xmm0
		add     edx, 16
	loop_entry:
		movdqa  xmm0, xmmword ptr [edx]                     // load 16 byte
		movdqa  xmm1, xmm0                                  // copy
		paddb   xmm0, xmm2                                  // all bytes greater than 'Z' if negative
		pcmpeqb xmm3, xmm1                                  // compare 16 bytes with zero
		pcmpgtb xmm0, xmm4                                  // xmm0 = (byte >= 'A' && byte <= 'Z') ? 0xFF : 0x00
		pmovmskb ecx, xmm3                                  // get one bit for each byte result
		pand    xmm0, xmm5                                  // assign a mask for the appropriate bytes
		pxor    xmm0, xmm1                                  // negation of the 5th bit - lowercase letters
		test    ecx, ecx
		jz      loop_begin
	store_last_xmmword:
		shr     ecx, 1
		jc      epilogue
		bsf     ecx, ecx
		push    edi
		xor     ecx, 15
		mov     edi, edx
		movdqu  xmm1, xmmword ptr [maskbit + ecx]
		maskmovdqu xmm0, xmm1
		pop     edi
	epilogue:
		ret
	}
}
#endif

// 386 version
__declspec(naked) static char * __cdecl strlwrGeneric(char *string)
{
#if 0
	__asm
	{
		mov     edx, dword ptr [esp + 4]                    // string

		// loop
	A100:
		mov     al, byte ptr [edx]
		test    al, al
		jz      A900                                        // end of string
		sub     al, 'A'
		cmp     al, 'Z' - 'A' + 1
		jb      A200                                        // is upper case
		inc     edx
		jmp     A100                                        // loop to next character

		// convert to lower case
	A200:
		add     al, 'a'
		mov     byte ptr [edx], al
		inc     edx
		jmp     A100

	A900:
		ret
	}
#else
	__asm
	{
		mov     ecx, 'A'
		jmp     strlwruprGeneric
	}
#endif
}

// 386 version
__declspec(naked) static char * __cdecl struprGeneric(char *string)
{
#if 0
	__asm
	{
		mov     edx, dword ptr [esp + 4]                    // string

		// loop
	B100:
		mov     al, byte ptr [edx]
		test    al, al
		jz      B900                                        // end of string
		sub     al, 'a'
		cmp     al, 'z' - 'a' + 1
		jb      B200                                        // is lower case
		inc     edx
		jmp     B100                                        // loop to next character

		// convert to upper case
	B200:
		add     al, 'A'
		mov     byte ptr [edx], al
		inc     edx
		jmp     B100

	B900:
		ret
	}
#else
	__asm
	{
		mov     ecx, 'a'
		jmp     strlwruprGeneric
	}
#endif
}

#if 1
__declspec(naked) static char * __cdecl strlwruprGeneric(char *string)
{
	__asm
	{
		push    ebx
		mov     edx, dword ptr [esp + 8]                    // string

		// loop
		align   16
	A100:
		mov     al, byte ptr [edx]
		inc     edx
		test    al, al
		jz      A900                                        // end of string

	A200:
		mov     bl, al                                      // check case
		sub     al, cl
		cmp     al, 'Z' - 'A' + 1
		jae     A100

		// convert case
		mov     al, byte ptr [edx]
		xor     bl, 'a' - 'A'
		mov     byte ptr [edx - 1], bl
		inc     edx
		test    al, al
		jnz     A200                                        // loop to next character

	A900:
		mov     eax, dword ptr [esp + 8]                    // string
		pop     ebx
		ret
	}
}
#endif

// CPU dispatching for strlwr. This is executed only once
__declspec(naked) static char * __cdecl strlwrCPUDispatch(char *string)
{
	__asm
	{
		// get supported instruction set
		call    InstructionSet

		// Point to generic version
		mov     ecx, offset strlwrGeneric

#if 1
		cmp     eax, 4                                      // check SSE2
		jb      Q100

		// SSE2 supported
		// Point to SSE2 version
		mov     ecx, offset strlwrSSE2
#endif

		cmp     eax, 10                                     // check SSE4.2
		jb      Q100

		// SSE4.2 supported
		// Point to SSE4.2 version
		mov     ecx, offset strlwrSSE42

	Q100:
		mov     dword ptr [strlwrDispatch], ecx

		// Continue in appropriate version
		jmp     ecx
	}
}

// CPU dispatching for strupr. This is executed only once
__declspec(naked) static char * __cdecl struprCPUDispatch(char *string)
{
	__asm
	{
		// get supported instruction set
		call    InstructionSet

		// Point to generic version
		mov     ecx, offset struprGeneric

#if 1
		cmp     eax, 4                                      // check SSE2
		jb      Q200

		// SSE2 supported
		// Point to SSE2 version
		mov     ecx, offset struprSSE2
#endif

		cmp     eax, 10                                     // check SSE4.2
		jb      Q200

		// SSE4.2 supported
		// Point to SSE4.2 version
		mov     ecx, offset struprSSE42

	Q200:
		mov     dword ptr [struprDispatch], ecx

		// Continue in appropriate version
		jmp     ecx
	}
}
