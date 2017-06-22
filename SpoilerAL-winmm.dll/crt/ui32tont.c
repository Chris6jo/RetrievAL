#if defined(_ui32tont)
#include <windows.h>
#if defined(_MSC_VER) && _MSC_VER >= 1310
#include <intrin.h>
#pragma intrinsic(__emulu)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned __int64 __emulu(unsigned int a, unsigned int b)
{
	__asm
	{
		mov     edx, dword ptr [b]
		mov     eax, dword ptr [a]
		mul     edx
	}
}
#else
#define __emulu(a, b) ((unsigned __int64)(unsigned int)(a) * (unsigned int)(b))
#endif
#include "digitstbl.h"

#ifdef _MSC_VER
#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN    4321
#define __BYTE_ORDER    __LITTLE_ENDIAN
#endif

typedef __int32          int32_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

#ifdef _UNICODE
typedef uint32_t tchar2_t;
#define digitsDec100T ((tchar2_t *)digitsDec100W)
#else
typedef uint16_t tchar2_t;
#define digitsDec100T ((tchar2_t *)digitsDec100A)
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define TO_TCHAR2(c) (tchar2_t)(c)
#else
#define TO_TCHAR2(c) ((tchar2_t)(c) << (sizeof(TCHAR) * 8))
#endif

#define _ui32to10t _ui32tont(10)
#define _ui32to16t _ui32tont(16)
#define _ui32to8t  _ui32tont(8)

#ifndef _M_IX86
size_t __fastcall _ui32to10t(uint32_t value, TCHAR *buffer)
{
	if (value >= 1000000u)
		if (value >= 100000000u)
			if (value >= 1000000000u)
				goto LENGTH10;
			else
				goto LENGTH9;
		else
			if (value >= 10000000u)
				goto LENGTH8;
			else
				goto LENGTH7;
	else
		if (value >= 10000u)
			if (value >= 100000u)
				goto LENGTH6;
			else
				goto LENGTH5;
		else
			if (value >= 100u)
				if (value >= 1000u)
					goto LENGTH4;
				else
					goto LENGTH3;
			else
				if (value >= 10u)
					goto LENGTH2;
				else
					goto LENGTH1;

LENGTH10:
	{
		const uint64_t reciprocal_u8 = (((uint64_t)1 << (32 + 25)) / 10000000u);
		const uint32_t reciprocal_lo = (uint32_t)reciprocal_u8;
		const uint32_t reciprocal_hi = (uint32_t)(reciprocal_u8 >> 32);

		do	// do { ... } while (0);
		{
			if ((int32_t)(value -= 3000000000) >= 0)
			{
				if ((int32_t)(value -= 1000000000) >= 0)
				{
					buffer[0] = TEXT('4');
					break;
				}
				buffer[0] = TEXT('3');
			}
			else
			{
				if ((int32_t)(value += 1000000000) >= 0)
				{
					buffer[0] = TEXT('2');
					break;
				}
				buffer[0] = TEXT('1');
			}
			value += 1000000000;
		} while (0);
		value = (uint32_t)(__emulu(value, reciprocal_lo) >> 32)
			+ value * reciprocal_hi
			+ 2;
		*(tchar2_t *)&buffer[1] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[3] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[5] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[7] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 10;
		*(tchar2_t *)&buffer[9] = TO_TCHAR2((value >> 25) + TEXT('0'));
		return 10;
	}

LENGTH9:
	{
		const uint64_t reciprocal_u8 = (((uint64_t)1 << (32 + 25)) / 10000000u);
		const uint32_t reciprocal_lo = (uint32_t)reciprocal_u8;
		const uint32_t reciprocal_hi = (uint32_t)(reciprocal_u8 >> 32);

		value = (uint32_t)(__emulu(value, reciprocal_lo) >> 32)
			+ value * reciprocal_hi
			+ 2;
		*(tchar2_t *)&buffer[0] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[2] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[4] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[6] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 10;
		*(tchar2_t *)&buffer[8] = TO_TCHAR2((value >> 25) + TEXT('0'));
		return 9;
	}

LENGTH8:
	{
		value = value * ((1 << 25) / 1000000)
			+ (value >> 1)
			+ (value >> 4)
			- (value >> 7)
			- (value >> 12)
			- (value >> 17)
			- (value >> 18)
			+ (value >> 21)
			- (value >> 23);
		*(tchar2_t *)&buffer[0] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[2] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[4] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[6] = digitsDec100T[value >> 25];
		              buffer[8] = TEXT('\0');
		return 8;
	}

LENGTH7:
	{
		value = value * ((1 << 25) / 100000)
			+ (value >> 1)
			+ (value >> 5)
			+ (value >> 6)
			- (value >> 9)
			- (value >> 11)
			- (value >> 13)
			+ (value >> 15);
		*(tchar2_t *)&buffer[0] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[2] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[4] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 10;
		*(tchar2_t *)&buffer[6] = TO_TCHAR2((value >> 25) + TEXT('0'));
		return 7;
	}

LENGTH6:
	{
		value = value * ((1 << 25) / 10000)
			+ (value >> 1)
			- (value >> 4)
			+ (value >> 7);
		*(tchar2_t *)&buffer[0] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[2] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[4] = digitsDec100T[value >> 25];
		              buffer[6] = TEXT('\0');
		return 6;
	}

LENGTH5:
	{
		value = value * ((1 << 25) / 1000 + 1) - (value >> 2);
		*(tchar2_t *)&buffer[0] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[2] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 10;
		*(tchar2_t *)&buffer[4] = TO_TCHAR2((value >> 25) + TEXT('0'));
		return 5;
	}

LENGTH4:
	{
		value = value * ((1 << 25) / 100 + 1);
		*(tchar2_t *)&buffer[0] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[2] = digitsDec100T[value >> 25];
		              buffer[4] = TEXT('\0');
		return 4;
	}

LENGTH3:
	{
		value = value * ((1 << 25) / 10 + 1);
		*(tchar2_t *)&buffer[0] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 10;
		*(tchar2_t *)&buffer[2] = TO_TCHAR2((value >> 25) + TEXT('0'));
		return 3;
	}

LENGTH2:
	{
		*(tchar2_t *)&buffer[0] = digitsDec100T[value];
		              buffer[2] = TEXT('\0');
		return 2;
	}

LENGTH1:
	{
		*(tchar2_t *)&buffer[0] = TO_TCHAR2(value + TEXT('0'));
		return 1;
	}
}
#else
__declspec(naked) size_t __fastcall _ui32to10t(uint32_t value, TCHAR *buffer)
{
#ifndef _UNICODE
	#define digits        digitsDec100A
	#define movt2         movzx
	#define tchar         byte
	#define tchar2        word
	#define sizeof_tchar2 2
	#define inc_tchar(r)  inc r
	#define t(r)          r##l
	#define t2(r)         r##x
#else
	#define digits        digitsDec100W
	#define movt2         mov
	#define tchar         word
	#define tchar2        dword
	#define sizeof_tchar2 4
	#define inc_tchar(r)  add r, 2
	#define t(r)          r##x
	#define t2(r)         e##r##x
#endif

	__asm
	{
		push    ebx
		mov     ebx, ecx

		cmp     ecx, 1000000
		jb      L2
		cmp     ecx, 100000000
		jb      L1
		cmp     ecx, 1000000000
		jae     LENGTH10
		push    9
		jmp     LENGTH9
	L1:
		cmp     ecx, 10000000
		jae     LENGTH8
		imul    eax, ecx, (1 << 25) / 100000
		shr     ebx, 1
		push    7
		shr     ecx, 5
		add     eax, ebx
		shr     ebx, 6 - 1
		add     eax, ecx
		shr     ecx, 9 - 5
		add     eax, ebx
		shr     ebx, 11 - 6
		sub     eax, ecx
		shr     ecx, 13 - 9
		sub     eax, ebx
		shr     ebx, 15 - 11
		sub     eax, ecx
		add     eax, ebx
		mov     ecx, eax
		jmp     LENGTH7
	L2:
		cmp     ecx, 10000
		jb      L4
		cmp     ecx, 100000
		jb      L3
		imul    eax, ecx, (1 << 25) / 10000
		shr     ebx, 1
		push    6
		shr     ecx, 4
		add     eax, ebx
		shr     ebx, 7 - 1
		sub     eax, ecx
		add     eax, ebx
		mov     ecx, eax
		jmp     LENGTH6
	L3:
		imul    ecx, (1 << 25) / 1000 + 1
		shr     ebx, 2
		push    5
		sub     ecx, ebx
		mov     eax, ecx
		jmp     LENGTH5
	L4:
		cmp     ecx, 100
		jb      L6
		cmp     ecx, 1000
		jb      L5
		imul    ecx, (1 << 25) / 100 + 1
		push    4
		mov     eax, ecx
		jmp     LENGTH4
	L5:
		imul    ecx, (1 << 25) / 10 + 1
		push    3
		mov     eax, ecx
		jmp     LENGTH3
	L6:
		cmp     ecx, 10
		jb      L7
		mov     eax, 2
		jmp     LENGTH2
	L7:
		mov     eax, 1
		jmp     LENGTH1

	LENGTH10:
		add     ecx, -3000000000
		js      L9
		sub     ecx, 1000000000
		js      L8
		mov     tchar ptr [edx], '4'
		jmp     L12
	L8:
		mov     tchar ptr [edx], '3'
		jmp     L11
	L9:
		add     ecx, 1000000000
		js      L10
		mov     tchar ptr [edx], '2'
		jmp     L12
	L10:
		mov     tchar ptr [edx], '1'
	L11:
		add     ecx, 1000000000
	L12:
		push    10
		inc_tchar(edx)
	LENGTH9:
		mov     eax, ecx
		lea     ecx, [ecx + ecx * 2]
		mov     ebx, edx
		mov     edx, 0x5AFE5357	// (0x200000000000000 / 10000000) & 0xFFFFFFFF
		mul     edx
		add     ecx, edx
		mov     edx, ebx
		add     ecx, 3
		mov     eax, ecx
		shr     ecx, 25
		and     eax, 0x01FFFFFF
		imul    eax, 100
		movt2   ebx, tchar2 ptr [digits + ecx * sizeof_tchar2]
		mov     ecx, eax
		mov     tchar2 ptr [edx], t2(b)
		add     edx, sizeof_tchar2
	LENGTH7:
		shr     ecx, 25
		and     eax, 0x01FFFFFF
		imul    eax, 100
		movt2   ebx, tchar2 ptr [digits + ecx * sizeof_tchar2]
		mov     ecx, eax
		mov     tchar2 ptr [edx], t2(b)
		add     edx, sizeof_tchar2
	LENGTH5:
		shr     ecx, 25
		and     eax, 0x01FFFFFF
		imul    eax, 100
		movt2   ebx, tchar2 ptr [digits + ecx * sizeof_tchar2]
		mov     ecx, eax
		mov     tchar2 ptr [edx], t2(b)
		add     edx, sizeof_tchar2
	LENGTH3:
		shr     ecx, 25
		and     eax, 0x01FFFFFF
		movt2   ebx, tchar2 ptr [digits + ecx * sizeof_tchar2]
		lea     ecx, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(b)
		add     edx, sizeof_tchar2
		shr     ecx, 24
		pop     eax
	LENGTH1:
#if __BYTE_ORDER == __LITTLE_ENDIAN
		add     t2(c), '0'
		pop     ebx
		mov     tchar2 ptr [edx], t2(c)
#else
		add     t(c), '0'
		pop     ebx
		mov     tchar ptr [edx], t(c)
		mov     tchar ptr [edx + sizeof_tchar], '\0'
#endif
		ret

	LENGTH8:
		mov     eax, ecx
		push    8
		shl     eax, 5
		shr     ebx, 1
		add     eax, ecx
		shr     ecx, 4
		add     eax, ebx
		shr     ebx, 7 - 1
		add     eax, ecx
		shr     ecx, 12 - 4
		sub     eax, ebx
		shr     ebx, 17 - 7
		sub     eax, ecx
		shr     ecx, 18 - 12
		sub     eax, ebx
		shr     ebx, 21 - 17
		sub     eax, ecx
		shr     ecx, 23 - 18
		add     eax, ebx
		sub     eax, ecx
		mov     ecx, eax
		shr     ecx, 25
		and     eax, 0x01FFFFFF
		imul    eax, 100
		movt2   ebx, tchar2 ptr [digits + ecx * sizeof_tchar2]
		mov     ecx, eax
		mov     tchar2 ptr [edx], t2(b)
		add     edx, sizeof_tchar2
	LENGTH6:
		shr     ecx, 25
		and     eax, 0x01FFFFFF
		imul    eax, 100
		movt2   ebx, tchar2 ptr [digits + ecx * sizeof_tchar2]
		mov     ecx, eax
		mov     tchar2 ptr [edx], t2(b)
		add     edx, sizeof_tchar2
	LENGTH4:
		shr     ecx, 25
		and     eax, 0x01FFFFFF
		movt2   ebx, tchar2 ptr [digits + ecx * sizeof_tchar2]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(b)
		lea     ecx, [eax + eax * 4]
		shr     ecx, 23
		add     edx, sizeof_tchar2
		pop     eax
	LENGTH2:
		movt2   ecx, tchar2 ptr [digits + ecx * sizeof_tchar2]
		pop     ebx
		mov     tchar2 ptr [edx], t2(c)
		mov     tchar ptr [edx + sizeof_tchar2], '\0'
		ret
	}

	#undef digits
	#undef movt2
	#undef tchar
	#undef tchar2
	#undef sizeof_tchar2
	#undef inc_tchar
	#undef t
	#undef t2
}
#endif

size_t __fastcall _ui32to16t(uint32_t value, TCHAR *buffer, BOOL upper)
{
	size_t     length;
	TCHAR      *p;
	const char *digits;

	if (value >= 0x10000u)
		if (value >= 0x1000000u)
			if (value >= 0x10000000u)
				length = 8;
			else
				length = 7;
		else
			if (value >= 0x100000u)
				length = 6;
			else
				length = 5;
	else
		if (value >= 0x100u)
			if (value >= 0x1000u)
				length = 4;
			else
				length = 3;
		else
			if (value >= 0x10u)
				length = 2;
			else
				length = 1;
	p = buffer + length;
	*p = TEXT('\0');
	digits = upper ? digitsHexLarge : digitsHexSmall;
	do
	{
		*(--p) = digits[(size_t)value & 0x0F];
	} while (value >>= 4);
	return length;
}

size_t __fastcall _ui32to8t(uint32_t value, TCHAR *buffer)
{
	size_t length;
	TCHAR  *p;

	if (value >= 010000000u)
		if (value >= 01000000000u)
			if (value >= 010000000000u)
				length = 11;
			else
				length = 10;
		else
			if (value >= 0100000000u)
				length = 9;
			else
				length = 8;
	else
		if (value >= 010000u)
			if (value >= 01000000u)
				length = 7;
			else
				if (value >= 0100000u)
					length = 6;
				else
					length = 5;
		else
			if (value >= 0100u)
				if (value >= 01000u)
					length = 4;
				else
					length = 3;
			else
				if (value >= 010u)
					length = 2;
				else
					length = 1;
	p = buffer + length;
	*p = TEXT('\0');
	do
	{
		*(--p) = ((TCHAR)value & 0x07) + TEXT('0');
	} while (value >>= 3);
	return length;
}
#endif
