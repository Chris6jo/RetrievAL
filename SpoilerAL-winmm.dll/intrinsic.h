#ifndef _INTRINSIC_H_
#define _INTRINSIC_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#if defined(_MSC_VER) && _MSC_VER >= 1310
#include <intrin.h>
#endif

#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef unsigned __int64 uint64_t;
#else
typedef unsigned long long uint64_t;
#endif

#ifdef __BORLANDC__
#pragma warn -8027
#pragma warn -8098
#endif

#if (defined(_MSC_VER) && _MSC_VER < 1200) || defined(__BORLANDC__)
#define __forceinline static __inline
#endif

#if (defined(_MSC_VER) && _MSC_VER < 1400) || defined(__BORLANDC__)
#define __restrict
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define CODEPAGE_SUPPORT 0

#define __intrinsic_isleadbyte_cp932(c) \
	((BYTE)(c) >= (BYTE)0x81 && ((BYTE)(c) <= (BYTE)0x9F || ((BYTE)(c) >= (BYTE)0xE0 && (BYTE)(c) <= (BYTE)0xFC)))

#define __intrinsic_istrailbyte_cp932(c) \
	((BYTE)(c) >= (BYTE)0x40 && (BYTE)(c) <= (BYTE)0xFC && (BYTE)(c) != (BYTE)0x7F)

#define case_unsigned_leadbyte_cp932                                                        \
               case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87: \
    case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8E: case 0x8F: \
    case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97: \
    case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D: case 0x9E: case 0x9F: \
    case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xE4: case 0xE5: case 0xE6: case 0xE7: \
    case 0xE8: case 0xE9: case 0xEA: case 0xEB: case 0xEC: case 0xED: case 0xEE: case 0xEF: \
    case 0xF0: case 0xF1: case 0xF2: case 0xF3: case 0xF4: case 0xF5: case 0xF6: case 0xF7: \
    case 0xF8: case 0xF9: case 0xFA: case 0xFB: case 0xFC

#if CODEPAGE_SUPPORT
#define __intrinsic_isleadbyte(c) IsDBCSLeadByteEx(CP_THREAD_ACP, c)
#else
#define __intrinsic_isleadbyte __intrinsic_isleadbyte_cp932
#define __intrinsic_istrailbyte __intrinsic_istrailbyte_cp932
#define case_unsigned_leadbyte case_unsigned_leadbyte_cp932
#endif

#define __intrinsic_isascii(c) \
	((char)(c) >= 0)

#define __intrinsic_isdigit(c) \
	((unsigned)((c) - '0') <= '9' - '0')

#define __intrinsic_isxdigit(c) \
	((char)(c) >= '0' && (BYTE)(c) <= 'f' && ((BYTE)(c) <= '9' || (BYTE)(c) >= 'A' && ((BYTE)(c) <= 'F' || (BYTE)(c) >= 'a')))

#define __intrinsic_isspace(c) \
	((c) == ' ' || (BYTE)(c) <= '\r' && (BYTE)(c) >= '\t')

#define __intrinsic_isblank(c) \
	((c) == ' ' || (c) == '\t')

#define __intrinsic_isspace_without_return(c) \
	(__intrinsic_isblank(c) || (c) == '\v' || (c) == '\f')

#define __intrinsic_isupper(c) \
	((unsigned)((c) - 'A') <= 'Z' - 'A')

#define __intrinsic_islower(c) \
	((unsigned)((c) - 'a') <= 'z' - 'a')

#define __intrinsic_isalpha(c) \
	(__intrinsic_isupper(c) || __intrinsic_islower(c))

#define __intrinsic_isalnum(c) \
	(__intrinsic_isalpha(c) || __intrinsic_isdigit(c))

#define __intrinsic_iscsym(c) \
	(__intrinsic_isalnum(c) || (c) == '_')

#define __intrinsic_iscsymf(c) \
	(__intrinsic_isalpha(c) || (c) == '_')

#define __intrinsic_isprint(c) \
	((char)(c) >= 0x20 && (BYTE)(c) <= 0x7E)

#define __intrinsic_isgraph(c) \
	((char)(c) >= 0x21 && (BYTE)(c) <= 0x7E)

#define __intrinsic_iscntrl(c) \
	((BYTE)(c) <= 0x1F || (c) == 0x7F)

#define __intrinsic_ispunct(c) \
	(__intrinsic_isgraph(c) && !__intrinsic_isalnum(c))

#define __intrinsic_iskana(c) \
	((BYTE)(c) >= 0xA1 && (BYTE)(c) <= 0xDF)

#define __intrinsic_ismbchira(c) \
	((c) >= 0x829F && (c) <= 0x82F1)

#define __intrinsic_ismbckata(c) \
	((c) >= 0x8340 && (c) <= 0x8396)

#define __intrinsic_ismbclegal(c) \
	((c) >= 0x8140 && (c) <= 0xFCFC && ((c) <= 0x9FFC || (c) >= 0xE040) && (BYTE)(c) != 0x7F)

#define __intrinsic_ismbcsymbol(c) \
	((c) >= 0x8141 && (c) <= 0x81AC)

#define __intrinsic_ismbcl0(c) \
	((c) >= 0x8140 && (c) <= 0x889E)

#define __intrinsic_ismbcl1(c) \
	((c) >= 0x889F && (c) <= 0x9872)

#define __intrinsic_ismbcl2(c) \
	((c) >= 0x989F && (c) <= 0xEA9E)

#define __intrinsic_tolower(c) \
	(__intrinsic_isupper(c) ? ((c) + ('a' - 'A')) : (c))

#define __intrinsic_toupper(c) \
	(__intrinsic_islower(c) ? ((c) - ('a' - 'A')) : (c))

#define __intrinsic_toascii(c) \
	((c) & 0x7F)

#define __intrinsic_iswascii(c) \
	((c) <= 0x7F)

#define __intrinsic_iswdigit(c) \
	((unsigned)((c) - L'0') <= L'9' - L'0')

#define __intrinsic_iswxdigit(c) \
	((c) <= 'f' && (c) >= '0' && ((c) <= '9' || (c) >= 'A' && ((c) <= 'F' || (c) >= 'a')))

#define __intrinsic_iswspace(c) \
	((c) == L' ' || ((c) <= L'\r' && (c) >= L'\t'))

#define __intrinsic_iswblank(c) \
	((c) == L' ' || (c) == L'\t')

#define __intrinsic_iswspace_without_return(c) \
	(__intrinsic_iswblank(c) || (c) == L'\v' || (c) == L'\f')

#define __intrinsic_iswupper(c) \
	((unsigned)((c) - L'A') <= L'Z' - L'A')

#define __intrinsic_iswlower(c) \
	((unsigned)((c) - L'a') <= L'z' - L'a')

#define __intrinsic_iswalpha(c) \
	(__intrinsic_iswupper(c) || __intrinsic_iswlower(c))

#define __intrinsic_iswalnum(c) \
	(__intrinsic_iswalpha(c) || __intrinsic_iswdigit(c))

#define __intrinsic_iswcsym(c) \
	(__intrinsic_iswalnum(c) || (c) == L'_')

#define __intrinsic_iswcsymf(c) \
	(__intrinsic_iswalpha(c) || (c) == L'_')

#define __intrinsic_iswprint(c) \
	((c) <= 0x7E && (c) >= 0x20)

#define __intrinsic_iswgraph(c) \
	((c) <= 0x7E && (c) >= 0x21)

#define __intrinsic_iswcntrl(c) \
	((c) <= 0x1F || (c) == 0x7F)

#define __intrinsic_iswpunct(c) \
	(__intrinsic_iswgraph(c) && !__intrinsic_iswalnum(c))

#define __intrinsic_towlower(c) \
	(__intrinsic_iswupper(c) ? ((c) + (L'a' - L'A')) : (c))

#define __intrinsic_towupper(c) \
	(__intrinsic_iswlower(c) ? ((c) - (L'a' - L'A')) : (c))

#define __intrinsic_towascii(c) \
	((c) & 0x7F)

// for constant value
#define BSWAP16(value) (WORD)( \
    ((WORD)(value) >> 8) |     \
    ((WORD)(value) << 8))

// for constant value
#define BSWAP24(value) (DWORD)(             \
    (((DWORD)(value) >> 16) & 0x000000FF) | \
    ( (DWORD)(value)        & 0x0000FF00) | \
    (((DWORD)(value) << 16) & 0x00FF0000))

// for constant value
#define BSWAP32(value) (DWORD)(             \
    ( (DWORD)(value) >> 24              ) | \
    (((DWORD)(value) >>  8) & 0x0000FF00) | \
    (((DWORD)(value) <<  8) & 0x00FF0000) | \
    ( (DWORD)(value) << 24              ))

// for constant value
#define BSWAP40(value) (uint64_t)(                     \
    (((uint64_t)(value) >> 32) & 0x00000000000000FF) | \
    (((uint64_t)(value) >> 16) & 0x000000000000FF00) | \
    ( (uint64_t)(value)        & 0x0000000000FF0000) | \
    (((uint64_t)(value) << 16) & 0x00000000FF000000) | \
    (((uint64_t)(value) << 32) & 0x000000FF00000000))

// for constant value
#define BSWAP48(value) (uint64_t)(                     \
    (((uint64_t)(value) >> 40) & 0x00000000000000FF) | \
    (((uint64_t)(value) >> 24) & 0x000000000000FF00) | \
    (((uint64_t)(value) >>  8) & 0x0000000000FF0000) | \
    (((uint64_t)(value) <<  8) & 0x00000000FF000000) | \
    (((uint64_t)(value) << 24) & 0x000000FF00000000) | \
    (((uint64_t)(value) << 40) & 0x0000FF0000000000))

// for constant value
#define BSWAP56(value) (uint64_t)(                     \
    (((uint64_t)(value) >> 48) & 0x00000000000000FF) | \
    (((uint64_t)(value) >> 32) & 0x000000000000FF00) | \
    (((uint64_t)(value) >> 16) & 0x0000000000FF0000) | \
    ( (uint64_t)(value)        & 0x00000000FF000000) | \
    (((uint64_t)(value) << 16) & 0x000000FF00000000) | \
    (((uint64_t)(value) << 32) & 0x0000FF0000000000) | \
    (((uint64_t)(value) << 48) & 0x00FF000000000000))

// for constant value
#define BSWAP64(value) (uint64_t)(                     \
    ( (uint64_t)(value) >> 56                      ) | \
    (((uint64_t)(value) >> 40) & 0x000000000000FF00) | \
    (((uint64_t)(value) >> 24) & 0x0000000000FF0000) | \
    (((uint64_t)(value) >>  8) & 0x00000000FF000000) | \
    (((uint64_t)(value) <<  8) & 0x000000FF00000000) | \
    (((uint64_t)(value) << 24) & 0x0000FF0000000000) | \
    (((uint64_t)(value) << 40) & 0x00FF000000000000) | \
    ( (uint64_t)(value) << 56                      ))

// for constant value
#define MASM_BSWAP32(value) (            \
    (((value) shr 24) and 0x000000FF) or \
    (((value) shr  8) and 0x0000FF00) or \
    (((value) shl  8) and 0x00FF0000) or \
    (((value) shl 24) and 0xFF000000))

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(_byteswap_ushort)
#pragma intrinsic(_byteswap_ulong)
#pragma intrinsic(_byteswap_uint64)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned short _byteswap_ushort(unsigned short val)
{
	__asm
	{
		mov     ax, word ptr [val]
		xchg    al, ah
	}
}
__forceinline unsigned long _byteswap_ulong(unsigned long val)
{
	__asm
	{
		mov     eax, dword ptr [val]
		bswap   eax
	}
}
__forceinline unsigned __int64 _byteswap_uint64(unsigned __int64 val)
{
	__asm
	{
		mov     edx, dword ptr [val]
		mov     eax, dword ptr [val + 4]
		bswap   edx
		bswap   eax
	}
}
#elif defined(__BORLANDC__)
unsigned short __fastcall _byteswap_ushort(unsigned short val);
unsigned long __fastcall _byteswap_ulong(unsigned long val);
unsigned __int64 __msreturn __fastcall __fastcall_byteswap_uint64(unsigned long high, unsigned long low);
#define _byteswap_uint64(val) __fastcall_byteswap_uint64((unsigned long)((unsigned __int64)(val) >> 32), (unsigned long)(val))
#else
__forceinline unsigned short _byteswap_ushort(unsigned short val) { return BSWAP16 (val); }
__forceinline unsigned long  _byteswap_ulong (unsigned long  val) { return BSWAP32 (val); }
__forceinline uint64_t       _byteswap_uint64(uint64_t       val) { return BSWAP64 (val); }
#endif

#if defined(_MSC_VER) || defined(__BORLANDC__)
#define __intrinsic_bswap16 _byteswap_ushort
#define __intrinsic_bswap24(value) _byteswap_ulong((DWORD)(value) << 8)
#define __intrinsic_bswap32 _byteswap_ulong
#define __intrinsic_bswap40(value) _byteswap_uint64((uint64_t)(value) << 24)
#define __intrinsic_bswap48(value) _byteswap_uint64((uint64_t)(value) << 16)
#define __intrinsic_bswap56(value) _byteswap_uint64((uint64_t)(value) << 8)
#define __intrinsic_bswap64 _byteswap_uint64
#else
__forceinline unsigned short __intrinsic_bswap16(unsigned short value) { return BSWAP16 (value); }
__forceinline unsigned long  __intrinsic_bswap24(unsigned long  value) { return BSWAP24 (value); }
__forceinline unsigned long  __intrinsic_bswap32(unsigned long  value) { return BSWAP32 (value); }
__forceinline uint64_t       __intrinsic_bswap40(uint64_t       value) { return BSWAP40 (value); }
__forceinline uint64_t       __intrinsic_bswap48(uint64_t       value) { return BSWAP48 (value); }
__forceinline uint64_t       __intrinsic_bswap56(uint64_t       value) { return BSWAP56 (value); }
__forceinline uint64_t       __intrinsic_bswap64(uint64_t       value) { return BSWAP64 (value); }
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(_rotl)
#pragma intrinsic(_rotr)
#pragma intrinsic(_rotl64)
#pragma intrinsic(_rotr64)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned int _rotl(unsigned int value, int shift)
{
	__asm
	{
		mov     cl, byte ptr [shift]
		mov     eax, dword ptr [value]
		rol     eax, cl
	}
}
__forceinline unsigned int _rotr(unsigned int value, int shift)
{
	__asm
	{
		mov     cl, byte ptr [shift]
		mov     eax, dword ptr [value]
		ror     eax, cl
	}
}
__forceinline unsigned __int64 _rotl64(unsigned __int64 value, int shift)
{
	__asm
	{
		mov     eax, dword ptr [value]
		mov     edx, dword ptr [value + 4]
		mov     cl, byte ptr [shift]
		mov     edi, eax
		test    cl, 32
		jz      L1
		mov     edi, edx
		mov     edx, eax
		mov     eax, edi
	L1:
		and     cl, 31
		shld    eax, edx, cl
		shld    edx, edi, cl
	}
}
__forceinline unsigned __int64 _rotr64(unsigned __int64 value, int shift)
{
	__asm
	{
		mov     eax, dword ptr [value]
		mov     edx, dword ptr [value + 4]
		mov     cl, byte ptr [shift]
		mov     edi, eax
		test    cl, 32
		jz      L1
		mov     edi, edx
		mov     edx, eax
		mov     eax, edi
	L1:
		and     cl, 31
		shrd    eax, edx, cl
		shrd    edx, edi, cl
	}
}
#elif defined(__BORLANDC__)
unsigned int __fastcall __fastcall_rotl(unsigned int value, int shift);
unsigned int __fastcall __fastcall_rotr(unsigned int value, int shift);
unsigned __int64 __msreturn __fastcall __fastcall_rotl64(DWORD low, DWORD high, int shift);
unsigned __int64 __msreturn __fastcall __fastcall_rotr64(DWORD low, DWORD high, int shift);
#define _rotl __fastcall_rotl
#define _rotr __fastcall_rotr
#define _rotl64(value, shift) __fastcall_rotl64((DWORD)(value), (DWORD)((uint64_t)(value) >> 32), shift)
#define _rotr64(value, shift) __fastcall_rotr64((DWORD)(value), (DWORD)((uint64_t)(value) >> 32), shift)
#else
#define _rotl(value, shift) (((unsigned int)(value) << (shift)) | ((unsigned int)(value) >> (32 - (shift))))
#define _rotr(value, shift) (((unsigned int)(value) >> (shift)) | ((unsigned int)(value) << (32 - (shift))))
#define _rotl64(value, shift) (((uint64_t)(value) << (shift)) | ((uint64_t)(value) >> (64 - (shift))))
#define _rotr64(value, shift) (((uint64_t)(value) >> (shift)) | ((uint64_t)(value) << (64 - (shift))))
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(__movsb)
#pragma intrinsic(__movsw)
#pragma intrinsic(__movsd)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
#define __movsb(Destination, Source, Count)                     \
do                                                              \
{                                                               \
    unsigned char *      __restrict _Destination = Destination; \
    const unsigned char *__restrict _Source      = Source;      \
    size_t                          _Count       = Count;       \
                                                                \
    __asm   mov     ecx, dword ptr [_Count]                     \
    __asm   mov     esi, dword ptr [_Source]                    \
    __asm   mov     edi, dword ptr [_Destination]               \
    __asm   rep movsb                                           \
} while (0)
#define __movsw(Destination, Source, Count)                      \
do                                                               \
{                                                                \
    unsigned short *      __restrict _Destination = Destination; \
    const unsigned short *__restrict _Source      = Source;      \
    size_t                           _Count       = Count;       \
                                                                 \
    __asm   mov     ecx, dword ptr [_Count]                      \
    __asm   mov     esi, dword ptr [_Source]                     \
    __asm   mov     edi, dword ptr [_Destination]                \
    __asm   rep movsw                                            \
} while (0)
#define __movsd(Destination, Source, Count)                     \
do                                                              \
{                                                               \
    unsigned long *      __restrict _Destination = Destination; \
    const unsigned long *__restrict _Source      = Source;      \
    size_t                          _Count       = Count;       \
                                                                \
    __asm   mov     ecx, dword ptr [_Count]                     \
    __asm   mov     esi, dword ptr [_Source]                    \
    __asm   mov     edi, dword ptr [_Destination]               \
    __asm   rep movsd                                           \
} while (0)
#elif defined(__BORLANDC__)
void __fastcall __movsb(unsigned char *Destination, const unsigned char *Source, size_t Count);
void __fastcall __movsw(unsigned short *Destination, const unsigned short *Source, size_t Count);
void __fastcall __movsd(unsigned long *Destination, const unsigned long *Source, size_t Count);
#else
#define __movsb memcpy
#define __movsw(Destination, Source, Count) memcpy(Destination, Source, (size_t)(Count) * 2)
#define __movsd(Destination, Source, Count) memcpy(Destination, Source, (size_t)(Count) * 4)
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(__stosb)
#pragma intrinsic(__stosw)
#pragma intrinsic(__stosd)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
#define __stosb(Dest, Data, Count)            \
do                                            \
{                                             \
    unsigned char *__restrict _Dest  = Dest;  \
    unsigned char             _Data  = Data;  \
    size_t                    _Count = Count; \
                                              \
    __asm   mov     ecx, dword ptr [_Count]   \
    __asm   mov     al, byte ptr [_Data]      \
    __asm   mov     edi, dword ptr [_Dest]    \
    __asm   rep stosb                         \
} while (0)
#define __stosw(Dest, Data, Count)             \
do                                             \
{                                              \
    unsigned short *__restrict _Dest  = Dest;  \
    unsigned short             _Data  = Data;  \
    size_t                     _Count = Count; \
                                               \
    __asm   mov     ecx, dword ptr [_Count]    \
    __asm   mov     ax, word ptr [_Data]       \
    __asm   mov     edi, dword ptr [_Dest]     \
    __asm   rep stosw                          \
} while (0)
#define __stosd(Dest, Data, Count)            \
do                                            \
{                                             \
    unsigned long *__restrict _Dest  = Dest;  \
    unsigned long             _Data  = Data;  \
    size_t                    _Count = Count; \
                                              \
    __asm   mov     ecx, dword ptr [_Count]   \
    __asm   mov     eax, dword ptr [_Data]    \
    __asm   mov     edi, dword ptr [_Dest]    \
    __asm   rep stosd                         \
} while (0)
#elif defined(__BORLANDC__)
__declspec(naked) void __fastcall __fastcall_stosb(unsigned char Data, unsigned char *Dest, size_t Count);
__declspec(naked) void __fastcall __fastcall_stosw(unsigned short Data, unsigned short *Dest, size_t Count);
__declspec(naked) void __fastcall __fastcall_stosd(unsigned long Data, unsigned long *Dest, size_t Count);
#define __stosb(Dest, Data, Count) __fastcall_stosb(Data, Dest, Count)
#define __stosw(Dest, Data, Count) __fastcall_stosw(Data, Dest, Count)
#define __stosd(Dest, Data, Count) __fastcall_stosd(Data, Dest, Count)
#else
#define __stosb memset
#define __stosw(Dest, Data, Count)             \
do                                             \
{                                              \
    unsigned short *__restrict _Dest  = Dest;  \
    unsigned short             _Data  = Data;  \
    size_t                     _Count = Count; \
                                               \
    while (_Count--)                           \
        *(_Dest++) = _Data;                    \
} while (0)
#define __stosd(Dest, Data, Count)            \
do                                            \
{                                             \
    unsigned long *__restrict _Dest  = Dest;  \
    unsigned long             _Data  = Data;  \
    size_t                    _Count = Count; \
                                              \
    while (_Count--)                          \
        *(_Dest++) = _Data;                   \
} while (0)
#endif

#if defined(_MSC_VER) && defined(_M_X64)
#pragma intrinsic(__stosq)
#else
#define __stosq(Dest, Data, Count)               \
do                                               \
{                                                \
    unsigned __int64 *__restrict _Dest  = Dest;  \
    unsigned __int64             _Data  = Data;  \
    size_t                       _Count = Count; \
                                                 \
    while (_Count--)                             \
        *(_Dest++) = _Data;                      \
} while (0)
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(__emulu)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned __int64 __emulu(unsigned int a, unsigned int b)
{
	__asm
	{
		mov     eax, dword ptr [a]
		mov     edx, dword ptr [b]
		mul     edx
	}
}
#elif defined(__BORLANDC__)
unsigned __int64 __msreturn __fastcall __emulu(unsigned int a, unsigned int b);
#else
#define __emulu(a, b) ((uint64_t)(unsigned int)(a) * (unsigned int)(b))
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1920
#pragma intrinsic(_udiv64)
#elif defined(_MSC_VER) && _MSC_VER < 1920 && defined(_M_IX86)
__forceinline unsigned int __ui64return_udiv64(unsigned __int64 dividend, unsigned int divisor)
{
	__asm
	{
		mov     eax, dword ptr [dividend]
		mov     edx, dword ptr [dividend + 4]
		mov     ecx, dword ptr [divisor]
		div     ecx
	}
}
__forceinline unsigned int _udiv64(unsigned __int64 dividend, unsigned int divisor, unsigned int *remainder)
{
	unsigned __int64 x = __ui64return_udiv64(dividend, divisor);
	*remainder = x >> 32;
	return (unsigned int)x;
}
#elif defined(__BORLANDC__)
unsigned int __fastcall __fastcall_udiv64(DWORD low, DWORD high, unsigned int divisor, unsigned int *remainder);
unsigned int __fastcall __fastcall__udiv64(DWORD low, DWORD high, unsigned int divisor);
#define _udiv64(dividend, divisor, remainder) __fastcall_udiv64((DWORD)(dividend), (DWORD)((uint64_t)(dividend) >> 32), divisor, remainder)
x#else
__forceinline unsigned int _udiv64(unsigned __int64 dividend, unsigned int divisor, unsigned int *remainder)
{
	*remainder = dividend % divisor;
	return (unsigned int)(dividend / divisor);
}
#endif

// for constant value
#define _BSF8(x, default) (  \
    ((x) & 0x01) ?  0 :     \
    ((x) & 0x02) ?  1 :     \
    ((x) & 0x04) ?  2 :     \
    ((x) & 0x08) ?  3 :     \
    ((x) & 0x10) ?  4 :     \
    ((x) & 0x20) ?  5 :     \
    ((x) & 0x40) ?  6 :     \
    ((x) & 0x80) ?  7 :     \
    (default))
#define _BSF16(x, default) _BSF8(x, _BSF8((x) >> 8, (default) - 8) + 8)
#define _BSF32(x, default) _BSF16(x, _BSF16((x) >> 16, (default) - 16) + 16)
#define _BSF64(x, default) _BSF32(x, _BSF32((x) >> 32, (default) - 32) + 32)
#define BSF8(x) _BSF8(x, -1)
#define BSF16(x) _BSF16(x, -1)
#define BSF32(x) _BSF32(x, -1)
#define BSF64(x) _BSF64(x, -1)

// for constant value
#define MASM_BSF32(x) (                                               -1 + \
    (  (x) and 0x00000001                                         )      + \
    ((((x) and 0x00000002) shr  1) and (((x) and 0x00000001) eq 0)) *  2 + \
    ((((x) and 0x00000004) shr  2) and (((x) and 0x00000003) eq 0)) *  3 + \
    ((((x) and 0x00000008) shr  3) and (((x) and 0x00000007) eq 0)) *  4 + \
    ((((x) and 0x00000010) shr  4) and (((x) and 0x0000000F) eq 0)) *  5 + \
    ((((x) and 0x00000020) shr  5) and (((x) and 0x0000001F) eq 0)) *  6 + \
    ((((x) and 0x00000040) shr  6) and (((x) and 0x0000003F) eq 0)) *  7 + \
    ((((x) and 0x00000080) shr  7) and (((x) and 0x0000007F) eq 0)) *  8 + \
    ((((x) and 0x00000100) shr  8) and (((x) and 0x000000FF) eq 0)) *  9 + \
    ((((x) and 0x00000200) shr  9) and (((x) and 0x000001FF) eq 0)) * 10 + \
    ((((x) and 0x00000400) shr 10) and (((x) and 0x000003FF) eq 0)) * 11 + \
    ((((x) and 0x00000800) shr 11) and (((x) and 0x000007FF) eq 0)) * 12 + \
    ((((x) and 0x00001000) shr 12) and (((x) and 0x00000FFF) eq 0)) * 13 + \
    ((((x) and 0x00002000) shr 13) and (((x) and 0x00001FFF) eq 0)) * 14 + \
    ((((x) and 0x00004000) shr 14) and (((x) and 0x00003FFF) eq 0)) * 15 + \
    ((((x) and 0x00008000) shr 15) and (((x) and 0x00007FFF) eq 0)) * 16 + \
    ((((x) and 0x00010000) shr 16) and (((x) and 0x0000FFFF) eq 0)) * 17 + \
    ((((x) and 0x00020000) shr 17) and (((x) and 0x0001FFFF) eq 0)) * 18 + \
    ((((x) and 0x00040000) shr 18) and (((x) and 0x0003FFFF) eq 0)) * 19 + \
    ((((x) and 0x00080000) shr 19) and (((x) and 0x0007FFFF) eq 0)) * 20 + \
    ((((x) and 0x00100000) shr 20) and (((x) and 0x000FFFFF) eq 0)) * 21 + \
    ((((x) and 0x00200000) shr 21) and (((x) and 0x001FFFFF) eq 0)) * 22 + \
    ((((x) and 0x00400000) shr 22) and (((x) and 0x003FFFFF) eq 0)) * 23 + \
    ((((x) and 0x00800000) shr 23) and (((x) and 0x007FFFFF) eq 0)) * 24 + \
    ((((x) and 0x01000000) shr 24) and (((x) and 0x00FFFFFF) eq 0)) * 25 + \
    ((((x) and 0x02000000) shr 25) and (((x) and 0x01FFFFFF) eq 0)) * 26 + \
    ((((x) and 0x04000000) shr 26) and (((x) and 0x03FFFFFF) eq 0)) * 27 + \
    ((((x) and 0x08000000) shr 27) and (((x) and 0x07FFFFFF) eq 0)) * 28 + \
    ((((x) and 0x10000000) shr 28) and (((x) and 0x0FFFFFFF) eq 0)) * 29 + \
    ((((x) and 0x20000000) shr 29) and (((x) and 0x1FFFFFFF) eq 0)) * 30 + \
    ((((x) and 0x40000000) shr 30) and (((x) and 0x3FFFFFFF) eq 0)) * 31 + \
    ((((x) and 0x80000000) shr 31) and (((x) and 0x7FFFFFFF) eq 0)) * 32)

// for constant value
#define _BSR8(x, default) ( \
    ((x) & 0x80) ?  7 :     \
    ((x) & 0x40) ?  6 :     \
    ((x) & 0x20) ?  5 :     \
    ((x) & 0x10) ?  4 :     \
    ((x) & 0x08) ?  3 :     \
    ((x) & 0x04) ?  2 :     \
    ((x) & 0x02) ?  1 :     \
    ((x) & 0x01) ?  0 :     \
    (default))
#define _BSR16(x, default) (_BSR8((x) >> 8, _BSR8(x, default) - 8) + 8)
#define _BSR32(x, default) (_BSR16((x) >> 16, _BSR16(x, default) - 16) + 16)
#define _BSR64(x, default) (_BSR32((x) >> 32, _BSR32(x, default) - 32) + 32)
#define BSR8(x) _BSR8(x, -1)
#define BSR16(x) _BSR16(x, -1)
#define BSR32(x) _BSR32(x, -1)
#define BSR64(x) _BSR64(x, -1)

// for constant value
#define MASM_BSR32(x) (                                               -1 + \
    ((((x) and 0x80000000) shr 31) and 1                          ) * 32 + \
    ((((x) and 0x40000000) shr 30) and (((x) and 0x80000000) eq 0)) * 31 + \
    ((((x) and 0x20000000) shr 29) and (((x) and 0xC0000000) eq 0)) * 30 + \
    ((((x) and 0x10000000) shr 28) and (((x) and 0xE0000000) eq 0)) * 29 + \
    ((((x) and 0x08000000) shr 27) and (((x) and 0xF0000000) eq 0)) * 28 + \
    ((((x) and 0x04000000) shr 26) and (((x) and 0xF8000000) eq 0)) * 27 + \
    ((((x) and 0x02000000) shr 25) and (((x) and 0xFC000000) eq 0)) * 26 + \
    ((((x) and 0x01000000) shr 24) and (((x) and 0xFE000000) eq 0)) * 25 + \
    ((((x) and 0x00800000) shr 23) and (((x) and 0xFF000000) eq 0)) * 24 + \
    ((((x) and 0x00400000) shr 22) and (((x) and 0xFF800000) eq 0)) * 23 + \
    ((((x) and 0x00200000) shr 21) and (((x) and 0xFFC00000) eq 0)) * 22 + \
    ((((x) and 0x00100000) shr 20) and (((x) and 0xFFE00000) eq 0)) * 21 + \
    ((((x) and 0x00080000) shr 19) and (((x) and 0xFFF00000) eq 0)) * 20 + \
    ((((x) and 0x00040000) shr 18) and (((x) and 0xFFF80000) eq 0)) * 19 + \
    ((((x) and 0x00020000) shr 17) and (((x) and 0xFFFC0000) eq 0)) * 18 + \
    ((((x) and 0x00010000) shr 16) and (((x) and 0xFFFE0000) eq 0)) * 17 + \
    ((((x) and 0x00008000) shr 15) and (((x) and 0xFFFF0000) eq 0)) * 16 + \
    ((((x) and 0x00004000) shr 14) and (((x) and 0xFFFF8000) eq 0)) * 15 + \
    ((((x) and 0x00002000) shr 13) and (((x) and 0xFFFFC000) eq 0)) * 14 + \
    ((((x) and 0x00001000) shr 12) and (((x) and 0xFFFFE000) eq 0)) * 13 + \
    ((((x) and 0x00000800) shr 11) and (((x) and 0xFFFFF000) eq 0)) * 12 + \
    ((((x) and 0x00000400) shr 10) and (((x) and 0xFFFFF800) eq 0)) * 11 + \
    ((((x) and 0x00000200) shr  9) and (((x) and 0xFFFFFC00) eq 0)) * 10 + \
    ((((x) and 0x00000100) shr  8) and (((x) and 0xFFFFFE00) eq 0)) *  9 + \
    ((((x) and 0x00000080) shr  7) and (((x) and 0xFFFFFF00) eq 0)) *  8 + \
    ((((x) and 0x00000040) shr  6) and (((x) and 0xFFFFFF80) eq 0)) *  7 + \
    ((((x) and 0x00000020) shr  5) and (((x) and 0xFFFFFFC0) eq 0)) *  6 + \
    ((((x) and 0x00000010) shr  4) and (((x) and 0xFFFFFFE0) eq 0)) *  5 + \
    ((((x) and 0x00000008) shr  3) and (((x) and 0xFFFFFFF0) eq 0)) *  4 + \
    ((((x) and 0x00000004) shr  2) and (((x) and 0xFFFFFFF8) eq 0)) *  3 + \
    ((((x) and 0x00000002) shr  1) and (((x) and 0xFFFFFFFC) eq 0)) *  2 + \
    (( (x) and 0x00000001        ) and (((x) and 0xFFFFFFFE) eq 0)))

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanReverse)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned __int64 __ui64return_BitScanForward(unsigned long Mask)
{
	__asm
	{
		bsf     edx, dword ptr [Mask]
		setnz   al
	}
}
__forceinline unsigned char _BitScanForward(unsigned long *Index, unsigned long Mask)
{
	unsigned __int64 x = __ui64return_BitScanForward(Mask);
	*Index = x >> 32;
	return (unsigned char)x;
}
__forceinline unsigned __int64 __ui64return_BitScanReverse(unsigned long Mask)
{
	__asm
	{
		bsr     edx, dword ptr [Mask]
		setnz   al
	}
}
__forceinline unsigned char _BitScanReverse(unsigned long *Index, unsigned long Mask)
{
	unsigned __int64 x = __ui64return_BitScanReverse(Mask);
	*Index = x >> 32;
	return (unsigned char)x;
}
#elif defined(__BORLANDC__)
unsigned char __fastcall _BitScanForward(unsigned long *Index, unsigned long Mask);
unsigned char __fastcall _BitScanReverse(unsigned long *Index, unsigned long Mask);
#else
__forceinline unsigned char _BitScanForward(unsigned long *Index, unsigned long Mask)
{
	if (Mask)
	{
		unsigned long i;

		for (i = 0; !(Mask & 1); Mask >>= 1)
			i++;
		*Index = i;
		return 1;
	}
	else
	{
		return 0;
	}
}
__forceinline unsigned char _BitScanReverse(unsigned long *Index, unsigned long Mask)
{
	if (Mask)
	{
		unsigned long i;

		for (i = 31; (long)Mask >= 0; Mask <<= 1)
			i--;
		*Index = i;
		return 1;
	}
	else
	{
		return 0;
	}
}
#endif

#if defined(_MSC_VER) && defined(_M_X64)
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(_BitScanReverse64)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned __int64 __ui64return_BitScanForward64(unsigned __int64 Mask)
{
	__asm
	{
		bsf     edx, dword ptr [Mask]
		jnz     L1
		bsf     edx, dword ptr [Mask + 4]
		lea     edx, [edx + 32]
	L1:
		setnz   al
	}
}
__forceinline unsigned char _BitScanForward64(unsigned long *Index, unsigned __int64 Mask)
{
	unsigned __int64 x = __ui64return_BitScanForward64(Mask);
	*Index = x >> 32;
	return (unsigned char)x;
}
__forceinline unsigned __int64 __ui64return_BitScanReverse64(unsigned __int64 Mask)
{
	__asm
	{
		bsr     edx, dword ptr [Mask + 4]
		lea     edx, [edx + 32]
		jnz     L1
		bsr     edx, dword ptr [Mask]
	L1:
		setnz   al
	}
}
__forceinline unsigned char _BitScanReverse64(unsigned long *Index, unsigned __int64 Mask)
{
	unsigned __int64 x = __ui64return_BitScanReverse64(Mask);
	*Index = x >> 32;
	return (unsigned char)x;
}
#elif defined(__BORLANDC__)
unsigned char __fastcall __fastcall_BitScanForward64(DWORD low, DWORD high, unsigned long *Index);
unsigned char __fastcall __fastcall_BitScanReverse64(DWORD low, DWORD high, unsigned long *Index);
#define _BitScanForward64(Index, Mask) __fastcall_BitScanForward64((DWORD)(Mask), (DWORD)((uint64_t)(Mask) >> 32), Index)
#define _BitScanReverse64(Index, Mask) __fastcall_BitScanReverse64((DWORD)(Mask), (DWORD)((uint64_t)(Mask) >> 32), Index)
#else
__forceinline unsigned char _BitScanForward64(unsigned long *Index, uint64_t Mask)
{
	unsigned char Result;

	if (!(Result = _BitScanForward(Index, (unsigned long)Mask)))
		if (Result = _BitScanForward(Index, (unsigned long)(Mask >> 32)))
			*Index += 32;
	return Result;
}
__forceinline unsigned char _BitScanReverse64(unsigned long *Index, uint64_t Mask)
{
	unsigned char Result;

	if (Result = _BitScanReverse(Index, (unsigned long)(Mask >> 32)))
		*Index += 32;
	else
		Result = _BitScanReverse(Index, (unsigned long)Mask);
	return Result;
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(_addcarry_u32)
#define _add_u32(a, b, out) _addcarry_u32(0, a, b, out)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned __int64 __ui64return_add_u32(unsigned int a, unsigned int b)
{
	__asm
	{
		mov     edx, dword ptr [a]
		mov     eax, dword ptr [b]
		add     edx, eax
		setc    al
	}
}
__forceinline unsigned char _add_u32(unsigned int a, unsigned int b, unsigned int *out)
{
	unsigned __int64 x = __ui64return_add_u32(a, b);
	*out = x >> 32;
	return (unsigned char)x;
}
__forceinline unsigned __int64 __ui64return_addcarry_u32(unsigned char c_in, unsigned int a, unsigned int b)
{
	__asm
	{
		mov     al, byte ptr [c_in]
		mov     edx, dword ptr [a]
		add     al, -1
		mov     ecx, dword ptr [b]
		adc     edx, ecx
		setc    al
	}
}
__forceinline unsigned char __addcarry_u32(unsigned char c_in, unsigned int a, unsigned int b, unsigned int *out)
{
	unsigned __int64 x = __ui64return_addcarry_u32(c_in, a, b);
	*out = x >> 32;
	return (unsigned char)x;
}
#elif defined(__BORLANDC__)
unsigned char __fastcall _add_u32(unsigned int a, unsigned int b, unsigned int *out);
unsigned char __fastcall _addcarry_u32(unsigned char c_in, unsigned int a, unsigned int b, unsigned int *out);
#else
__forceinline unsigned char _add_u32(unsigned int a, unsigned int b, unsigned int *out)
{
	return (*out = a + b) < b;
}
__forceinline unsigned char _addcarry_u32(unsigned char c_in, unsigned int a, unsigned int b, unsigned int *out)
{
	return ((*out = a + b) < b) | (c_in && !++(*out));
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(_subborrow_u32)
#define _sub_u32(a, b, out) _subborrow_u32(0, a, b, out)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned __int64 __ui64return_sub_u32(unsigned int a, unsigned int b)
{
	__asm
	{
		mov     edx, dword ptr [a]
		mov     eax, dword ptr [b]
		sub     edx, eax
		setc    al
	}
}
__forceinline unsigned char _sub_u32(unsigned int a, unsigned int b, unsigned int *out)
{
	unsigned __int64 x = __ui64return_sub_u32(a, b);
	*out = x >> 32;
	return (unsigned char)x;
}
__forceinline unsigned __int64 __ui64return_subborrow_u32(unsigned char b_in, unsigned int a, unsigned int b)
{
	__asm
	{
		mov     al, byte ptr [b_in]
		mov     edx, dword ptr [a]
		add     al, -1
		mov     ecx, dword ptr [b]
		sbb     edx, ecx
		setc    al
	}
}
__forceinline unsigned char _subborrow_u32(unsigned char b_in, unsigned int a, unsigned int b, unsigned int *out)
{
	unsigned __int64 x = __ui64return_subborrow_u32(b_in, a, b);
	*out = x >> 32;
	return (unsigned char)x;
}
#elif defined(__BORLANDC__)
unsigned char __fastcall _sub_u32(unsigned int a, unsigned int b, unsigned int *out);
unsigned char __fastcall _subborrow_u32(unsigned char b_in, unsigned int a, unsigned int b, unsigned int *out);
#else
__forceinline unsigned char _sub_u32(unsigned int a, unsigned int b, unsigned int *out)
{
	return (*out = a - b) > a;
}
__forceinline unsigned char _subborrow_u32(unsigned char b_in, unsigned int a, unsigned int b, unsigned int *out)
{
	return ((*out = a - b) > a) | (b_in && !(*out)--);
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310 && defined(_WIN64)
#pragma intrinsic(_addcarry_u64)
#define _add_u64(a, b, out) _addcarry_u64(0, a, b, out)
#else
__forceinline unsigned char _add_u64(uint64_t a, uint64_t b, uint64_t *out)
{
	return (*out = a + b) < b;
}
__forceinline unsigned char _addcarry_u64(unsigned char c_in, uint64_t a, uint64_t b, uint64_t *out)
{
	return ((*out = a + b) < b) | (c_in && !++(*out));
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310 && defined(_WIN64)
#pragma intrinsic(_subborrow_u64)
#define _sub_u64(a, b, out) _subborrow_u64(0, a, b, out)
#else
__forceinline unsigned char _sub_u64(uint64_t a, uint64_t b, uint64_t *out)
{
	return (*out = a - b) > a;
}
__forceinline unsigned char _subborrow_u64(unsigned char b_in, uint64_t a, uint64_t b, uint64_t *out)
{
	return ((*out = a - b) > a) | (b_in && !(*out)--);
}
#endif

#ifndef _WIN64
#define _add_uintptr       _add_u32
#define _addborrow_uintptr _addborrow_u32
#define _sub_uintptr       _sub_u32
#define _subborrow_uintptr _subborrow_u32
#else
#define _add_uintptr       _add_u64
#define _addborrow_uintptr _addborrow_u64
#define _sub_uintptr       _sub_u64
#define _subborrow_uintptr _subborrow_u64
#endif

#ifdef __cplusplus
}
#endif

#endif	// _INTRINSIC_H_
