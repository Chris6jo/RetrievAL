#ifndef _ENDIANNESS_H_
#define _ENDIANNESS_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __has_include // C++17, supported as extension to C++11 in Clang, GCC 5+, VS2015
#if __has_include(<endian.h>)
#include <endian.h> // GNU libc normally provides, Linux
#elif __has_include(<machine/endian.h>)
#include <machine/endian.h> // OpenBSD, macos
#elif __has_include(<sys/param.h>)
#include <sys/param.h> // MinGW, some bsd (not open/macos)
#elif __has_include(<sys/isadefs.h>)
#include <sys/isadefs.h> // Solaris
#endif
#endif

#ifndef __ORDER_BIG_ENDIAN__
#define __ORDER_BIG_ENDIAN__ 4321
#endif
#ifndef __ORDER_LITTLE_ENDIAN__
#define __ORDER_LITTLE_ENDIAN__ 1234
#endif
#ifndef __ORDER_PDP_ENDIAN__
#define __ORDER_PDP_ENDIAN__ 3412
#endif

#ifndef __BYTE_ORDER__
#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || \
    defined(_BYTE_ORDER) && _BYTE_ORDER == _BIG_ENDIAN || \
    defined(BYTE_ORDER) && BYTE_ORDER == BIG_ENDIAN || \
    defined(__sun) && defined(__SVR4) && defined(_BIG_ENDIAN) || \
    defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || \
    defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__) || \
    defined(_M_PPC)
#define __BYTE_ORDER__ __ORDER_BIG_ENDIAN__
#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || /* Linux header */ \
    defined(_BYTE_ORDER) && _BYTE_ORDER == _LITTLE_ENDIAN || \
    defined(BYTE_ORDER) && BYTE_ORDER == LITTLE_ENDIAN || /* MinGW header */ \
    defined(__sun) && defined(__SVR4) && defined(_LITTLE_ENDIAN) || /* Solaris */ \
    defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || \
    defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__) || \
    defined(_M_IX86) || defined(_M_X64) || defined(_M_IA64) || /* MSVC for intel processors */ \
    defined(_M_ARM) /* MSVC code on arm executes in little endian mode */
#define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __PDP_ENDIAN || \
    defined(_BYTE_ORDER) && _BYTE_ORDER == _PDP_ENDIAN || \
    defined(BYTE_ORDER) && BYTE_ORDER == PDP_ENDIAN
#define __BYTE_ORDER__ __ORDER_PDP_ENDIAN__
#else
#include <limits.h>
#if CHAR_BIT == 8
#define __BYTE_ORDER__ (              \
    ( '\4\3\2\1'        & 7)        + \
    (('\4\3\2\1' >>  8) & 7) *   10 + \
    (('\4\3\2\1' >> 16) & 7) *  100 + \
    ( '\4\3\2\1' >> 24     ) * 1000)
#endif
#endif
#endif

#if defined(__BYTE_ORDER__) && !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define __BIG_ENDIAN__ 1
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__ 1
#endif
#endif

#endif //_ENDIANNESS_H_
