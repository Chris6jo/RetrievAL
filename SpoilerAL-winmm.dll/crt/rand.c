/*
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

#include "rand.h"
#if defined(_M_IX86) || defined(_M_X64)
#include <emmintrin.h>
#else
typedef union __declspec(align(16)) {
	uint32_t m128i_u32[4];
	uint64_t m128i_u64[2];
} __m128i;
#endif

/* definitions */
#define SFMT_MEXP    19937                  // Mersenne Exponent. The period of the sequence is a multiple of 2^SFMT_MEXP-1.
#define SFMT_N       (SFMT_MEXP / 128 + 1)  // SFMT generator has an internal state array of 128-bit integers, and SFMT_N is its size.
#define SFMT_N32     (SFMT_N * 4)           // SFMT_N32 is the size of internal state array when regarded as an array of 32-bit integers.
#define SFMT_POS1    122                    // the pick up position of the array.
#define SFMT_SL1     18                     // the parameter of shift left as four 32-bit registers.
#define SFMT_SL2     1                      // the parameter of shift left as one 128-bit register. The 128-bit integer is shifted by (SFMT_SL2 * 8) bits.
#define SFMT_SR1     11                     // the parameter of shift right as four 32-bit registers.
#define SFMT_SR2     1                      // the parameter of shift right as one 128-bit register. The 128-bit integer is shifted by (SFMT_SR2 * 8) bits.
#define SFMT_MSK1    0xDFFFFFEFU            // A bitmask, used in the recursion.  These parameters are introduced to break symmetry of SIMD.
#define SFMT_MSK2    0xDDFECB7FU            //
#define SFMT_MSK3    0xBFFAFFFFU            //
#define SFMT_MSK4    0xBFFFFFF6U            //
#define SFMT_PARITY1 0x00000001U            // These definitions are part of a 128-bit period certification vector. */
#define SFMT_PARITY2 0x00000000U            //
#define SFMT_PARITY3 0x00000000U            //
#define SFMT_PARITY4 0x13C9E684U            //

/* variables */
static __m128i sfmt[SFMT_N];                // the array for the state vector
#define        sfmt32 ((uint32_t *)sfmt)    //
static size_t  idx;                         // index of sfmt32 array

/* functions */
#if defined(_M_IX86)
static void sfmt_gen_rand_all_sse2();
static void sfmt_gen_rand_all_generic();
static void sfmt_gen_rand_all_cpu_dispatch();

static void (*sfmt_gen_rand_all_dispatch)() = sfmt_gen_rand_all_cpu_dispatch;
#endif

/* macros */
#define BSF8(x, default) (  \
    ((x) & 0x01) ?  0 :     \
    ((x) & 0x02) ?  1 :     \
    ((x) & 0x04) ?  2 :     \
    ((x) & 0x08) ?  3 :     \
    ((x) & 0x10) ?  4 :     \
    ((x) & 0x20) ?  5 :     \
    ((x) & 0x40) ?  6 :     \
    ((x) & 0x80) ?  7 :     \
    (default))
#define BSF16(x, default) BSF8(x, BSF8((x) >> 8, (default) - 8) + 8)
#define BSF32(x, default) BSF16(x, BSF16((x) >> 16, (default) - 16) + 16)

/* This function initializes the internal state array with a 32-bit
   integer seed. */
void __cdecl srand(unsigned int seed)
{
	uint32_t x;
	size_t i;

	sfmt32[0] = x = seed;
	for (i = 1; i < SFMT_N32; i++)
		sfmt32[i] = x = ((x >> 30) ^ x) * 1812433253UL + i;
	idx = SFMT_N32;
	/* certificate the period of 2^{MEXP} */
	x =  sfmt32[0] & SFMT_PARITY1;
	x ^= sfmt32[1] & SFMT_PARITY2;
	x ^= sfmt32[2] & SFMT_PARITY3;
	x ^= sfmt32[3] & SFMT_PARITY4;
	x ^= x >> 16;
	x ^= x >> 8;
	x ^= x >> 4;
	x ^= x >> 2;
	x ^= x >> 1;
	x &= 1;
#if SFMT_PARITY1
	sfmt32[0] ^= x << BSF32(SFMT_PARITY1, -1);
#elif SFMT_PARITY2
	sfmt32[1] ^= x << BSF32(SFMT_PARITY2, -1);
#elif SFMT_PARITY3
	sfmt32[2] ^= x << BSF32(SFMT_PARITY3, -1);
#elif SFMT_PARITY4
	sfmt32[3] ^= x << BSF32(SFMT_PARITY4, -1);
#endif
}

int __cdecl rand()
{
	return rand32() & 0x00007FFF;
}

uint8_t __cdecl rand8()
{
	return (uint8_t)rand32();
}

uint16_t __cdecl rand16()
{
	return (uint16_t)rand32();
}

#if defined(_M_IX86)
__declspec(naked) static void sfmt_gen_rand_all()
{
	__asm
	{
		jmp     dword ptr [sfmt_gen_rand_all_dispatch]
	}
}
#endif

#if defined(_M_IX86) || defined(_M_X64)
/* This function represents the recursion formula. */
#define mm_recursion(r, a, b, c, d, mask)   \
do {                                        \
    __m128i __v, __x, __y, __z;             \
                                            \
    __y = _mm_srli_epi32(b, SFMT_SR1);      \
    __z = _mm_srli_si128(c, SFMT_SR2);      \
    __v = _mm_slli_epi32(d, SFMT_SL1);      \
    __z = _mm_xor_si128(__z, a);            \
    __z = _mm_xor_si128(__z, __v);          \
    __x = _mm_slli_si128(a, SFMT_SL2);      \
    __y = _mm_and_si128(__y, mask);         \
    __z = _mm_xor_si128(__z, __x);          \
    __z = _mm_xor_si128(__z, __y);          \
    *(r) = __z;                             \
} while (0)

/* This function fills the internal state array with pseudorandom
   integers. */
#if defined(_M_IX86)
static void sfmt_gen_rand_all_sse2()
#else
static void sfmt_gen_rand_all()
#endif
{
	__declspec(align(16)) static const union {
		uint32_t u[4];
		__m128i si;
	} mask = { { SFMT_MSK1, SFMT_MSK2, SFMT_MSK3, SFMT_MSK4 } };
	__m128i r1, r2, *p;

	r1 = sfmt[SFMT_N - 2];
	r2 = sfmt[SFMT_N - 1];
	p = sfmt;
	do {
		mm_recursion(p, *p, p[SFMT_POS1], r1, r2, mask.si);
		r1 = r2;
		r2 = *p;
	} while (++p != sfmt + SFMT_N - SFMT_POS1);
	do {
		mm_recursion(p, *p, p[SFMT_POS1 - SFMT_N], r1, r2, mask.si);
		r1 = r2;
		r2 = *p;
	} while (++p != sfmt + SFMT_N);
}
#endif

#if !defined(_M_X64)
#if !defined(_M_IX86)
/* This function simulates SIMD 128-bit left shift by the standard C.
   The 128-bit integer given in in is shifted by (shift * 8) bits.
   This function simulates the LITTLE ENDIAN SIMD. */
#define lshift128(r, a, shift)                                      \
do {                                                                \
    uint32_t __t = (a)->m128i_u32[1] >> (32 - (shift) * 8);         \
    (r)->m128i_u64[0] =  (a)->m128i_u64[0] << ((shift) * 8);        \
    (r)->m128i_u64[1] = ((a)->m128i_u64[1] << ((shift) * 8)) | __t; \
} while (0)

/* This function simulates SIMD 128-bit right shift by the standard C.
   The 128-bit integer given in in is shifted by (shift * 8) bits.
   This function simulates the LITTLE ENDIAN SIMD. */
#define rshift128(r, a, shift)                                                          \
do {                                                                                    \
    uint32_t __t = (a)->m128i_u32[2] << (32 - (shift) * 8);                             \
    (r)->m128i_u64[1] =  (a)->m128i_u64[1] >> ((shift) * 8);                            \
    (r)->m128i_u64[0] = ((a)->m128i_u64[0] >> ((shift) * 8)) | ((uint64_t)__t << 32);   \
} while (0)

#define xor128(r, a, b)                                         \
do {                                                            \
    (r)->m128i_u64[0] = (a)->m128i_u64[0] ^ (b)->m128i_u64[0];  \
    (r)->m128i_u64[1] = (a)->m128i_u64[1] ^ (b)->m128i_u64[1];  \
} while (0)

/* This function represents the recursion formula. */
#define do_recursion(a, b, c, d)                                        \
do {                                                                    \
    __m128i __x;                                                        \
                                                                        \
    lshift128(&__x, a, SFMT_SL2);                                       \
    xor128(a, a, &__x);                                                 \
    (a)->m128i_u32[0] ^= ((b)->m128i_u32[0] >> SFMT_SR1) & SFMT_MSK1;   \
    (a)->m128i_u32[1] ^= ((b)->m128i_u32[1] >> SFMT_SR1) & SFMT_MSK2;   \
    (a)->m128i_u32[2] ^= ((b)->m128i_u32[2] >> SFMT_SR1) & SFMT_MSK3;   \
    (a)->m128i_u32[3] ^= ((b)->m128i_u32[3] >> SFMT_SR1) & SFMT_MSK4;   \
    rshift128(&__x, c, SFMT_SR2);                                       \
    xor128(a, a, &__x);                                                 \
    (a)->m128i_u32[0] ^= (d)->m128i_u32[0] << SFMT_SL1;                 \
    (a)->m128i_u32[1] ^= (d)->m128i_u32[1] << SFMT_SL1;                 \
    (a)->m128i_u32[2] ^= (d)->m128i_u32[2] << SFMT_SL1;                 \
    (a)->m128i_u32[3] ^= (d)->m128i_u32[3] << SFMT_SL1;                 \
} while (0)
#else
__declspec(naked) static void __fastcall do_recursion(__m128i *a, __m128i *b, __m128i *c, __m128i *d)
{
	__asm
	{
		#define a ecx
		#define b edx
		#define c (esp + 4)
		#define d (esp + 8)

		push    ebx
		push    ebp
		push    esi
		push    edi

		// lshift128(&__x, a, SFMT_SL2);
		mov     edi, dword ptr [ecx + 12]
		mov     esi, dword ptr [ecx +  8]
		shl     edi, SFMT_SL2 * 8
		mov     ebp, esi
		shr     ebp, 32 - SFMT_SL2 * 8
		mov     ebx, dword ptr [ecx +  4]
		mov     eax, dword ptr [ecx     ]
		or      edi, ebp
		shl     esi, SFMT_SL2 * 8
		mov     ebp, ebx
		shr     ebp, 32 - SFMT_SL2 * 8
		push    ecx
		shl     ebx, SFMT_SL2 * 8
		mov     ecx, eax
		shr     ecx, 32 - SFMT_SL2 * 8
		or      esi, ebp
		shl     eax, SFMT_SL2 * 8
		or      ebx, ecx
		pop     ecx

		// xor128(a, a, &__x);
		xor     eax, dword ptr [ecx     ]
		xor     ebx, dword ptr [ecx +  4]
		xor     dword ptr [ecx +  8], esi
		xor     dword ptr [ecx + 12], edi

		// (a)->m128i_u32[0] ^= ((b)->m128i_u32[0] >> SFMT_SR1) & SFMT_MSK1;
		// (a)->m128i_u32[1] ^= ((b)->m128i_u32[1] >> SFMT_SR1) & SFMT_MSK2;
		// (a)->m128i_u32[2] ^= ((b)->m128i_u32[2] >> SFMT_SR1) & SFMT_MSK3;
		// (a)->m128i_u32[3] ^= ((b)->m128i_u32[3] >> SFMT_SR1) & SFMT_MSK4;
		mov     esi, dword ptr [edx     ]
		mov     edi, dword ptr [edx +  4]
		shr     esi, SFMT_SR1
		mov     ebp, dword ptr [edx +  8]
		shr     edi, SFMT_SR1
		mov     edx, dword ptr [edx + 12]
		shr     ebp, SFMT_SR1
		and     esi, SFMT_MSK1
		shr     edx, SFMT_SR1
		and     edi, SFMT_MSK2
		and     ebp, SFMT_MSK3
		and     edx, SFMT_MSK4
		xor     eax, esi
		xor     ebx, edi
		mov     dword ptr [ecx     ], eax
		mov     dword ptr [ecx +  4], ebx
		xor     dword ptr [ecx +  8], ebp
		xor     dword ptr [ecx + 12], edx
		mov     ebx, dword ptr [c + 16]

		// rshift128(&__x, c, SFMT_SR2);
		mov     eax, dword ptr [ebx     ]
		mov     edx, dword ptr [ebx +  4]
		shr     eax, SFMT_SR2 * 8
		mov     ebp, edx
		shl     ebp, 32 - SFMT_SR2 * 8
		mov     esi, dword ptr [ebx +  8]
		mov     edi, dword ptr [ebx + 12]
		or      eax, ebp
		shr     edx, SFMT_SR2 * 8
		mov     ebp, esi
		shl     ebp, 32 - SFMT_SR2 * 8
		mov     ebx, edi
		shr     esi, SFMT_SR2 * 8
		or      edx, ebp
		shl     ebx, 32 - SFMT_SR2 * 8
		mov     ebp, dword ptr [d + 16]
		shr     edi, SFMT_SR2 * 8
		or      esi, ebx

		// xor128(a, a, &__x);
		xor     eax, dword ptr [ecx     ]
		xor     edx, dword ptr [ecx +  4]
		xor     dword ptr [ecx +  8], esi
		xor     dword ptr [ecx + 12], edi

		// (a)->m128i_u32[0] ^= (d)->m128i_u32[0] << SFMT_SL1;
		// (a)->m128i_u32[1] ^= (d)->m128i_u32[1] << SFMT_SL1;
		// (a)->m128i_u32[2] ^= (d)->m128i_u32[2] << SFMT_SL1;
		// (a)->m128i_u32[3] ^= (d)->m128i_u32[3] << SFMT_SL1;
		mov     esi, dword ptr [ebp     ]
		mov     edi, dword ptr [ebp +  4]
		shl     esi, SFMT_SL1
		mov     ebx, dword ptr [ebp +  8]
		shl     edi, SFMT_SL1
		mov     ebp, dword ptr [ebp + 12]
		shl     ebx, SFMT_SL1
		xor     eax, esi
		shl     ebp, SFMT_SL1
		xor     edx, edi
		mov     dword ptr [ecx     ], eax
		mov     dword ptr [ecx +  4], edx
		xor     dword ptr [ecx +  8], ebx
		xor     dword ptr [ecx + 12], ebp

		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret     8

		#undef a
		#undef b
		#undef c
		#undef d
	}
}
#endif

/* This function fills the internal state array with pseudorandom
   integers. */
#if defined(_M_IX86)
static void sfmt_gen_rand_all_generic()
#else
static void sfmt_gen_rand_all()
#endif
{
	__m128i *r1, *r2, *p;

	r1 = sfmt + SFMT_N - 2;
	r2 = sfmt + SFMT_N - 1;
	p = sfmt;
	do {
		do_recursion(p, p + SFMT_POS1, r1, r2);
		r1 = r2;
		r2 = p;
	} while (++p != sfmt + SFMT_N - SFMT_POS1);
	do {
		do_recursion(p, p + SFMT_POS1 - SFMT_N, r1, r2);
		r1 = r2;
		r2 = p;
	} while (++p != sfmt + SFMT_N);
}
#endif

#if defined(_M_IX86)
__declspec(naked) static void sfmt_gen_rand_all_cpu_dispatch()
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [sfmt_gen_rand_all_dispatch], offset sfmt_gen_rand_all_generic
		jmp     sfmt_gen_rand_all_generic
	L1:
		mov     dword ptr [sfmt_gen_rand_all_dispatch], offset sfmt_gen_rand_all_sse2
		jmp     sfmt_gen_rand_all_sse2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif

/* This function generates and returns 32-bit pseudorandom number.
   init_gen_rand or init_by_array must be called before this function. */
uint32_t __cdecl rand32()
{
	if (idx >= SFMT_N32) {
		sfmt_gen_rand_all();
		idx = 0;
	}
	return sfmt32[idx++];
}

/* This function generates and returns 64-bit pseudorandom number.
   init_gen_rand or init_by_array must be called before this function. */
uint64_t __cdecl rand64()
{
	uint64_t r;

	idx = (idx + 1) & -2;
	if (idx >= SFMT_N32) {
		sfmt_gen_rand_all();
		idx = 0;
	}
	r = *(uint64_t *)(sfmt32 + idx);
	idx += 2;
	return r;
}

float __cdecl randf32()
{
	uint32_t r;

	r = ((rand32() & 1) << 31) | ((rand32() & 0xFF) << 23) | (rand32() & 0x007FFFFF);
	return *(float *)&r;
}

double __cdecl randf64()
{
	uint64_t r;

	r = ((uint64_t)(rand32() & 1) << 63) | ((uint64_t)(rand32() & 0x07FF) << 52) | (rand64() & 0x000FFFFFFFFFFFFF);
	return *(double *)&r;
}
