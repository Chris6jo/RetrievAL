#include <windows.h>
#include <errno.h>

#ifndef _M_IX86
unsigned long __cdecl strtoul(const char *nptr, char **endptr, int base)
{
	const char    *p;
	char          c;
	unsigned long number;
	unsigned      digval;
	unsigned long maxval;
	BOOLEAN       overflow;
	BOOLEAN       neg;

	overflow = FALSE;
	neg = FALSE;
	p = nptr;                   /* p is our scanning pointer */
	number = 0;                 /* start with zero */

	c = *p;                     /* read char */
	while (c == ' ' || ((unsigned char)c <= (unsigned char)'\r' && (unsigned char)c >= (unsigned char)'\t'))
		c = *(++p);             /* skip whitespace */

	if (c == '-')
	{
		neg = TRUE;             /* remember minus sign */
		c = *(++p);
	}
	else if (c == '+')
		c = *(++p);             /* skip sign */

	if (base == 1 || (unsigned int)base > (unsigned int)(10 + 'Z' - 'A' + 1))
	{
		/* bad base! */
		errno = EINVAL;
		if (endptr)
			/* store beginning of string in endptr */
			*endptr = (char *)nptr;
		return 0UL;             /* return 0 */
	}
	else if (base == 0)
	{
		/* determine base free-lance, based on first two chars of
		   string */
		if (c != '0')
			base = 10;
		else if (*(p + 1) == 'x' || *(p + 1) == 'X')
		{
			c = *(p += 2);
			base = 16;
		}
		else
			base = 8;
	}

	/* convert c to value */
	if (c >= '0' && (unsigned char)c <= (unsigned char)'9')
		digval = c - '0';
	else if (c >= 'A' && (unsigned char)c <= (unsigned char)'Z')
		digval = c - 'A' + 10;
	else if (c >= 'a' && (unsigned char)c <= (unsigned char)'z')
		digval = c - 'a' + 10;
	else
	{
		/* no number there; return 0 and point to beginning of
		   string */
		if (endptr)
			/* store beginning of string in endptr later on */
			*endptr = (char *)nptr;
		return 0UL;             /* return 0 */
	}

	/* if our number exceeds this, we will overflow on multiply */
	maxval = ULONG_MAX / base;

	for (; ; )                  /* exit in middle of loop */
	{
		/* we now need to compute number = number * base + digval,
		   but we need to know if overflow occured.  This requires
		   a tricky pre-check. */

		if (number < maxval || (number == maxval && digval <= (unsigned int)(ULONG_MAX % base)))
			/* we won't overflow, go ahead and multiply */
			number = number * base + digval;
		else
			/* we would have overflowed -- set the overflow flag */
			overflow = TRUE;

		c = *(++p);             /* read next digit */

	    /* convert c to value */
		if (c >= '0' && (unsigned char)c <= (unsigned char)'9')
			digval = c - '0';
		else if (c >= 'A' && (unsigned char)c <= (unsigned char)'Z')
			digval = c - 'A' + 10;
		else if (c >= 'a' && (unsigned char)c <= (unsigned char)'z')
			digval = c - 'a' + 10;
		else
			break;
		if (digval >= (unsigned int)base)
			break;              /* exit loop if bad digit found */
	}

	if (endptr != NULL)
		/* store pointer to char that stopped the scan */
		*endptr = (char *)p;

	if (overflow)
	{
		/* overflow or signed overflow occurred */
		errno = ERANGE;
		number = ULONG_MAX;
	}
	else if (neg)
		/* negate result if there was a neg sign */
		number = (unsigned long)(-(long)number);

	return number;              /* done. */
}
#else
#pragma warning(disable:4102)

__declspec(naked) unsigned long __cdecl strtoul(const char *nptr, char **endptr, int base)
{
	__asm
	{
		push    ebx                         ; store register
		push    esi
		push    edi

		#define nptr   (esp + 4 * (1 + 3))
		#define endptr (esp + 4 * (2 + 3))
		#define base   (esp + 4 * (3 + 3))
		#define sign   (esp + 4 * (3 + 3))

		mov     esi, dword ptr [nptr]       ; esi is our scanning pointer
		xor     eax, eax                    ; start with zero
		mov     cl, byte ptr [esi]          ; read char
		mov     ebx, dword ptr [base]
		jmp     L2

		align   16
	L1:
		mov     cl, byte ptr [esi + 1]      ; skip whitespace
		inc     esi
	L2:
		cmp     cl, ' '
		je      L1
		cmp     cl, 0DH
		ja      L3
		cmp     cl, 09H
		jae     L1
		jmp     L110

	L3:
		mov     byte ptr [sign], cl         ; store sign char
		and     ecx, 0FFH
		cmp     cl, '-'                     ; skip sign
		je      L4
		cmp     cl, '+'
		jne     L5
	L4:
		mov     cl, byte ptr [esi + 1]
		inc     esi

	L5:
		cmp     ebx, 1
		jae     L6
		cmp     cl, '0'                     ; determine base free-lance, based on first two chars of string
		jne     L10
		mov     cl, byte ptr [esi + 1]
		inc     esi
		cmp     cl, 'x'
		je      L31
		cmp     cl, 'X'
		je      L31
		jmp     L63

	L6:
		je      L7
		cmp     ebx, 10
		je      L10
		cmp     ebx, 16
		je      L30
		cmp     ebx, 8
		je      L60
		cmp     ebx, 10 + 'Z' - 'A' + 1
		jbe     L80
	L7:
		call    _errno                      ; bad base!
		mov     dword ptr [eax], EINVAL
		xor     eax, eax
		jmp     L110

		align   16
	L10:
		sub     cl, '0'                     ; base == 10
		jl      L11
		cmp     cl, '9' - '0'
		jbe     L13
	L11:
		jmp     L110                        ; no number there; return 0 and point to beginning of string
	L12:
		jmp     L112

		align   16
	L13:
		inc     esi
		lea     eax, [eax + eax * 4]
		lea     eax, [ecx + eax * 2]
		mov     cl, byte ptr [esi]          ; read next char
		sub     cl, '0'                     ; check and convert char to value
		jl      L12
		cmp     cl, '9' - '0'
		ja      L12
		cmp     eax, 19999999H
		jb      L13
		jne     L20
		cmp     cl, 5
		jbe     L13

	L20:
		call    _errno                      ; overflow there
		mov     ebx, dword ptr [endptr]
		mov     dword ptr [eax], ERANGE
		test    ebx, ebx
		mov     eax, 0FFFFFFFFH
		jz      L114

		align   16
	L21:
		mov     cl, byte ptr [esi + 1]      ; point to end of string
		inc     esi
		cmp     cl, '0'
		jl      L22
		cmp     cl, '9'
		jbe     L21
	L22:
		jmp     L111

		align   16
	L30:
		sub     cl, '0'                     ; base == 16
		jne     L32
		mov     cl, byte ptr [esi + 1]
		inc     esi
		cmp     cl, 'x'
		je      L31
		cmp     cl, 'X'
		jne     L37

		align   16
	L31:
		mov     cl, byte ptr [esi + 1]
		inc     esi
		sub     cl, '0'
	L32:
		jl      L34
		cmp     cl, '9' - '0'
		jbe     L36
		sub     cl, 'A' - '0'
		jb      L34
		cmp     cl, 'F' - 'A'
		jbe     L33
		sub     cl, 'a' - 'A'
		jb      L34
		cmp     cl, 'f' - 'a'
		ja      L34
	L33:
		add     cl, 10
		jmp     L36
	L34:
		jmp     L110                        ; no number there; return 0 and point to beginning of string
	L35:
		jmp     L112

		align   16
	L36:
		shl     eax, 4
		inc     esi
		or      eax, ecx
		mov     cl, byte ptr [esi]          ; read next char
	L37:
		sub     cl, '0'                     ; check and convert char to value
		jl      L35
		cmp     cl, '9' - '0'
		jbe     L39
		sub     cl, 'A' - '0'
		jb      L35
		cmp     cl, 'F' - 'A'
		jbe     L38
		sub     cl, 'a' - 'A'
		jb      L35
		cmp     cl, 'f' - 'a'
		ja      L35
	L38:
		add     cl, 10
	L39:
		test    eax, 0F0000000H
		jz      L36

	L50:
		call    _errno                      ; overflow there
		mov     ebx, dword ptr [endptr]
		mov     dword ptr [eax], ERANGE
		test    ebx, ebx
		mov     eax, 0FFFFFFFFH
		jz      L114

		align   16
	L51:
		mov     cl, byte ptr [esi + 1]      ; point to end of string
		inc     esi
		cmp     cl, '0'
		jl      L52
		cmp     cl, '9'
		jbe     L51
		cmp     cl, 'A'
		jb      L52
		cmp     cl, 'F'
		jbe     L51
		cmp     cl, 'a'
		jb      L52
		cmp     cl, 'f'
		jbe     L51
	L52:
		jmp     L111

		align   16
	L60:
		sub     cl, '0'                     ; base == 8
		jl      L61
		cmp     cl, '7' - '0'
		jbe     L62
	L61:
		jmp     L110                        ; no number there; return 0 and point to beginning of string

		align   16
	L62:
		shl     eax, 3
		inc     esi
		or      eax, ecx
		mov     cl, byte ptr [esi]          ; read next char
	L63:
		sub     cl, '0'                     ; check and convert char to value
		jl      L73
		cmp     cl, '7' - '0'
		ja      L73
		test    eax, 0E0000000H
		jz      L62

	L70:
		call    _errno                      ; overflow there
		mov     ebx, dword ptr [endptr]
		mov     dword ptr [eax], ERANGE
		test    ebx, ebx
		mov     eax, 0FFFFFFFFH
		jz      L114

		align   16
	L71:
		mov     cl, byte ptr [esi + 1]      ; point to end of string
		inc     esi
		cmp     cl, '0'
		jl      L72
		cmp     cl, '7'
		jbe     L71
	L72:
		jmp     L111
	L73:
		jmp     L112

		align   16
	L80:
		sub     cl, '0'                     ; base > 1 && base <= 36 && base != 10 && base != 16 && base != 8
		jl      L83
		cmp     cl, '9' - '0'
		jbe     L82
		sub     cl, 'A' - '0'
		jb      L83
		cmp     cl, 'Z' - 'A'
		jbe     L81
		sub     cl, 'a' - 'A'
		jb      L83
		cmp     cl, 'z' - 'a'
		ja      L83
	L81:
		add     cl, 10
	L82:
		cmp     cl, bl
		jb      L84
	L83:
		jmp     L110                        ; no number there; return 0 and point to beginning of string

		align   16
	L84:
		mul     ebx
		jc      L100
		add     eax, ecx
		jc      L100
		mov     cl, byte ptr [esi + 1]      ; read next char
		inc     esi
		sub     cl, '0'                     ; check and convert char to value
		jl      L90
		cmp     cl, '9' - '0'
		jbe     L86
		sub     cl, 'A' - '0'
		jb      L90
		cmp     cl, 'Z' - 'A'
		jbe     L85
		sub     cl, 'a' - 'A'
		jb      L90
		cmp     cl, 'z' - 'a'
		ja      L90
	L85:
		add     cl, 10
	L86:
		cmp     cl, bl
		jb      L84
	L90:
		jmp     L112

	L100:
		call    _errno                      ; overflow there
		mov     ebx, dword ptr [endptr]
		mov     dword ptr [eax], ERANGE
		test    ebx, ebx
		mov     eax, 0FFFFFFFFH
		jz      L114

		align   16
	L101:
		mov     cl, byte ptr [esi + 1]      ; point to end of string
		inc     esi
		sub     cl, '0'
		jl      L111
		cmp     cl, '9' - '0'
		jbe     L103
		sub     cl, 'A' - '0'
		jb      L111
		cmp     cl, 'Z' - 'A'
		jbe     L102
		sub     cl, 'a' - 'A'
		jb      L111
		cmp     cl, 'z' - 'a'
		ja      L111
	L102:
		add     cl, 10
	L103:
		cmp     cl, bl
		jb      L101
		jmp     L111

		align   16
	L110:
		mov     ebx, dword ptr [endptr]     ; store beginning of string in endptr
		mov     esi, dword ptr [nptr]       ; return 0
		test    ebx, ebx
		jz      L114
	L111:
		mov     dword ptr [ebx], esi
		jmp     L114
	L112:
		mov     ecx, dword ptr [endptr]
		test    ecx, ecx
		jz      L113
		mov     dword ptr [ecx], esi        ; store pointer to char that stopped the scan
	L113:
		cmp     byte ptr [sign], '-'
		jne     L114
		neg     eax                         ; negate result if there was a neg sign
	L114:
		pop     edi                         ; restore register
		pop     esi
		pop     ebx
		ret

		#undef nptr
		#undef endptr
		#undef base
		#undef sign
	}
}
#endif
