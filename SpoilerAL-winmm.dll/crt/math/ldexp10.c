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

EXTERN_C double __cdecl ldexp10(double x, int exp)
{
	static const longdouble c0 = { 0xD49A000000000000, 0x4000 };	// 3.3218994140625
	static const longdouble c1 = { 0xF0979A3715FC9258, 0x3FEF };	// log2(10) - 3.3218994140625

	uint16_t cw1, cw2;
	double   y;

	if (!_finite(x))
	{
		errno = _isnan(x) ? EDOM : ERANGE;
		return x;
	}
	if (!x || !exp)
	{
		return x;
	}
	if (exp < -631)
	{
		errno = ERANGE;
		return x >= 0 ? 0.0 : -0.0;
	}
	if (exp > 631)
	{
		errno = ERANGE;
		return x >= 0 ? HUGE_VAL : -HUGE_VAL;
	}
#ifdef __cplusplus
	cw1 = longdouble::fstcw();
	cw2 = (cw1 & ~CW_RC_MASK) | CW_PC_64 | CW_EM_UNDERFLOW | CW_EM_OVERFLOW;
	for (; ; )
	{
		longdouble z, i, j, f;

		longdouble::fldcw(cw2);
		z = (longdouble)exp;
		i = (z * longdouble::fldl2t()).frndint();
		f = c0 * z - i;
		f = f + c1 * z;
		z = f.f2xm1();
		++z;
		z *= ((longdouble)x).fxtract(&j);
		i += j;
		z = z.fscale(i);
		y = (double)z;
		if (fabs(y) <= DBL_MAX)
		{
			if (y)
				break;
		}
		else if ((cw2 & CW_RC_MASK) != CW_RC_CHOP)
		{
			cw2 |= CW_RC_CHOP;
			continue;
		}
		errno = ERANGE;
		break;
	}
	x = y;
	longdouble::fclex();
	longdouble::fldcw(cw1);
	return x;
#else
	cw1 = _fstcw();
	cw2 = (cw1 & ~CW_RC_MASK) | CW_PC_64 | CW_EM_UNDERFLOW | CW_EM_OVERFLOW;
	for (; ; )
	{
		longdouble z, i, j, f;

		_fldcw(cw2);
		z = _fld_i4(exp);
		i = _frndint(_fmul(z, _fldl2t()));
		f = _fsub(_fmul(c0, z), i);
		f = _fadd(f, _fmul(c1, z));
		z = _f2xm1(f);
		z = _finc(z);
		z = _fmul(z, _fxtract(_fld_r8(x), &j));
		i = _fadd(i, j);
		z = _fscale(z, i);
		y = _fst_r8(z);
		if (fabs(y) <= DBL_MAX)
		{
			if (y)
				break;
		}
		else if ((cw2 & CW_RC_MASK) != CW_RC_CHOP)
		{
			cw2 |= CW_RC_CHOP;
			continue;
		}
		errno = ERANGE;
		break;
	}
	x = y;
	_fclex();
	_fldcw(cw1);
	return x;
#endif
}
#else
#include <errno.h>

#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif
#endif

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

EXTERN_C const double fpconst_inf;
#define _inf fpconst_inf

EXTERN_C __declspec(naked) double __cdecl ldexp10(double x, int exp)
{
	static const double       c0   = 3.3218994140625;
	static const unsigned int c1[] = { 0x15FC9258, 0xF0979A37, 0x00003FEF };	// log2(10) - 3.3218994140625

#ifdef _DEBUG
	errno_t * __cdecl _errno();
	#define set_errno(x) \
		__asm   call    _errno                  /* Get C errno variable pointer */ \
		__asm   mov     dword ptr [eax], x      /* Set error number */
#else
	extern errno_t _terrno;
	#define set_errno(x) \
		__asm   mov     dword ptr [_terrno], x  /* Set error number */
#endif

	__asm
	{
		mov     edx, dword ptr [esp + 4]
		mov     eax, dword ptr [esp + 8]
		mov     ecx, dword ptr [esp + 12]
		add     eax, eax
		cmp     eax, 7FF00000H * 2          /* x is NaN or Inf ? */
		jae     L1
		or      eax, edx                    /* x is Zero ? */
		jz      L3
		test    ecx, ecx                    /* exp is Zero ? */
		jz      L3
		cmp     ecx, -631                   /* exp < -631 ? */
		jl      L4
		cmp     ecx, 631                    /* exp > 631 ? */
		jg      L5

		/* Set round-to-nearest temporarily.  */
		sub     esp, 12
		fstcw   word ptr [esp + 8]          /* Store control word */
		mov     cx, word ptr [esp + 8]
		and     cx, not CW_RC_MASK
		or      cx, CW_PC_64        or \
		            CW_EM_UNDERFLOW or \
		            CW_EM_OVERFLOW
		jmp     L8

		align   16
	L1:
		xor     eax, 7FF00000H * 2          /* Is NaN ? */
		or      eax, edx
		jnz     L2
		set_errno(ERANGE)                   /* Set range error (ERANGE) */
		jmp     L3

		align   16
	L2:
		set_errno(EDOM)                     /* Set domain error (EDOM) */
	L3:
		fld     qword ptr [esp + 4]         /* Set result to x */
		ret

		align   16
	L4:
		set_errno(ERANGE)                   /* Set range error (ERANGE) */
		fldz                                /* Set result to 0 */
		jmp     L6

		align   16
	L5:
		set_errno(ERANGE)                   /* Set range error (ERANGE) */
		fld     qword ptr [_inf]            /* Set result to Inf */
	L6:
		cmp     dword ptr [esp + 8], 0      /* x >= 0 ? */
		jge     L7
		fchs
	L7:
		ret

		align   16
	L8:
		mov     word ptr [esp], cx          /* Set new control word */
		fldcw   word ptr [esp]
		fild    dword ptr [esp + 24]
		fldl2t                              /* 1  log2(10)        */
		fmul    st(0), st(1)                /* 1  exp * log2(10)  */
		frndint                             /* 1  i               */
		fld     qword ptr [c0]              /* 3  c0              */
		fld     st(2)                       /* 4  exp             */
		fmul                                /* 4  c0 * exp        */
		fsub    st(0), st(1)                /* 4  f = c0 * exp - i */
		fld     tbyte ptr [c1]              /* 3                  */
		fmulp   st(3), st(0)                /* 2  c1 * exp        */
		faddp   st(2), st(0)                /* 1  f = f + c1 * exp */
		fxch
		f2xm1                               /* 1 2^(fract(exp * log2(base))) - 1 */
		fld1                                /* 2 1.0              */
		fadd                                /* 1 2^(fract(exp * log2(base))) */
		fld     qword ptr [esp + 16]
		fxtract
		fmulp   st(2), st(0)
		faddp   st(2), st(0)
		fscale                              /* 1 scale factor is st(1); base^x */
		fstp    st(1)                       /* 0  */
		fstp    qword ptr [esp]             /* Cast to qword */
		fld     qword ptr [esp]
		fxam
		fstsw   ax
		and     ax, 4500H
		test    cx, CW_RC_CHOP              /* Has CW_RC_CHOP ? */
		jnz     L9
		cmp     ax, 4000H                   /* Is Zero ? */
		je      L10
		cmp     ax, 0500H                   /* Is not Inf ? */
		jne     L11
		or      cx, CW_RC_CHOP
		jmp     L8

		align   16
	L9:
		cmp     ax, 0500H                   /* Is Inf ? */
		jne     L11
	L10:
#ifdef _DEBUG
		fstp    qword ptr [esp]
		set_errno(ERANGE)                   /* Set range error (ERANGE) */
		fld     qword ptr [esp]
#else
		set_errno(ERANGE)                   /* Set range error (ERANGE) */
#endif
	L11:
		fclex                               /* Clear exceptions */
		fldcw   word ptr [esp + 8]          /* Restore control word */
		add     esp, 12
		ret
	}

	#undef set_errno
}
#endif

#define TEST 0
#if TEST
#include <stdio.h>
#include <math.h>
#include <float.h>

// "$(TargetPath)" >"$(TargetDir)$(TargetName).txt"
int main()
{
	static const double table[] = {
		1e-323, 1e-322, 1e-321, 1e-320, 1e-319, 1e-318, 1e-317, 1e-316,
		1e-315, 1e-314, 1e-313, 1e-312, 1e-311, 1e-310, 1e-309, 1e-308,
		1e-307, 1e-306, 1e-305, 1e-304, 1e-303, 1e-302, 1e-301, 1e-300,
		1e-299, 1e-298, 1e-297, 1e-296, 1e-295, 1e-294, 1e-293, 1e-292,
		1e-291, 1e-290, 1e-289, 1e-288, 1e-287, 1e-286, 1e-285, 1e-284,
		1e-283, 1e-282, 1e-281, 1e-280, 1e-279, 1e-278, 1e-277, 1e-276,
		1e-275, 1e-274, 1e-273, 1e-272, 1e-271, 1e-270, 1e-269, 1e-268,
		1e-267, 1e-266, 1e-265, 1e-264, 1e-263, 1e-262, 1e-261, 1e-260,
		1e-259, 1e-258, 1e-257, 1e-256, 1e-255, 1e-254, 1e-253, 1e-252,
		1e-251, 1e-250, 1e-249, 1e-248, 1e-247, 1e-246, 1e-245, 1e-244,
		1e-243, 1e-242, 1e-241, 1e-240, 1e-239, 1e-238, 1e-237, 1e-236,
		1e-235, 1e-234, 1e-233, 1e-232, 1e-231, 1e-230, 1e-229, 1e-228,
		1e-227, 1e-226, 1e-225, 1e-224, 1e-223, 1e-222, 1e-221, 1e-220,
		1e-219, 1e-218, 1e-217, 1e-216, 1e-215, 1e-214, 1e-213, 1e-212,
		1e-211, 1e-210, 1e-209, 1e-208, 1e-207, 1e-206, 1e-205, 1e-204,
		1e-203, 1e-202, 1e-201, 1e-200, 1e-199, 1e-198, 1e-197, 1e-196,
		1e-195, 1e-194, 1e-193, 1e-192, 1e-191, 1e-190, 1e-189, 1e-188,
		1e-187, 1e-186, 1e-185, 1e-184, 1e-183, 1e-182, 1e-181, 1e-180,
		1e-179, 1e-178, 1e-177, 1e-176, 1e-175, 1e-174, 1e-173, 1e-172,
		1e-171, 1e-170, 1e-169, 1e-168, 1e-167, 1e-166, 1e-165, 1e-164,
		1e-163, 1e-162, 1e-161, 1e-160, 1e-159, 1e-158, 1e-157, 1e-156,
		1e-155, 1e-154, 1e-153, 1e-152, 1e-151, 1e-150, 1e-149, 1e-148,
		1e-147, 1e-146, 1e-145, 1e-144, 1e-143, 1e-142, 1e-141, 1e-140,
		1e-139, 1e-138, 1e-137, 1e-136, 1e-135, 1e-134, 1e-133, 1e-132,
		1e-131, 1e-130, 1e-129, 1e-128, 1e-127, 1e-126, 1e-125, 1e-124,
		1e-123, 1e-122, 1e-121, 1e-120, 1e-119, 1e-118, 1e-117, 1e-116,
		1e-115, 1e-114, 1e-113, 1e-112, 1e-111, 1e-110, 1e-109, 1e-108,
		1e-107, 1e-106, 1e-105, 1e-104, 1e-103, 1e-102, 1e-101, 1e-100,
		1e-099, 1e-098, 1e-097, 1e-096, 1e-095, 1e-094, 1e-093, 1e-092,
		1e-091, 1e-090, 1e-089, 1e-088, 1e-087, 1e-086, 1e-085, 1e-084,
		1e-083, 1e-082, 1e-081, 1e-080, 1e-079, 1e-078, 1e-077, 1e-076,
		1e-075, 1e-074, 1e-073, 1e-072, 1e-071, 1e-070, 1e-069, 1e-068,
		1e-067, 1e-066, 1e-065, 1e-064, 1e-063, 1e-062, 1e-061, 1e-060,
		1e-059, 1e-058, 1e-057, 1e-056, 1e-055, 1e-054, 1e-053, 1e-052,
		1e-051, 1e-050, 1e-049, 1e-048, 1e-047, 1e-046, 1e-045, 1e-044,
		1e-043, 1e-042, 1e-041, 1e-040, 1e-039, 1e-038, 1e-037, 1e-036,
		1e-035, 1e-034, 1e-033, 1e-032, 1e-031, 1e-030, 1e-029, 1e-028,
		1e-027, 1e-026, 1e-025, 1e-024, 1e-023, 1e-022, 1e-021, 1e-020,
		1e-019, 1e-018, 1e-017, 1e-016, 1e-015, 1e-014, 1e-013, 1e-012,
		1e-011, 1e-010, 1e-009, 1e-008, 1e-007, 1e-006, 1e-005, 1e-004,
		1e-003, 1e-002, 1e-001, 1e+000, 1e+001, 1e+002, 1e+003, 1e+004,
		1e+005, 1e+006, 1e+007, 1e+008, 1e+009, 1e+010, 1e+011, 1e+012,
		1e+013, 1e+014, 1e+015, 1e+016, 1e+017, 1e+018, 1e+019, 1e+020,
		1e+021, 1e+022, 1e+023, 1e+024, 1e+025, 1e+026, 1e+027, 1e+028,
		1e+029, 1e+030, 1e+031, 1e+032, 1e+033, 1e+034, 1e+035, 1e+036,
		1e+037, 1e+038, 1e+039, 1e+040, 1e+041, 1e+042, 1e+043, 1e+044,
		1e+045, 1e+046, 1e+047, 1e+048, 1e+049, 1e+050, 1e+051, 1e+052,
		1e+053, 1e+054, 1e+055, 1e+056, 1e+057, 1e+058, 1e+059, 1e+060,
		1e+061, 1e+062, 1e+063, 1e+064, 1e+065, 1e+066, 1e+067, 1e+068,
		1e+069, 1e+070, 1e+071, 1e+072, 1e+073, 1e+074, 1e+075, 1e+076,
		1e+077, 1e+078, 1e+079, 1e+080, 1e+081, 1e+082, 1e+083, 1e+084,
		1e+085, 1e+086, 1e+087, 1e+088, 1e+089, 1e+090, 1e+091, 1e+092,
		1e+093, 1e+094, 1e+095, 1e+096, 1e+097, 1e+098, 1e+099, 1e+100,
		1e+101, 1e+102, 1e+103, 1e+104, 1e+105, 1e+106, 1e+107, 1e+108,
		1e+109, 1e+110, 1e+111, 1e+112, 1e+113, 1e+114, 1e+115, 1e+116,
		1e+117, 1e+118, 1e+119, 1e+120, 1e+121, 1e+122, 1e+123, 1e+124,
		1e+125, 1e+126, 1e+127, 1e+128, 1e+129, 1e+130, 1e+131, 1e+132,
		1e+133, 1e+134, 1e+135, 1e+136, 1e+137, 1e+138, 1e+139, 1e+140,
		1e+141, 1e+142, 1e+143, 1e+144, 1e+145, 1e+146, 1e+147, 1e+148,
		1e+149, 1e+150, 1e+151, 1e+152, 1e+153, 1e+154, 1e+155, 1e+156,
		1e+157, 1e+158, 1e+159, 1e+160, 1e+161, 1e+162, 1e+163, 1e+164,
		1e+165, 1e+166, 1e+167, 1e+168, 1e+169, 1e+170, 1e+171, 1e+172,
		1e+173, 1e+174, 1e+175, 1e+176, 1e+177, 1e+178, 1e+179, 1e+180,
		1e+181, 1e+182, 1e+183, 1e+184, 1e+185, 1e+186, 1e+187, 1e+188,
		1e+189, 1e+190, 1e+191, 1e+192, 1e+193, 1e+194, 1e+195, 1e+196,
		1e+197, 1e+198, 1e+199, 1e+200, 1e+201, 1e+202, 1e+203, 1e+204,
		1e+205, 1e+206, 1e+207, 1e+208, 1e+209, 1e+210, 1e+211, 1e+212,
		1e+213, 1e+214, 1e+215, 1e+216, 1e+217, 1e+218, 1e+219, 1e+220,
		1e+221, 1e+222, 1e+223, 1e+224, 1e+225, 1e+226, 1e+227, 1e+228,
		1e+229, 1e+230, 1e+231, 1e+232, 1e+233, 1e+234, 1e+235, 1e+236,
		1e+237, 1e+238, 1e+239, 1e+240, 1e+241, 1e+242, 1e+243, 1e+244,
		1e+245, 1e+246, 1e+247, 1e+248, 1e+249, 1e+250, 1e+251, 1e+252,
		1e+253, 1e+254, 1e+255, 1e+256, 1e+257, 1e+258, 1e+259, 1e+260,
		1e+261, 1e+262, 1e+263, 1e+264, 1e+265, 1e+266, 1e+267, 1e+268,
		1e+269, 1e+270, 1e+271, 1e+272, 1e+273, 1e+274, 1e+275, 1e+276,
		1e+277, 1e+278, 1e+279, 1e+280, 1e+281, 1e+282, 1e+283, 1e+284,
		1e+285, 1e+286, 1e+287, 1e+288, 1e+289, 1e+290, 1e+291, 1e+292,
		1e+293, 1e+294, 1e+295, 1e+296, 1e+297, 1e+298, 1e+299, 1e+300,
		1e+301, 1e+302, 1e+303, 1e+304, 1e+305, 1e+306, 1e+307, 1e+308,
	};
	double x, y;
	for (int i = -323; i <= 308; i++)
	{
		x = table[i + 323];
		y = ldexp10(1, i);
		if (x == y)
		{
			printf("match  : %.50e\n", x);
		}
		else
		{
			printf("unmatch: %.50e\n", x);
			printf("ldexp10: %.50e\n", y);
			printf("pow    : %.50e\n", pow(10, i));
		}
	}
	x = ldexp10(179769313486231580.0, 291);
	if (x == DBL_MAX)
	{
		printf("match  : %.50e\n", DBL_MAX);
	}
	else
	{
		printf("unmatch: %.50e\n", DBL_MAX);
		printf("ldexp10: %.50e\n", x);
	}
	x = ldexp10(17976931348623158.0, 292);
	if (x == DBL_MAX)
	{
		printf("match  : %.50e\n", DBL_MAX);
	}
	else
	{
		printf("unmatch: %.50e\n", DBL_MAX);
		printf("ldexp10: %.50e\n", x);
	}
	x = ldexp10(494065645841246560.0, -341);
	if (x == DBL_TRUE_MIN)
	{
		printf("match  : %.50e\n", DBL_TRUE_MIN);
	}
	else
	{
		printf("unmatch: %.50e\n", DBL_TRUE_MIN);
		printf("ldexp10: %.50e\n", x);
	}
	x = ldexp10(49406564584124656.0, -340);
	if (x == DBL_TRUE_MIN)
	{
		printf("match  : %.50e\n", DBL_TRUE_MIN);
	}
	else
	{
		printf("unmatch: %.50e\n", DBL_TRUE_MIN);
		printf("ldexp10: %.50e\n", x);
	}
	return 0;
}
#endif
