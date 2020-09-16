#ifndef _RAND_H_
#define _RAND_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <stdint.h>

#ifndef __BORLANDC__
#define __msreturn
#endif

#if defined(_MSC_VER) && _MSC_VER < 1200
#define __forceinline static __inline
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma warning(push)
#pragma warning(disable:4273)

#define srandom srand
#define random (long)rand32

void __cdecl srand(unsigned int seed);
int __cdecl rand();
uint8_t __cdecl rand8();
uint16_t __cdecl rand16();
uint32_t __cdecl rand32();
uint64_t __msreturn __cdecl rand64();
uint32_t __cdecl internal_randf32();
uint64_t __msreturn __cdecl internal_randf64();
uint32_t __cdecl internal_randf32ge0lt1();
uint32_t __cdecl internal_randf32gt0le1();
uint32_t __cdecl internal_randf32gt0lt1();
uint64_t __msreturn __cdecl internal_randf64ge0lt1();
uint64_t __msreturn __cdecl internal_randf64gt0le1();
uint64_t __msreturn __cdecl internal_randf64gt0lt1();

/**
 * This function generates and returns single precision pseudorandom
 * number which distributes uniformly in the range [0, FLT_MAX).
 */
__forceinline float randf32()
{
	uint32_t x = internal_randf32();
	return *(float *)&x;
}

/**
 * This function generates and returns double precision pseudorandom
 * number which distributes uniformly in the range [0, DBL_MAX).
 */
__forceinline double randf64()
{
	uint64_t x = internal_randf64();
	return *(double *)&x;
}

/**
 * This function generates and returns single precision pseudorandom
 * number which distributes uniformly in the range [0, 1).
 */
__forceinline float randf32ge0lt1()
{
	uint32_t x = internal_randf32ge0lt1();
	return *(float *)&x;
}

/**
 * This function generates and returns single precision pseudorandom
 * number which distributes uniformly in the range (0, 1].
 */
__forceinline float randf32gt0le1()
{
	uint32_t x = internal_randf32gt0le1();
	return *(float *)&x;
}

/**
 * This function generates and returns single precision pseudorandom
 * number which distributes uniformly in the range (0, 1).
 */
__forceinline float randf32gt0lt1()
{
	uint32_t x = internal_randf32gt0lt1();
	return *(float *)&x;
}

/**
 * This function generates and returns single precision pseudorandom
 * number which distributes uniformly in the range [1, 2).
 */
__forceinline float randf32ge1lt2()
{
	uint32_t x = (rand32() & 0x007FFFFF) + 0x3F800000;
	return *(float *)&x;
}

/**
 * This function generates and returns double precision pseudorandom
 * number which distributes uniformly in the range [0, 1).
 */
__forceinline double randf64ge0lt1()
{
	uint64_t x = internal_randf64ge0lt1();
	return *(double *)&x;
}

/**
 * This function generates and returns double precision pseudorandom
 * number which distributes uniformly in the range (0, 1].
 */
__forceinline double randf64gt0le1()
{
	uint64_t x = internal_randf64gt0le1();
	return *(double *)&x;
}

/**
 * This function generates and returns double precision pseudorandom
 * number which distributes uniformly in the range (0, 1).
 */
__forceinline double randf64gt0lt1()
{
	uint64_t x = internal_randf64gt0lt1();
	return *(double *)&x;
}

/**
 * This function generates and returns double precision pseudorandom
 * number which distributes uniformly in the range [1, 2).
 */
__forceinline double randf64ge1lt2()
{
	uint64_t x = (rand64() & 0x000FFFFFFFFFFFFF) + 0x3FF0000000000000;
	return *(double *)&x;
}

#pragma warning(pop)

#ifdef __cplusplus
}
#endif

#endif	// _RAND_H_
