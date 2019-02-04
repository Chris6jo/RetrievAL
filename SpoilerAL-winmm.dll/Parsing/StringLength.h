#ifndef _STRINGLENGTH_H_
#define _STRINGLENGTH_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t __stdcall StringLengthA(
	IN OPTIONAL HANDLE hProcess,
	IN          LPCSTR lpString);

size_t __stdcall StringLengthW(
	IN OPTIONAL HANDLE  hProcess,
	IN          LPCWSTR lpString);

#ifdef __cplusplus
}
#endif

#endif	// _STRINGLENGTH_H_
