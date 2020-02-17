#include <time.h>
#include <errno.h>
#include <stdint.h>
#include <intrin.h>
#pragma intrinsic(_addcarry_u32)
#pragma intrinsic(_subborrow_u32)

#define OPTIMIZABLE_C 0

#define MAX_TIME_T     0x0000000793582AFF   // Number of seconds from 00:00:00, 01/01/1970 UTC to 07:59:59, 01/19/3001 UTC
#define MAX_LOCAL_TIME (13 * 60 * 60)       // Maximum local time adjustment (GMT + 13 Hours, DST -0 Hours)
#define MIN_LOCAL_TIME (-12 * 60 * 60)      // Minimum local time adjustment (GMT - 11 Hours, DST - 1 Hours)

#if !defined(_M_IX86) || OPTIMIZABLE_C
errno_t __cdecl _gmtime32_s(struct tm *dest, const __time32_t *source)
{
	if (dest)
	{
		if (source)
		{
			#define SIZE_OF_TIME 4

			uint32_t time32;

			if ((int32_t)(time32 = *source) >= MIN_LOCAL_TIME)
				#include "gmtime_common.h"

			#undef SIZE_OF_TIME
		}
		dest->tm_sec   = -1;
		dest->tm_min   = -1;
		dest->tm_hour  = -1;
		dest->tm_mday  = -1;
		dest->tm_mon   = -1;
		dest->tm_year  = -1;
		dest->tm_wday  = -1;
		dest->tm_yday  = -1;
		dest->tm_isdst = -1;
	}
	return EINVAL;
}
#else
#include "gmtime32_asm.h"
#endif

struct tm * __cdecl _gmtime32(__time32_t const *source)
{
	static struct tm dest;
	errno_t status;

	if (!(status = _gmtime32_s(&dest, source)))
		return &dest;
	errno = status;
	return NULL;
}

#if !defined(_M_IX86) || OPTIMIZABLE_C
errno_t __cdecl _gmtime64_s(struct tm *dest, const __time64_t *source)
{
	if (dest)
	{
		if (source)
		{
			#define SIZE_OF_TIME 8

			uint64_t time64;

			if ((int64_t)(time64 = *source) <= INT32_MAX)
				return _gmtime32_s(dest, (const __time32_t *)source);
			else if (time64 <= MAX_TIME_T + MAX_LOCAL_TIME)
				#include "gmtime_common.h"

			#undef SIZE_OF_TIME
		}
		dest->tm_sec   = -1;
		dest->tm_min   = -1;
		dest->tm_hour  = -1;
		dest->tm_mday  = -1;
		dest->tm_mon   = -1;
		dest->tm_year  = -1;
		dest->tm_wday  = -1;
		dest->tm_yday  = -1;
		dest->tm_isdst = -1;
	}
	return EINVAL;
}
#else
#include "gmtime64_asm.h"
#endif

struct tm * __cdecl _gmtime64(__time64_t const *source)
{
	static struct tm dest;
	errno_t status;

	if (!(status = _gmtime64_s(&dest, source)))
		return &dest;
	errno = status;
	return NULL;
}

#if TEST
#include <stdlib.h>	// using srand, rand
static uint32_t test_gmtime64_s()
{
	static const uint32_t mdays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	uint32_t year, mon, mday, hour, min, sec, days, leap, yday, mdaymax;
	uint64_t source;
	struct tm dest;

	srand((unsigned int)time(NULL));
	days = 0;
	for (year = 1970; year <= 3000; year++)
	{
		leap = !(year % 4) && (year % 100 || !(year % 400));
		yday = 0;
		for (mon = 0; mon < 12; mon++)
		{
			mdaymax = mdays[mon] + (mon == 1 && leap);
			for (mday = 1; mday <= mdaymax; mday++)
			{
				hour = rand() % 24;
				min  = rand() % 60;
				sec  = rand() % 60;
				source = (uint64_t)days * (24 * 60 * 60) + (hour * (60 * 60) + min * 60 + sec);
				_gmtime64_s(&dest, &source);
				if (dest.tm_sec   != sec            ||
					dest.tm_min   != min            ||
					dest.tm_hour  != hour           ||
					dest.tm_mday  != mday           ||
					dest.tm_mon   != mon            ||
					dest.tm_year  != year - 1900    ||
					dest.tm_wday  != (days + 4) % 7 ||
					dest.tm_yday  != yday           ||
					dest.tm_isdst != 0)
					return 0;
				days++;
				yday++;
			}
		}
	}
	return 1;
}

int main()
{
	printf(test_gmtime64_s() ? "success\n" : "failed\n");
	return 0;
}
#endif
