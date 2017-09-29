#ifdef __BORLANDC__
#pragma warn -8027
#pragma warn -8060
#endif

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>    // using IsDBCSLeadByteEx
#undef isleadbyte
#define isleadbyte(c) IsDBCSLeadByteEx(CP_THREAD_ACP, c)
#else
#include <ctype.h>      // using isleadbyte
#endif

#ifdef _WIN32
#if defined(_MSC_VER) && _MSC_VER >= 1700
#include <winternl.h>   // using PANSI_STRING, PUNICODE_STRING
#else
typedef struct {
	USHORT Length;
	USHORT MaximumLength;
	PCHAR  Buffer;
} ANSI_STRING, *PANSI_STRING;
typedef struct {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;
#endif
#endif

#if !defined(_MSC_VER) || _MSC_VER >= 1700
typedef long long          long_long;
typedef unsigned long long unsigned_long_long;
#else
typedef __int64            long_long;
typedef unsigned __int64   unsigned_long_long;
#endif

#include <stddef.h>     // using ptrdiff_t
#include <limits.h>     // using CHAR_BIT, _I[N]_MIN, _I[N]_MAX, _UI[N]_MAX, CHAR_MAX, SHRT_MAX, LONG_MAX, LLONG_MAX, INT_MAX

// standard integer type definition
#if !defined(_MSC_VER) || _MSC_VER >= 1600
#include <stdint.h>
#else
typedef __int8           int8_t;
typedef unsigned __int8  uint8_t;
typedef __int16          int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32          int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64          int64_t;
typedef unsigned __int64 uint64_t;
typedef __int64          intmax_t;
typedef unsigned __int64 uintmax_t;
typedef ptrdiff_t        intptr_t;
typedef size_t           uintptr_t;
#ifndef SIZE_MAX
#ifdef _WIN64
#define SIZE_MAX _UI64_MAX
#else
#define SIZE_MAX UINT_MAX
#endif
#endif
#define INT8_MIN    _I8_MIN
#define INT8_MAX    _I8_MAX
#define INT16_MIN   _I16_MIN
#define INT16_MAX   _I16_MAX
#define INT32_MIN   _I32_MIN
#define INT32_MAX   _I32_MAX
#define INT64_MIN   _I64_MIN
#define INT64_MAX   _I64_MAX
#define INTMAX_MIN  _I64_MIN
#define INTMAX_MAX  _I64_MAX
#define INTPTR_MIN  (-INTPTR_MAX - 1)
#define INTPTR_MAX  (UINTPTR_MAX >> 1)
#define UINT8_MAX   _UI8_MAX
#define UINT16_MAX  _UI16_MAX
#define UINT32_MAX  _UI32_MAX
#define UINT64_MAX  _UI64_MAX
#define UINTMAX_MAX _UI64_MAX
#define UINTPTR_MAX SIZE_MAX
#endif

#define INT_IS_CHAR      (INT_MAX == CHAR_MAX)
#define INT_IS_SHRT      (INT_MAX == SHRT_MAX)
#define INT_IS_LONG      (INT_MAX == LONG_MAX)
#define INT_IS_LLONG     (INT_MAX == LLONG_MAX)
#define INT_IS_INTMAX    (INT_MAX == INTMAX_MAX)
#define INTPTR_IS_CHAR   (INTPTR_MAX == CHAR_MAX)
#define INTPTR_IS_SHRT   (INTPTR_MAX == SHRT_MAX)
#define INTPTR_IS_LONG   (INTPTR_MAX == LONG_MAX)
#define INTPTR_IS_LLONG  (INTPTR_MAX == LLONG_MAX)
#define INTPTR_IS_INTMAX (INTPTR_MAX == INTMAX_MAX)
#define INTMAX_IS_LLONG  (INTMAX_MAX == LLONG_MAX)

#include <stdbool.h>

// byte-order definition
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__MINGW32__)
#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN    4321
#define __BYTE_ORDER    __LITTLE_ENDIAN
#elif defined(__GNUC__)
#include <sys/param.h>
#endif

#include <stdlib.h>     // using _countof
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif

#include <errno.h>      // using ERANGE, EOVERFLOW
#ifndef ERANGE
#define ERANGE E2BIG
#endif
#ifndef EOVERFLOW
#ifdef _MSC_VER
#define EOVERFLOW 132   // compiler dependent
#else
#define EOVERFLOW ERANGE
#endif
#endif

#include <math.h>       // using modf
#include <float.h>      // using DBL_MANT_DIG, DBL_MAX_EXP, DBL_MAX_10_EXP, DBL_DECIMAL_DIG
#pragma function(ceil)

#ifdef _DEBUG
#include <assert.h>     // using assert
#endif

#if defined(_MSC_VER) || defined(__BORLANDC__)
#define inline __inline
#endif

#ifndef NEAR
#define NEAR
#endif
#ifndef FAR
#define FAR
#endif

// floating-point constants
#define LONGDOUBLE_IS_QUAD         (defined(LDBL_MANT_DIG) && (LDBL_MANT_DIG == 113))
#define LONGDOUBLE_IS_X86_EXTENDED (defined(LDBL_MANT_DIG) && (LDBL_MANT_DIG == 64))
#define LONGDOUBLE_IS_DOUBLE       (!defined(LDBL_MANT_DIG) || (LDBL_MANT_DIG == DBL_MANT_DIG))
#define DOUBLE_IS_IEEE754          (DBL_MANT_DIG == 53)

#if LONGDOUBLE_IS_DOUBLE
#ifndef LDBL_MANT_DIG
#define LDBL_MANT_DIG DBL_MANT_DIG
#endif
#ifndef LDBL_MAX_EXP
#define LDBL_MAX_EXP DBL_MAX_EXP
#endif
#ifndef LDBL_MAX_10_EXP
#define LDBL_MAX_10_EXP DBL_MAX_10_EXP
#endif
#ifndef LDBL_DECIMAL_DIG
#define LDBL_DECIMAL_DIG DBL_DECIMAL_DIG
#endif
#endif

#if LONGDOUBLE_IS_QUAD
#define LDBL_BIT      128
#elif LONGDOUBLE_IS_X86_EXTENDED
#define LDBL_BIT      80
#elif LONGDOUBLE_IS_DOUBLE
#define LDBL_BIT      64
#endif
#define LDBL_SIGN_BIT 1
#define LDBL_NORM_BIT 1
#define LDBL_MANT_BIT (LDBL_MANT_DIG - LDBL_NORM_BIT)
#if !LONGDOUBLE_IS_X86_EXTENDED
#define LDBL_EXP_BIT  (LDBL_BIT - LDBL_SIGN_BIT - LDBL_MANT_BIT)
#else
#define LDBL_EXP_BIT  (LDBL_BIT - LDBL_SIGN_BIT - LDBL_MANT_DIG)
#endif
#define LDBL_EXP_BIAS (LDBL_MAX_EXP - 1)

// floating-point type definition
#if LONGDOUBLE_IS_DOUBLE
typedef double long_double;
#else
typedef long double long_double;
#endif

// floating-point structures
typedef union _LONGDOUBLE {
	long_double value;
	struct {
#if !LONGDOUBLE_IS_X86_EXTENDED
#if __BYTE_ORDER == __LITTLE_ENDIAN
		uintmax_t mantissa : LDBL_MANT_BIT;
		uintmax_t exponent : LDBL_EXP_BIT;
		uintmax_t sign     : LDBL_SIGN_BIT;
#else
		uintmax_t sign     : LDBL_SIGN_BIT;
		uintmax_t exponent : LDBL_EXP_BIT;
		uintmax_t mantissa : LDBL_MANT_BIT;
#endif
#elif __BYTE_ORDER == __LITTLE_ENDIAN
		struct {
			uint64_t mantissa  : LDBL_MANT_BIT;
			uint64_t normalize : LDBL_NORM_BIT;
		};
		struct {
			uint16_t exponent  : LDBL_EXP_BIT;
			uint16_t sign      : LDBL_SIGN_BIT;
		};
#else
		struct {
			uint16_t sign      : LDBL_SIGN_BIT;
			uint16_t exponent  : LDBL_EXP_BIT;
		};
		struct {
			uint64_t normalize : LDBL_NORM_BIT;
			uint64_t mantissa  : LDBL_MANT_BIT;
		};
#endif
	};
} LONGDOUBLE, NEAR *PLONGDOUBLE, FAR *LPLONGDOUBLE;

// floating-point macro function
#if LONGDOUBLE_IS_DOUBLE && DOUBLE_IS_IEEE754
#undef isinf
#define isinf(x) ((*(uint64_t *)&(x) & 0x7FF0000000000000) == 0x7FF0000000000000)
#undef isnan
#define isnan(x) (isinf(x) && (*(uint64_t *)&(x) & 0x000FFFFFFFFFFFFF))
#undef signbit
#define signbit(x) (*(int64_t *)&(x) < 0)
#endif

#if LONGDOUBLE_IS_DOUBLE
#undef isinfl
#define isinfl isinf
#undef isnanl
#define isnanl isnan
#undef signbitl
#define signbitl signbit
#undef modfl
#define modfl modf
#endif

#ifdef __BORLANDC__
#define isinfl !_finitel
#define isnanl _isnanl
#define signbitl(x) ((LPLONGDOUBLE)&(x))->sign
#endif

#ifdef __MINGW32__
#define isinfl(x) (__fpclassifyl(x) == FP_INFINITE)
#define isnanl __isnanl
#define signbitl __signbitl
#endif

// mathematical constants
#ifndef M_LOG10_2
#define M_LOG10_2 0.301029995663981195213738894724	// log10(2), ln(2)/ln(10)
#endif

// mathematical constant value macro funcion
#define CEIL(x) ((intptr_t)(x) + ((x) > (intptr_t)(x)))

// get number of digits, it is constant value macro funcion
#define DEC_DIG(bit) (size_t)CEIL((bit) * M_LOG10_2)
#define OCT_DIG(bit) (((bit) + (3 - 1)) / 3)
#define HEX_DIG(bit) (((bit) + (4 - 1)) / 4)

// constants
#define UINTMAX_OCT_DIG  OCT_DIG(sizeof(uintmax_t) * CHAR_BIT)
#define MANTISSA_HEX_DIG HEX_DIG(LDBL_MANT_BIT)
#define CVTBUFSIZE       ((LDBL_MAX_10_EXP + 1) + 40)
#define EXPBUFSIZE       (2 + DEC_DIG(LDBL_EXP_BIT - 1) + 1)

// the other macro funcion
#define CONCATIFY(a, b) a##b
#define CONCAT(a, b)    CONCATIFY(a, b)

/* Format flags. */
#define FL_SIGN         0x0001  /* put plus or minus in front */
#define FL_SIGNSP       0x0002  /* put space or minus in front */
#define FL_LEFT         0x0004  /* left justify */
#define FL_LEADZERO     0x0008  /* pad with leading zeros */
#define FL_ALTERNATE    0x0010  /* alternate form requested */
#define FL_QUOTE        0x0020
#define FL_UP           0x0040
#define FL_UNSIGNED     0x0080
#define FL_TYPE_E       0x0100
#define FL_TYPE_G       0x0200
#define FL_TYPE_A       0x0400

/* Conversion flags. */
enum {
	C_DEFAULT,
	C_CHAR,
	C_SHORT,
	C_LONG,
	C_LLONG,
#if !INTMAX_IS_LLONG
	C_INTMAX,
#endif
	C_LDOUBLE,
};

#if INTMAX_IS_LLONG
#define C_INTMAX C_LLONG
#endif

#define C_UNSIGNED (1 << CHAR_BIT)

#if INTPTR_IS_CHAR
#define C_PTRDIFF C_CHAR
#elif INTPTR_IS_SHRT
#define C_PTRDIFF C_SHORT
#elif INTPTR_IS_LONG
#define C_PTRDIFF C_LONG
#elif INTPTR_IS_LLONG
#define C_PTRDIFF C_LLONG
#else
#define C_PTRDIFF C_INTMAX
#endif

#define C_SIZE (C_UNSIGNED | C_PTRDIFF)

#ifdef _WIN32
#define C_WCHAR (C_UNSIGNED | C_SHORT)
#endif

// macro functions
#ifndef ALIGN
#define ALIGN(x, a) (((x) + (a) - 1) & ~((a) - 1))
#endif

#ifndef ISDIGIT
#define ISDIGIT(c) ((c) >= '0' && (c) <= '9')
#endif

#define OUTCHAR(dest, end, c) \
    if (++(dest) < (end))     \
        *(dest) = (c)

// internal variables
#ifndef _MSC_VER
static const char digitsHexLarge[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
static const char digitsHexSmall[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
#else
extern const char digitsHexLarge[16];
extern const char digitsHexSmall[16];
#endif
static const char lpcszNull   [] = "(null)";
#ifndef _WIN32
static const char lpcszNil    [] = "(nil)";
#endif
static const char lpcszNan    [] = "nan";
#ifdef _WIN32
static const char lpcszNanSnan[] = "nan(snan)";
static const char lpcszNanInd [] = "nan(ind)";
#endif
static const char lpcszInf    [] = "inf";

// external functions
#ifdef _MSC_VER
size_t __fastcall _ui32to10a(uint32_t value, char *buffer);
size_t __fastcall _ui64to10a(uint64_t value, char *buffer);
size_t __fastcall _ui64to16a(uint64_t value, char *buffer, BOOL upper);
size_t __fastcall _ui64to8a(uint64_t value, char *buffer);
#endif

#if defined(_MSC_VER) && LONGDOUBLE_IS_DOUBLE
double __cdecl pow10(double x);
#define pow10l pow10
#else
#define pow10l(x) powl(10, x)
#endif

// internal functions
static char *strfmt(char *, const char *, const char *, size_t, ptrdiff_t, int);
static char *intfmt(char *, const char *, intmax_t, unsigned char, size_t, ptrdiff_t, int);
static char *fltfmt(char *, const char *, long_double, size_t, ptrdiff_t, int);

int __cdecl _vsnprintf(char *buffer, size_t count, const char *format, va_list argptr)
{
	char          *dest;
	const char    *end;
	unsigned char overflow;
	char          c;

	/*
	 * C99 says: "If `n' is zero, nothing is written, and `s' may be a null
	 * pointer." (7.19.6.5, 2)  We're forgiving and allow a NULL pointer
	 * even if a count larger than zero was specified.  At least NetBSD's
	 * snprintf(3) does the same, as well as other versions of this file.
	 * (Though some of these versions will write to a non-NULL buffer even
	 * if a count of zero was specified, which violates the standard.)
	 */
	if (!buffer)
		count = 0;

	dest = buffer - 1;
	end = buffer + count;
	if (end < buffer)
		end = (char *)UINTPTR_MAX;

	overflow = 0;
	while (c = *(format++))
	{
		int             flags;
		size_t          width;
		ptrdiff_t       precision;
		int             cflags;
		unsigned char   base;
		long_double     f;
		intmax_t        value;
		char            *s;
		int             i;
#ifndef _WIN32
		char            cbuf[2];
#else
		char            cbuf[3];
		wchar_t         w;
		wchar_t         *ws;
		PANSI_STRING    as;
		PUNICODE_STRING us;
		HANDLE          handle;
#endif

		if (c != '%')
		{
			if (isleadbyte(c))
			{
				OUTCHAR(dest, end, c);
				if (!(c = *(format++)))
					break;
			}
			OUTCHAR(dest, end, c);
			continue;
		}
		c = *(format++);

		// Process flags
		flags = 0;
		for (; ; c = *(format++))
		{
			switch (c)
			{
			case '-':
				flags |= FL_LEFT;
				continue;
			case '+':
				flags |= FL_SIGN;
				continue;
			case ' ':
				flags |= FL_SIGNSP;
				continue;
			case '#':
				flags |= FL_ALTERNATE;
				continue;
			case '0':
				flags |= FL_LEADZERO;
				continue;
			case '\'':
				/* SUSv2 flag (not in C99). */
				flags |= FL_QUOTE;
				continue;
			}
			break;
		}

		// Get field width
		width = 0;
		while (ISDIGIT(c))
		{
			if (width < (size_t)(INT_MAX / 10) || (
				width == (size_t)(INT_MAX / 10) &&
				(unsigned char)c <= (unsigned char)(INT_MAX % 10 + '0')))
			{
				width = width * 10 + (unsigned char)c - '0';
				c = *(format++);
			}
			else
			{
				overflow = 1;
				goto NESTED_BREAK;
			}
		}
		if (c == '*')
		{
			/*
			 * C99 says: "A negative field width argument is
			 * taken as a `-' flag followed by a positive
			 * field width." (7.19.6.1, 5)
			 */
			i = va_arg(argptr, int);
			if (i < 0)
			{
				i = -i;
				flags |= FL_LEFT;
			}
			width = i;
			c = *(format++);
		}

		// Get precision
		if (c != '.')
		{
			precision = -1;
		}
		else
		{
			c = *(format++);
			precision = 0;
			while (ISDIGIT(c))
			{
				if ((size_t)precision < (size_t)(INT_MAX / 10) || (
					(size_t)precision == (size_t)(INT_MAX / 10) &&
					(unsigned char)c <= (unsigned char)(INT_MAX % 10 + '0')))
				{
					precision = (size_t)precision * 10 + (unsigned char)c - '0';
					c = *(format++);
				}
				else
				{
					overflow = 1;
					goto NESTED_BREAK;
				}
			}
			if (c == '*')
			{
				/*
				 * C99 says: "A negative precision argument is
				 * taken as if the precision were omitted."
				 * (7.19.6.1, 5)
				 */
				i = va_arg(argptr, int);
				precision = i >= 0 ? i : -1;
				c = *(format++);
			}
		}

		// Get the conversion qualifier
		switch (c)
		{
		case 'h':
			c = *(format++);
			if (c == 'h')
			{
				/* It's a char. */
				cflags = C_CHAR;
				c = *(format++);
			}
			else
				cflags = C_SHORT;
			break;
		case 'l':
			c = *(format++);
			if (c != 'l')
			{
				cflags = C_LONG;
				break;
			}
#if !INTMAX_IS_LLONG
			/* It's a long long. */
			cflags = C_LLONG;
			c = *(format++);
			break;
#endif
		case 'j':
			cflags = C_INTMAX;
			c = *(format++);
			break;
		case 'z':
			cflags = C_SIZE;
			c = *(format++);
			break;
		case 't':
			cflags = C_PTRDIFF;
			c = *(format++);
			break;
		case 'L':
			cflags = C_LDOUBLE;
			c = *(format++);
			break;
#ifdef _WIN32
		case 'I':
			c = *(format++);
			if (c == '6')
			{
				if (*format == '4')
				{
					cflags = C_LLONG;
					format++;
					c = *(format++);
					break;
				}
			}
			else if (c == '3')
			{
				if (*format == '2')
				{
					cflags = C_LONG;
					format++;
					c = *(format++);
					break;
				}
			}
			cflags = C_PTRDIFF;
			break;
		case 'w':
			cflags = C_WCHAR;
			c = *(format++);
			break;
#endif
		default:
			cflags = C_DEFAULT;
			break;
		}

		switch (c)
		{
		case '\0':
			goto NESTED_BREAK;
		case 'd':
			/* FALLTHROUGH */
		case 'i':
			switch (cflags)
			{
#if !INT_IS_CHAR
			case C_CHAR:
				value = (char)va_arg(argptr, int);
				break;
#endif
#if !INT_IS_SHRT
			case C_SHORT:
				value = (short)va_arg(argptr, int);
				break;
#endif
#if !INT_IS_LONG
			case C_LONG:
				value = va_arg(argptr, long);
				break;
#endif
#if !INT_IS_LLONG
			case C_LLONG:
				value = va_arg(argptr, long_long);
				break;
#endif
#if !INT_IS_INTMAX && !INTMAX_IS_LLONG
			case C_INTMAX:
				value = va_arg(argptr, intmax_t);
				break;
#endif
			case C_SIZE:
				value = va_arg(argptr, size_t);
				break;
			default:
				value = va_arg(argptr, int);
				break;
			}
			dest = intfmt(dest, end, value, 10, width, precision, flags);
			break;
		case 'X':
			flags |= FL_UP;
			/* FALLTHROUGH */
		case 'x':
			base = 16;
			goto PUT_UNSIGNED;
		case 'o':
			base = 8;
			goto PUT_UNSIGNED;
		case 'u':
			base = 10;
		PUT_UNSIGNED:
			flags |= FL_UNSIGNED;
			switch ((unsigned char)cflags)
			{
#if !INT_IS_CHAR
			case C_CHAR:
				value = (unsigned char)va_arg(argptr, int);
				break;
#endif
#if !INT_IS_SHRT
			case C_SHORT:
				value = (unsigned short)va_arg(argptr, int);
				break;
#endif
#if !INT_IS_LONG
			case C_LONG:
				value = va_arg(argptr, unsigned long);
				break;
#endif
#if !INT_IS_LLONG
			case C_LLONG:
				value = va_arg(argptr, unsigned_long_long);
				break;
#endif
#if !INT_IS_INTMAX && !INTMAX_IS_LLONG
			case C_INTMAX:
				value = va_arg(argptr, uintmax_t);
				break;
#endif
			default:
				value = va_arg(argptr, unsigned int);
				break;
			}
			dest = intfmt(dest, end, value, base, width, precision, flags);
			break;
		case 'F':
			flags |= FL_UP;
			/* FALLTHROUGH */
		case 'f':
#if !LONGDOUBLE_IS_DOUBLE
			if (cflags == C_LDOUBLE)
				f = va_arg(argptr, long_double);
			else
#endif
				f = va_arg(argptr, double);
			dest = fltfmt(dest, end, f, width, precision, flags);
			break;
		case 'E':
			flags |= FL_UP;
			/* FALLTHROUGH */
		case 'e':
			flags |= FL_TYPE_E;
#if !LONGDOUBLE_IS_DOUBLE
			if (cflags == C_LDOUBLE)
				f = va_arg(argptr, long_double);
			else
#endif
				f = va_arg(argptr, double);
			dest = fltfmt(dest, end, f, width, precision, flags);
			break;
		case 'G':
			flags |= FL_UP;
			/* FALLTHROUGH */
		case 'g':
			flags |= FL_TYPE_G;
#if !LONGDOUBLE_IS_DOUBLE
			if (cflags == C_LDOUBLE)
				f = va_arg(argptr, long_double);
			else
#endif
				f = va_arg(argptr, double);
			dest = fltfmt(dest, end, f, width, precision, flags);
			break;
		case 'A':
			flags |= FL_UP;
			/* FALLTHROUGH */
		case 'a':
			flags |= FL_TYPE_A;
#if !LONGDOUBLE_IS_DOUBLE
			if (cflags == C_LDOUBLE)
				f = va_arg(argptr, long_double);
			else
#endif
				f = va_arg(argptr, double);
			dest = fltfmt(dest, end, f, width, precision, flags);
			break;
		case 'c':
#ifdef _WIN32
			if (cflags == C_LONG || cflags == C_WCHAR)
				goto PUT_WCHAR;
		PUT_CHAR:
#endif
			cbuf[0] = va_arg(argptr, int);
			cbuf[1] = '\0';
			dest = strfmt(dest, end, cbuf, width, precision, flags);
			break;
#ifdef _WIN32
		case 'C':
			if (cflags == C_SHORT || cflags == C_CHAR)
				goto PUT_CHAR;
		PUT_WCHAR:
			w = va_arg(argptr, int);
			i = WideCharToMultiByte(CP_THREAD_ACP, 0, &w, 1, cbuf, 2, NULL, NULL);
			if (!i)
				break;
			cbuf[i] = '\0';
			dest = strfmt(dest, end, cbuf, width, precision, flags);
			break;
#endif
		case 's':
#ifdef _WIN32
			if (cflags == C_LONG || cflags == C_WCHAR)
				goto PUT_WSTR;
		PUT_STR:
#endif
			dest = strfmt(dest, end, va_arg(argptr, char *), width, precision, flags);
			break;
#ifdef _WIN32
		case 'S':
			if (cflags == C_SHORT || cflags == C_CHAR)
				goto PUT_STR;
		PUT_WSTR:
			s = NULL;
			i = 0;
			ws = va_arg(argptr, wchar_t *);
			if (ws)
				if (i = WideCharToMultiByte(CP_THREAD_ACP, 0, ws, -1, NULL, 0, NULL, NULL))
					if (s = (char *)HeapAlloc(handle = GetProcessHeap(), 0, ++i))
						i = WideCharToMultiByte(CP_THREAD_ACP, 0, ws, -1, s, i, NULL, NULL);
			dest = strfmt(dest, end, i ? s : NULL, width, precision, flags);
			if (s)
				HeapFree(handle, 0, s);
			break;
#endif
		case 'p':
			s = va_arg(argptr, void *);
#ifndef _WIN32
			/*
			 * C99 says: "The value of the pointer is
			 * converted to a sequence of printing
			 * characters, in an implementation-defined
			 * manner." (C99: 7.19.6.1, 8)
			 */
			if (!s)
			{
				/*
				 * We use the glibc format.  BSD prints
				 * "0x0", SysV "0".
				 */
				dest = strfmt(dest, end, lpcszNil, width, -1, flags);
			}
			else
			{
				/*
				 * We use the BSD/glibc format.  SysV
				 * omits the "0x" prefix (which we emit
				 * using the FL_ALTERNATE flag).
				 */
				flags |= FL_UNSIGNED | FL_UP | FL_ALTERNATE;
#else
				flags |= FL_UNSIGNED | FL_UP;
#endif
				dest = intfmt(dest, end, (uintptr_t)s, 16, width, sizeof(void *) * 2, flags);
#ifndef _WIN32
			}
#endif
			break;
		case 'n':
			switch ((unsigned char)cflags)
			{
#if !INT_IS_CHAR
			case C_CHAR:
				*va_arg(argptr, char *) = (char)(dest + 1 - buffer);
				break;
#endif
#if !INT_IS_SHRT
			case C_SHORT:
				*va_arg(argptr, short *) = (short)(dest + 1 - buffer);
				break;
#endif
#if !INT_IS_LONG
			case C_LONG:
				*va_arg(argptr, long *) = (long)(dest + 1 - buffer);
				break;
#endif
#if !INT_IS_LLONG
			case C_LLONG:
				*va_arg(argptr, long_long *) = (long_long)(dest + 1 - buffer);
				break;
#endif
#if !INT_IS_INTMAX && !INTMAX_IS_LLONG
			case C_INTMAX:
				*va_arg(argptr, intmax_t *) = dest + 1 - buffer;
				break;
#endif
			default:
				*va_arg(argptr, int *) = (int)(dest + 1 - buffer);
				break;
			}
			break;
#ifdef _WIN32
		case 'Z':
			switch (cflags)
			{
			case C_LONG:
			case C_WCHAR:
				s = NULL;
				i = 0;
				us = va_arg(argptr, PUNICODE_STRING);
				if (us && us->Buffer)
					if (s = (char *)HeapAlloc(handle = GetProcessHeap(), 0, i = ((unsigned int)us->Length + 1) * 2))
						i = WideCharToMultiByte(CP_THREAD_ACP, 0, us->Buffer, -1, s, i, NULL, NULL);
				dest = strfmt(dest, end, i ? s : NULL, width, precision, flags);
				if (s)
					HeapFree(handle, 0, s);
				break;
			default:
				as = va_arg(argptr, PANSI_STRING);
				dest = strfmt(dest, end, as ? as->Buffer : NULL, width, precision, flags);
				break;
			}
			break;
#endif
		case '%':
			/* Print a "%" character verbatim. */
			OUTCHAR(dest, end, c);
			break;
		default:
			/* Skip other characters. */
			if (isleadbyte(c))
				if (!*(format++))
					goto NESTED_BREAK;
			break;
		}
	}
NESTED_BREAK:
	if (++dest < end)
		*dest = '\0';
	else if (count)
		*(char *)(end - 1) = '\0';

	if (!overflow && (size_t)(dest - buffer) < INT_MAX)
	{
		return dest - buffer;
	}
	else
	{
		errno = overflow ? EOVERFLOW : ERANGE;
		return -1;
	}
}

#undef strnlen
#define strnlen inline_strnlen

static inline size_t inline_strnlen(const char *str, size_t numberOfElements)
{
	char *p = (char *)memchr(str, '\0', numberOfElements);
	return p ? p - str : numberOfElements;
}

static char *strfmt(char *dest, const char *end, const char *value, size_t width, ptrdiff_t precision, int flags)
{
	ptrdiff_t padlen;	/* Amount to pad. */

	/* We're forgiving. */
	if (!value)
		value = lpcszNull;

	if ((padlen = width - (precision < 0 ? strlen(value) : strnlen(value, precision))) < 0)
		padlen = 0;

	/* Left justify. */
	if (flags & FL_LEFT)
		padlen = -padlen;

	/* Leading spaces. */
	if (padlen > 0)
	{
		do
		{
			OUTCHAR(dest, end, ' ');
		} while (--padlen);
	}

	while (*value && (precision < 0 || precision-- > 0))
	{
		OUTCHAR(dest, end, *value);
		value++;
	}

	/* Trailing spaces. */
	if (padlen < 0)
	{
		do
		{
			OUTCHAR(dest, end, ' ');
		} while (++padlen);
	}

	return dest;
}

static inline size_t intcvt(uintmax_t value, char *buffer, unsigned char base, int flags)
{
#if defined(_MSC_VER) && UINTMAX_MAX == UINT64_MAX
	return
		base == 10 ? _ui64to10a(value, buffer) :
		base == 16 ? _ui64to16a(value, buffer, flags & FL_UP) :
		_ui64to8a(value, buffer);
#else
	char *dest;
	char *p1, *p2;
	char c1, c2;

	dest = buffer;
	if (base == 10)
	{
		do
		{
			*(dest++) = (char)(value % 10) + '0';
		} while (value /= 10);
	}
	else if (base == 16)
	{
		const char *digits;

		digits = (flags & FL_UP) ? digitsHexLarge : digitsHexSmall;
		do
		{
			*(dest++) = digits[(size_t)value & 0x0F];
		} while (value >>= 4);
	}
	else// if (base == 8)
	{
		do
		{
			*(dest++) = ((char)value & 0x07) + '0';
		} while (value >>= 3);
	}
	p1 = buffer;
	p2 = dest - 1;
	while (p1 < p2)
	{
		c1 = *p1;
		c2 = *p2;
		*(p1++) = c2;
		*(p2--) = c1;
	}
	return dest - buffer;
#endif
}

#define GETNUMSEP(digits) \
	((size_t)(digits) ? ((size_t)(digits) - 1) / 3 : 0)

#define PRINTSEP(dest, end) \
	OUTCHAR(dest, end, ',')

static char *intfmt(char *dest, const char *end, intmax_t value, unsigned char base, size_t width, ptrdiff_t precision, int flags)
{
	char          icvtbuf[ALIGN(UINTMAX_OCT_DIG + 1, 16)];
	uintmax_t     uvalue;
	char          sign;
	char          hexprefix;
	ptrdiff_t     spadlen;	/* Amount to space pad. */
	ptrdiff_t     zpadlen;	/* Amount to zero pad. */
	size_t        pos;
	size_t        separators;
	unsigned char noprecision;

	sign = '\0';
	if (flags & FL_UNSIGNED)
	{
		uvalue = value;
	}
	else
	{
		uvalue = (value >= 0) ? value : -value;
		if (value < 0)
			sign = '-';
		else if (flags & FL_SIGN)
			/* Do a sign. */
			sign = '+';
		else if (flags & FL_SIGNSP)
			sign = ' ';
	}

	pos = intcvt(uvalue, icvtbuf, base, flags);

	hexprefix = '\0';
	noprecision = (precision < 0);
	if ((flags & FL_ALTERNATE) && uvalue)
	{
		/*
		 * C99 says: "The result is converted to an `alternative form'.
		 * For `o' conversion, it increases the precision, if and only
		 * if necessary, to force the first digit of the result to be a
		 * zero (if the value and precision are both 0, a single 0 is
		 * printed).  For `x' (or `X') conversion, a nonzero result has
		 * `0x' (or `0X') prefixed to it." (7.19.6.1, 6)
		 */
		switch (base)
		{
		case 8:
			if (precision <= (ptrdiff_t)pos)
				precision = pos + 1;
			break;
		case 16:
			hexprefix = (flags & FL_UP) ? 'X' : 'x';
			break;
		}
	}

	/* Get the number of group separators we'll print. */
	separators = (flags & FL_QUOTE) ? GETNUMSEP(pos) : 0;

	zpadlen = precision - pos - separators;
	spadlen =
		width                               /* Minimum field width. */
		- separators                        /* Number of separators. */
		- max(precision, (ptrdiff_t)pos)    /* Number of integer digits. */
		- (sign ? 1 : 0)                    /* Will we print a sign? */
		- (hexprefix ? 2 : 0);              /* Will we print a prefix? */

	if (zpadlen < 0)
		zpadlen = 0;
	if (spadlen < 0)
		spadlen = 0;

	/*
	 * C99 says: "If the `0' and `-' flags both appear, the `0' flag is
	 * ignored.  For `d', `i', `o', `u', `x', and `X' conversions, if a
	 * precision is specified, the `0' flag is ignored." (7.19.6.1, 6)
	 */
	if (flags & FL_LEFT)
	{
		/* Left justify. */
		spadlen = -spadlen;
	}
	else if ((flags & FL_LEADZERO) && noprecision)
	{
		zpadlen += spadlen;
		spadlen = 0;
	}

	/* Leading spaces. */
	if (spadlen > 0)
	{
		do
		{
			OUTCHAR(dest, end, ' ');
		} while (--spadlen);
	}

	/* Sign. */
	if (sign)
		OUTCHAR(dest, end, sign);

	/* A "0x" or "0X" prefix. */
	if (hexprefix)
	{
		OUTCHAR(dest, end, '0');
		OUTCHAR(dest, end, hexprefix);
	}

	/* Leading zeros. */
	if (zpadlen)
	{
		do
		{
			OUTCHAR(dest, end, '0');
		} while (--zpadlen);
	}

	/* The actual digits. */
	if (pos)
	{
		size_t i;

		OUTCHAR(dest, end, *icvtbuf);
		for (i = 1; i < pos; i++)
		{
			if (separators && !((pos - i) % 3))
				PRINTSEP(dest, end);
			OUTCHAR(dest, end, icvtbuf[i]);
		}
	}

	/* Trailing spaces. */
	if (spadlen < 0)
	{
		do
		{
			OUTCHAR(dest, end, ' ');
		} while (++spadlen);
	}

	return dest;
}

#define ECVTBUF(value, ndigits, decpt, cvtbuf) \
	fltcvt(value, ndigits, decpt, cvtbuf, 1)

#define FCVTBUF(value, ndigits, decpt, cvtbuf) \
	fltcvt(value, ndigits, decpt, cvtbuf, 0)

static size_t fltcvt(long_double value, size_t ndigits, ptrdiff_t *decpt, char cvtbuf[CVTBUFSIZE], unsigned char eflag)
{
#if !LONGDOUBLE_IS_DOUBLE && (!LONGDOUBLE_IS_X86_EXTENDED || INTMAX_IS_LLONG)
	long_double intpart, fracpart;
	char        *p1, *p2;
	ptrdiff_t   r2;
	int         i;

#ifdef _DEBUG
	assert(!signbitl(value));
	assert(!isnanl(value));
	assert(!isinfl(value));
	assert((ptrdiff_t)ndigits >= 0);
#endif

	frexp(value, &i);
	*decpt = (ptrdiff_t)(((i - 1) * (int64_t)(0x0000000100000000 * M_LOG10_2)) >> 32) + 1;
	r2 = 0;
	p1 = cvtbuf;
	value = modfl(value, &intpart);
	if (intpart)
	{
		char c1, c2;

		do
		{
			fracpart = modfl(intpart / 10, &intpart);
			*(p1++) = (char)((fracpart + .03) * 10) + '0';
		} while (intpart && p1 < cvtbuf + CVTBUFSIZE - 1);
		r2 = (p1--) - cvtbuf;
		p2 = cvtbuf;
		while (p1 > p2)
		{
			c1 = *p1;
			c2 = *p2;
			*(p1--) = c2;
			*(p2++) = c1;
		}
		p1 = cvtbuf + r2;
	}
	else
	{
		if (value)
		{
			while ((fracpart = value * 10) < 1)
			{
				value = fracpart;
				r2--;
			}
		}
	}
	p2 = cvtbuf + ndigits;
	if (!eflag)
		p2 += r2;
	if (p2 >= cvtbuf)
	{
		char *p3;

		p3 = min(p2, cvtbuf + CVTBUFSIZE - 1);
		while (p1 <= p3)
		{
			value *= 10;
			value = modfl(value, &fracpart);
			*(p1++) = (char)fracpart + '0';
		}
		if (p2 < cvtbuf + CVTBUFSIZE)
		{
			p1 = p2;
			*p2 += 5;
			while (*p2 > '9')
			{
				*p2 = '0';
				if (p2 > cvtbuf)
				{
					++(*(--p2));
				}
				else
				{
					*p2 = '1';
					(*decpt)++;
					if (!eflag)
					{
						if (p1 > cvtbuf)
							*p1 = '0';
						p1++;
					}
				}
			}
		}
		else
		{
			p1 = cvtbuf + CVTBUFSIZE - 1;
		}
	}
	else
	{
		p1 = cvtbuf;
	}
	*p1 = '\0';
	return p1 - cvtbuf;
#else
#if LONGDOUBLE_IS_DOUBLE
	#define EXP10(exp2) (int32_t)(((int32_t)(exp2) * (int32_t)(0x00200000 * M_LOG10_2)) >> 21)
#else
	#define EXP10(exp2) (int32_t)(((int32_t)(exp2) * (int64_t)(0x0000000100000000 * M_LOG10_2)) >> 32)
#endif

	int32_t   e;
	size_t    length;
	ptrdiff_t padding;

	if (value)
	{
		long_double intpart, fracpart;
		uintmax_t   decimal;
		int32_t     i, shift;
		bool        round;

		fracpart = modfl(value, &intpart);
		e = EXP10(((LPLONGDOUBLE)&value)->exponent - LDBL_EXP_BIAS);
		if (value >= LDBL_MIN)
		{
			i = (LDBL_DECIMAL_DIG - 1) - e;
		}
		else
		{
			value *= CONCAT(1e, LDBL_MAX_10_EXP);
			value *= CONCAT(1e, LDBL_DECIMAL_DIG) / 10;
			i = EXP10(LDBL_EXP_BIAS - (int32_t)((LPLONGDOUBLE)&value)->exponent) + (LDBL_DECIMAL_DIG - 1) + 1;
			e -= i;
		}
		if (round = (eflag || fracpart && e < LDBL_DECIMAL_DIG - 1) && ndigits < (uint32_t)(LDBL_DECIMAL_DIG - (!eflag ? e : 0)))
			i -= LDBL_DECIMAL_DIG - ndigits - (!eflag ? e + 1 : 0);
		value *= pow10l(i);
		if (round)
		{
			e -= EXP10(((LPLONGDOUBLE)&value)->exponent - LDBL_EXP_BIAS);
			modfl(value + .5, &value);
			e += EXP10(((LPLONGDOUBLE)&value)->exponent - LDBL_EXP_BIAS);
		}
		decimal = ((LPLONGDOUBLE)&value)->mantissa | ((uintmax_t)1 << (LDBL_MANT_DIG - 1));
		if ((shift = (int32_t)((LPLONGDOUBLE)&value)->exponent - (LDBL_EXP_BIAS + LDBL_MANT_DIG - 1)) < 0)
			decimal >>= -shift;
		else
			decimal <<= shift;
		if (decimal)
			while (!(decimal % 10))
				decimal /= 10;
#if defined(_MSC_VER) && LONGDOUBLE_IS_DOUBLE
		length = _ui64to10a(decimal, cvtbuf);
#else
		length = intcvt(decimal, cvtbuf, 10, 0);
#endif
	}
	else
	{
		e = 0;
		cvtbuf[0] = '0';
		cvtbuf[1] = '\0';
		length = 1;
	}
	if ((padding = (!eflag ? e + 1 : 0) + ndigits - length) > 0)
	{
		if ((size_t)padding > CVTBUFSIZE - 1 - length)
			padding = CVTBUFSIZE - 1 - length;
		memset(cvtbuf + length, '0', padding);
		*(cvtbuf + (length += padding)) = '\0';
	}
	*decpt = e + 1;
	return length;

	#undef EXP10
#endif
}

static inline size_t hexcvt(long_double value, size_t precision, char cvtbuf[CVTBUFSIZE], size_t *elen, char ecvtbuf[EXPBUFSIZE], int flags)
{
	uintmax_t     mantissa;
	int32_t       exponent;
	size_t        i;
	const char    *digits;
	char          *p1, *p2;
#ifndef _MSC_VER
	char          c1, c2;
#endif

#ifdef _DEBUG
	assert(!signbitl(value));
	assert(!isnanl(value));
	assert(!isinfl(value));
	assert((ptrdiff_t)precision >= 0);
#endif

	mantissa = ((LPLONGDOUBLE)&value)->mantissa;
	exponent = (int32_t)((LPLONGDOUBLE)&value)->exponent - LDBL_EXP_BIAS;
	if (precision > MANTISSA_HEX_DIG)
		precision = MANTISSA_HEX_DIG;
	if (i = MANTISSA_HEX_DIG - precision)
	{
		mantissa >>= i * 4 - 4;
		mantissa += 7;
		mantissa >>= 4;
	}
	digits = (flags & FL_UP) ? digitsHexLarge : digitsHexSmall;
	if (precision)
	{
		p1 = cvtbuf + 1;
		p2 = p1 + precision;
		do
		{
			*(--p2) = digits[(size_t)mantissa & 0x0F];
			mantissa >>= 4;
		} while (p2 != p1);
	}
	*cvtbuf = digits[((size_t)mantissa + 1) & 0x0F];
	ecvtbuf[0] = (flags & FL_UP) ? 'P' : 'p';
	if (exponent >= 0)
	{
		ecvtbuf[1] = '+';
	}
	else
	{
		exponent = -exponent;
		ecvtbuf[1] = '-';
	}
#ifdef _MSC_VER
	*elen = _ui32to10a(exponent, ecvtbuf + 2) + 2;
#else
	p2 = p1 = ecvtbuf + 2;
	do
	{
		*(p2++) = (char)((uint32_t)exponent % 10) + '0';
	} while (exponent = (uint32_t)exponent / 10);
	*elen = (p2--) - ecvtbuf;
	while (p1 < p2)
	{
		c1 = *p1;
		c2 = *p2;
		*(p1++) = c2;
		*(p2--) = c1;
	}
#endif
	return precision + 1;
}

static char *fltfmt(char *dest, const char *end, long_double value, size_t width, ptrdiff_t precision, int flags)
{
	char       cvtbuf[ALIGN(CVTBUFSIZE, 16)];
	char       ecvtbuf[ALIGN(EXPBUFSIZE, 16)];	/* "e-12" */
	char       sign;
	char       hexprefix;
	size_t     cvtlen;
	ptrdiff_t  decpt;
	size_t     ilen;
	size_t     flen;
	size_t     elen;
	size_t     tailfraczeros;
	size_t     separators;
	size_t     emitpoint;
	ptrdiff_t  padlen;
	size_t     i;
	char       *p;
	const char *infnan;
	char       c;

	// Left align means no zero padding
	if (flags & FL_LEFT)
		flags &= ~FL_LEADZERO;

	// Determine padding and sign char
	if (signbitl(value))
		sign = '-';
	else if (flags & (FL_SIGN | FL_SIGNSP))
		sign = (flags & FL_SIGN) ? '+' : ' ';
	else
		sign = '\0';

	if (isnanl(value))
		goto NaN;
	if (isinfl(value))
		goto INF;

	if (signbitl(value))
		value = -value;

	// Compute the precision value
	if (precision < 0)
		// Default precision: 6
		precision = !(flags & FL_TYPE_A) ? 6 : MANTISSA_HEX_DIG;
	else if (!precision && (flags & FL_TYPE_G))
		// ANSI specified
		precision = 1;

	// Convert floating point number to text
	if (!(flags & FL_TYPE_A))
	{
		if (flags & FL_TYPE_G)
		{
			ECVTBUF(value, precision, &decpt, cvtbuf);
			if (decpt <= -4 || decpt > precision)
			{
				flags |= FL_TYPE_E;
				precision -= 1;
			}
			else
			{
				precision -= decpt;
			}
		}
		if (flags & FL_TYPE_E)
		{
			int32_t  exponent;
#ifndef _MSC_VER
			char     *p1, *p2;
			char     c1, c2;
#endif

			cvtlen = ECVTBUF(value, precision + 1, &decpt, cvtbuf);

			exponent = !decpt ? !value ? 0 : -1 : decpt - 1;
			decpt = 1;

			ecvtbuf[0] = (flags & FL_UP) ? 'E' : 'e';
			if (exponent >= 0)
			{
				ecvtbuf[1] = '+';
			}
			else
			{
				exponent = -exponent;
				ecvtbuf[1] = '-';
			}
#ifdef _MSC_VER
			elen = _ui32to10a(exponent, ecvtbuf + 2) + 2;
			if (elen == 3)
			{
				ecvtbuf[3] = ecvtbuf[2];
				ecvtbuf[2] = '0';
				elen++;
			}
#else
			elen = 2;
			do
			{
				ecvtbuf[elen++] = (char)((uint32_t)exponent % 10) + '0';
			} while (exponent = (uint32_t)exponent / 10);

			/*
			 * C99 says: "The exponent always contains at least two digits,
			 * and only as many more digits as necessary to represent the
			 * exponent." (7.19.6.1, 8)
			 */
			if (elen == 3)
				ecvtbuf[elen++] = '0';

			p1 = ecvtbuf + 2;
			p2 = ecvtbuf + elen - 1;
			while (p1 < p2)
			{
				c1 = *p1;
				c2 = *p2;
				*(p1++) = c2;
				*(p2--) = c1;
			}
#endif
		}
		else
		{
			elen = 0;
			cvtlen = FCVTBUF(value, precision, &decpt, cvtbuf);
		}
		if ((flags & FL_TYPE_G) && !(flags & FL_ALTERNATE))
		{
			char *end;

			p = end = cvtbuf + cvtlen;
			while (*(--p) == '0' && p != cvtbuf);
			if (++p != end)
			{
				ptrdiff_t diff;

				diff = end - p;
				precision -= min(diff, precision);
				cvtlen -= diff;
			}
		}
		ilen = max(decpt, 1);
		flen = cvtlen - min(decpt, (ptrdiff_t)cvtlen);
		hexprefix = '\0';
	}
	else
	{
		cvtlen = hexcvt(value, precision, cvtbuf, &elen, ecvtbuf, flags);
		ilen = decpt = 1;
		flen = cvtlen - 1;
		hexprefix = (flags & FL_UP) ? 'X' : 'x';
	}
	tailfraczeros = precision - min(flen, (size_t)precision);

	/*
	 * Print a decimal point if either the fractional part is non-zero
	 * and/or the "#" flag was specified.
	 */
	emitpoint = precision || (flags & FL_ALTERNATE);

	/* Get the number of group separators we'll print. */
	separators = ((flags & FL_QUOTE) && !(flags & FL_TYPE_A)) ? GETNUMSEP(ilen) : 0;

	padlen =
		width                   /* Minimum field width. */
		- ilen                  /* Number of integer digits. */
		- elen                  /* Number of exponent characters. */
		- precision             /* Number of fractional digits. */
		- separators            /* Number of group separators. */
		- emitpoint             /* Will we print a decimal point? */
		- (sign ? 1 : 0)        /* Will we print a sign character? */
		- (hexprefix ? 2 : 0);  /* Will we print a prefix? */

	if (padlen < 0)
		padlen = 0;

	/*
	 * C99 says: "If the `0' and `-' flags both appear, the `0' flag is
	 * ignored." (7.19.6.1, 6)
	 */
	if (flags & FL_LEFT)
	{
		/* Left justifty. */
		padlen = -padlen;
	}
	else if ((flags & FL_LEADZERO) && padlen)
	{
		/* Sign. */
		if (sign)
		{
			OUTCHAR(dest, end, sign);
			sign = '\0';
		}

		/* A "0x" or "0X" prefix. */
		if (hexprefix)
		{
			OUTCHAR(dest, end, '0');
			OUTCHAR(dest, end, hexprefix);
			hexprefix = '\0';
		}

		/* Leading zeros. */
		do
		{
			OUTCHAR(dest, end, '0');
		} while (--padlen);
	}

	/* Leading spaces. */
	if (padlen > 0)
	{
		do
		{
			OUTCHAR(dest, end, ' ');
		} while (--padlen);
	}

	/* Sign. */
	if (sign)
		OUTCHAR(dest, end, sign);

	/* A "0x" or "0X" prefix. */
	if (hexprefix)
	{
		OUTCHAR(dest, end, '0');
		OUTCHAR(dest, end, hexprefix);
	}

	/* Integer part. */
	if (decpt > 0)
	{
		OUTCHAR(dest, end, *cvtbuf);
		for (i = 1; i < (size_t)decpt; i++)
		{
			if (separators && !(((size_t)decpt - i) % 3))
				PRINTSEP(dest, end);
			OUTCHAR(dest, end, cvtbuf[i]);
		}
	}
	else
	{
		OUTCHAR(dest, end, '0');
	}

	/* Decimal point. */
	if (emitpoint)
		OUTCHAR(dest, end, '.');

	/* The remaining fractional part. */
	if (decpt < 0)
	{
		do
		{
			OUTCHAR(dest, end, '0');
		} while (++decpt);
	}
	for (i = decpt; i < cvtlen; i++)
		OUTCHAR(dest, end, cvtbuf[i]);

	/* Following fractional part zeros. */
	if (tailfraczeros)
	{
		do
		{
			OUTCHAR(dest, end, '0');
		} while (--tailfraczeros);
	}

	/* Exponent. */
	for (i = 0; i < elen; i++)
		OUTCHAR(dest, end, ecvtbuf[i]);

	/* Trailing spaces. */
	if (padlen < 0)
	{
		do
		{
			OUTCHAR(dest, end, ' ');
		} while (++padlen);
	}

	return dest;

NaN:
#ifndef _WIN32
	infnan = lpcszNan;
#elif LONGDOUBLE_IS_DOUBLE && DOUBLE_IS_IEEE754
	if (!(*(uint64_t *)&value & 0x0008000000000000))
		infnan = lpcszNanSnan;
	else if (*(uint64_t *)&value != 0xFFF8000000000000)
		infnan = lpcszNan;
	else
		infnan = lpcszNanInd;
#else
	if (!(((LPLONGDOUBLE)&value)->mantissa & ((uintmax_t)1 << (LDBL_MANT_BIT - 1))))
		infnan = lpcszNanSnan;
	else if (((LPLONGDOUBLE)&value)->mantissa != ((uintmax_t)1 << (LDBL_MANT_BIT - 1)) || !signbitl(value))
		infnan = lpcszNan;
	else
		infnan = lpcszNanInd;
#endif
	goto INF_NaN;

INF:
	infnan = lpcszInf;

INF_NaN:
	p = cvtbuf;
	if (sign)
		*(p++) = sign;
	if (flags & FL_UP)
		while (c = *(infnan++))
			*(p++) = (c >= 'a' && c <= 'z') ? c - ('a' - 'A') : c;
	else
		while (c = *(infnan++))
			*(p++) = c;
	return strfmt(dest, end, cvtbuf, width, p - cvtbuf, flags);
}
