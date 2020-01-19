#define SMALL 0

#ifndef _M_IX86
#include <longdouble.h>
#include <float.h>
#include <math.h>
#include <errno.h>

#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif
#endif

#define LSW(x) ((uint32_t *)&(x))[0]
#define MSW(x) ((uint32_t *)&(x))[1]

EXTERN_C double __cdecl exp(double x)
{
	static const longdouble c0 = { 0xB8AA000000000000, 0x3FFF };	// 1.44268798828125
	static const longdouble c1 = { 0xECA5705FC2EEFA20, 0x3FED };	// log2(e) - 1.44268798828125

	uint32_t   msw;
	uint16_t   cw;
	longdouble z, i, f, xi, xf;

	msw = MSW(x) & 0x7FFFFFFF;
	if (msw < 0x3BC00000)		// |x| < 0x1p-67 (6.7762635780344027e-21) ?
		return 1;
	if (msw > 0x40D00000)		// |x| > 0x1p+15 (16384.0) ?
	{
		if (msw >= 0x7FF00000)	// Is NaN or Inf ?
			if (msw == 0x7FF00000 && !LSW(x) && (int32_t)MSW(x) < 0)
				return 0;		// -Inf
			else
				return x;		// +Inf or NaN
		x = (int32_t)MSW(x) >= 0 ? 16384 : -16384;
	}
#ifdef __cplusplus
	cw = longdouble::fstcw();
	longdouble::fldcw((cw & ~CW_RC_MASK) | CW_PC_64 | CW_EM_UNDERFLOW | CW_EM_OVERFLOW);
	z = x;
	i = (z * longdouble::fldl2e()).frndint();
	xi = z.frndint();
	f = c0 * xi - i;
	xf = z - xi;
	f = f + c0 * xf;
	f = f + c1 * z;
	z = f.f2xm1();
	++z;
	x = (double)z.fscale(i);
	longdouble::fclex();
	longdouble::fldcw(cw);
#else
	cw = _fstcw();
	_fldcw((cw & ~CW_RC_MASK) | CW_PC_64 | CW_EM_UNDERFLOW | CW_EM_OVERFLOW);
	z = _fld_r8(x);
	i = _frndint(_fmul(z, _fldl2e()));
	xi = _frndint(z);
	f = _fsub(_fmul(_fld_r8(c0), xi), i);
	xf = _fsub(z, xi);
	f = _fadd(f, _fmul(_fld_r8(c0), xf));
	f = _fadd(f, _fmul(c1, z));
	z = _f2xm1(f);
	z = _finc(z);
	z = _fscale(z, i);
	x = _fst_r8(z);
	_fclex();
	_fldcw(cw);
#endif
	if (fabs(x) > DBL_MAX)
		errno = ERANGE;
	return x;
}
#elif !SMALL
#include <errno.h>

#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif
#endif

EXTERN_C double __cdecl _CIexp(/*st0 x*/);

EXTERN_C __declspec(naked) double __cdecl exp(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]         /* Load real from stack */
		jmp     _CIexp
	}
}

#define CW_EM_MASK                        0x003F
#define CW_EM_INVALID                     0x0001
#define CW_EM_DENORMAL                    0x0002
#define CW_EM_ZERODIVIDE                  0x0004
#define CW_EM_OVERFLOW                    0x0008
#define CW_EM_UNDERFLOW                   0x0010
#define CW_EM_INEXACT                     0x0020
#define CW_EM_DEFAULT                     0x003F
#define CW_PC_MASK                        0x0300
#define CW_PC_24                          0x0100
#define CW_PC_53                          0x0200
#define CW_PC_64                          0x0300
#define CW_PC_DEFAULT                     CW_PC_53
#define CW_RC_MASK                        0x0C00
#define CW_RC_NEAR                        0x0000
#define CW_RC_DOWN                        0x0400
#define CW_RC_UP                          0x0800
#define CW_RC_CHOP                        0x0C00
#define CW_RC_DEFAULT                     CW_RC_NEAR
#define CW_IC_MASK                        0x1000
#define CW_IC_PROJECTIVE                  0x0000
#define CW_IC_AFFINE                      0x1000
#define CW_IC_DEFAULT                     CW_IC_PROJECTIVE
#define CW_DN_MASK                        0x8040
#define CW_DN_SAVE                        0x0000
#define CW_DN_FLUSH_OPERANDS_SAVE_RESULTS 0x0040
#define CW_DN_SAVE_OPERANDS_FLUSH_RESULTS 0x8000
#define CW_DN_FLUSH                       0x8040
#define CW_DN_DEFAULT                     CW_DN_FLUSH_OPERANDS_SAVE_RESULTS

EXTERN_C const double fpconst_one;
#define _one fpconst_one

EXTERN_C __declspec(naked) double __cdecl _CIexp(/*st0 x*/)
{
	static const unsigned int csat = 0x8000;
	static const double       c0   = 1.44268798828125;
	static const unsigned int c1[] = { 0xC2EEFA20, 0xECA5705F, 0x00003FED };	// log2(e) - 1.44268798828125

#ifdef _DEBUG
	errno_t * __cdecl _errno();
	#define set_errno(x) \
		__asm   fstp    qword ptr [esp]         /* Save x */ \
		__asm   call    _errno                  /* Get C errno variable pointer */ \
		__asm   mov     dword ptr [eax], x      /* Set error number */ \
		__asm   fld     qword ptr [esp]         /* Load x */
#else
	extern errno_t _terrno;
	#define set_errno(x) \
		__asm   mov     dword ptr [_terrno], x  /* Set error number */
#endif

	__asm
	{
		sub     esp, 8
		fstp    qword ptr [esp]             /* Cast to qword */
		fld     qword ptr [esp]
		mov     eax, dword ptr [esp + 4]
		mov     ecx, dword ptr [esp]
		mov     edx, eax
		and     eax, 7FFFFFFFH
		cmp     eax, 40D00000H              /* |x| > 0x1p+15 (16384.0) ? */
		ja      L1
		cmp     eax, 3BC00000H              /* |x| < 0x1p-67 (6.7762635780344027e-21) ? */
		jae     L2

		/* Argument's exponent below -67, result rounds to 1.  */
		fstp    st(0)
		fld1                                /* Set result to 1.  */
		jmp     L3

		align   16
	L1:
		/* Overflow, underflow or infinity or NaN as argument.  */
		sub     eax, 7FF00000H              /* Is NaN or +-Inf?  */
		jae     L4

		/* Overflow or underflow; saturate.  */
		fstp    st(0)
		fild    dword ptr [csat]
		test    edx, edx
		jns     L2
		fchs
	L2:
		/* Set round-to-nearest temporarily.  */
		fstcw   word ptr [esp + 4]
		mov     ax, word ptr [esp + 4]
		and     ax, not CW_RC_MASK
		or      ax, CW_PC_64        or \
		            CW_EM_UNDERFLOW or \
		            CW_EM_OVERFLOW
		mov     word ptr [esp], ax
		fldcw   word ptr [esp]
		fldl2e                              /* 1 log2(e)          */
		fmul    st(0), st(1)                /* 1 x * log2(e)      */
		frndint                             /* 1 i                */
		fld     st(1)                       /* 2 x                */
		frndint                             /* 2 xi               */
		fld     qword ptr [c0]              /* 3 c0               */
		fld     st(1)                       /* 4 xi               */
		fmul    st(0), st(1)                /* 4 c0 * xi          */
		fsub    st(0), st(3)                /* 4 f = c0 xi  - i   */
		fxch    st(2)
		fsubr   st(0), st(4)                /* 4 xf = x - xi      */
		fmul                                /* 3 c0 xf            */
		fadd                                /* 2 f = f + c0 xf    */
		fld     tbyte ptr [c1]              /* 3                  */
		fmulp   st(3), st(0)                /* 2 c1 * x           */
		faddp   st(2), st(0)                /* 1 f = f + c1 * x   */
		fxch
		f2xm1                               /* 1 2^(fract(x * log2(e))) - 1 */
		fadd    qword ptr [_one]            /* 1 2^(fract(x * log2(e))) */
		fscale                              /* 1 scale factor is st(1); e^x */
		fstp    st(1)                       /* 0                  */
		fclex                               /* Clear exceptions */
		fldcw   word ptr [esp + 4]
		fstp    qword ptr [esp]             /* Cast to qword */
		fld     qword ptr [esp]
		mov     eax, dword ptr [esp + 4]
		and     eax, 7FFFFFFFH              /* Is +-Inf?  */
		cmp     eax, 7FF00000H
		jne     L3                          /* Is not +-Inf, jump.    */
		set_errno(ERANGE)                   /* Set range error (ERANGE) */
	L3:
		add     esp, 8
		ret

		align   16
	L4:
		or      eax, ecx                    /* Is +-Inf?  */
		jnz     L5                          /* Is not +-Inf(+-NaN), jump.    */
		test    edx, edx                    /* Test sign.  */
		jns     L5                          /* If positive, jump.  */
		fstp    st(0)
		fldz                                /* Set result to 0.  */
	L5:
		add     esp, 8
		ret
	}

	#undef set_errno
}
#else
#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif
#endif

EXTERN_C const double fpconst_one;
#define _one fpconst_one

EXTERN_C __declspec(naked) double __cdecl exp(double x)
{
	double __cdecl _CIexp(/*st0 x*/);

	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		jmp     _CIexp
	}
}

EXTERN_C __declspec(naked) double __cdecl _CIexp(/*st0 x*/)
{
	__asm
	{
		fldl2e                              ; Load log base 2(e)
		fmul                                ; Multiply x * log base 2(e)
		fld     st(0)                       ; Duplicate result
		frndint                             ; Round to integer
		fsub    st(1), st(0)                ; Subtract
		fxch                                ; Exchange st, st(1)
		f2xm1                               ; Compute 2 to the (x - 1)
		fadd    qword ptr [_one]            ; 2 to the x
		fscale                              ; Scale by power of 2
		fstp    st(1)                       ; Set new stack top and pop
		ret
	}
}
#endif
