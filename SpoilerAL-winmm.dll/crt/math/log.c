#pragma function(log)

#ifndef _M_IX86
#include <math.h>
#include <float.h>
#include <errno.h>

#ifndef M_SQRT1_2
#define M_SQRT1_2 0.707106781186547524400844362104849039
#endif
#ifndef M_LN2
#define M_LN2 0.693147180559945309417232121458176568
#endif

#define A0   -0.64124943423745581147e2
#define A1    0.16383943563021534222e2
#define A2   -0.78956112887491257267e0
#define A(x) ((A2 * (x) + A1) * (x) + A0)

#define B0   -0.76949932108494879777e3
#define B1    0.31203222091924532844e3
#define B2   -0.35667977739034646171e2
#define B(x) ((((x) + B2) * (x) + B1) * (x) + B0)

double __cdecl log(double x)
{
	if (!_isnan(x))
	{
		if (x > 0)
		{
			if (x <= DBL_MAX)
			{
				double znum, zden, z, w;
				int    exponent;

				x = frexp(x, &exponent);
				if (x > M_SQRT1_2)
				{
					znum = (x - 0.5) - 0.5;
					zden = x * 0.5 + 0.5;
				}
				else
				{
					znum = x - 0.5;
					zden = znum * 0.5 + 0.5;
					exponent--;
				}
				z = znum / zden;
				w = z * z;
				x = z + z * w * (A(w) / B(w));
				x += exponent * M_LN2;
			}
		}
		else if (x)
		{
			errno = EDOM;
			x = NAN;
		}
		else
		{
			errno = ERANGE;
			x = -INFINITY;
		}
	}
	return x;
}
#else
#include <errno.h>

__declspec(naked) double __cdecl log(double x)
{
	double __cdecl _CIlog(/*st0 x*/);

	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		jmp     _CIlog
	}
}

__declspec(naked) double __cdecl _CIlog(/*st0 x*/)
{
	extern const double fpconst_one;
	extern const double fpconst_minus_inf;
	extern const double fpconst_nan_ind;
	#define _one       fpconst_one
	#define _minus_inf fpconst_minus_inf
	#define _nan_ind   fpconst_nan_ind

	static const double limit = 0.29;

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
		fxam
		fnstsw  ax
		sahf
		jnc     L1                          // x is not NaN and +-Inf ?
		jnp     L3                          // x is not +-Inf ?
	L1:
		ftst
		fnstsw  ax
		sahf
		jbe     L4                          // x <= 0 ?
		fldln2                              // log(2) : x
		fxch                                // x : log(2)
		fld     st(0)                       // x : x : log(2)
		fsub    qword ptr [_one]            // x-1 : x : log(2)
		fld     st(0)                       // x-1 : x-1 : x : log(2)
		fabs                                // |x-1| : x-1 : x : log(2)
		fcomp   qword ptr [limit]           // x-1 : x : log(2)
		fnstsw  ax                          // x-1 : x : log(2)
		test    ah, 45H
		jz      L6
		fxam
		fnstsw  ax
		and     ah, 45H
		cmp     ah, 40H
		jne     L2
		fabs                                // log(1) is +0 in all rounding modes.
	L2:
		fstp    st(1)                       // x-1 : log(2)
		fyl2xp1                             // log(x)
	L3:
		ret

		align   16
	L4:
		fstp    st(0)
		je      L5                          // x == 0 ?
		set_errno(EDOM)                     // Set domain error (EDOM)
		fld     qword ptr [_nan_ind]        // Load NaN(indeterminate)
		ret

		align   16
	L5:
		set_errno(ERANGE)                   // Set range error (ERANGE)
		fld     qword ptr [_minus_inf]      // Load -Inf
		ret

		align   16
	L6:
		fstp    st(0)                       // x : log(2)
		fyl2x                               // log(x)
		ret
	}

	#undef set_errno
}
#endif
