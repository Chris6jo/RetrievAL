#ifndef _ISBADPTR_H_
#define _ISBADPTR_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL __stdcall IsBadReadPtrCompatible(IN LPCVOID lp, IN UINT_PTR ucb);
BOOL __stdcall IsBadWritePtrCompatible(IN LPVOID lp, IN UINT_PTR ucb);
BOOL __stdcall IsBadStringPtrCompatibleA(IN LPCSTR lpsz, IN UINT ucchMax);
BOOL __stdcall IsBadStringPtrCompatibleW(IN LPCWSTR lpsz, IN UINT ucchMax);

#define IsBadReadPtr    IsBadReadPtrCompatible
#define IsBadWritePtr   IsBadWritePtrCompatible
#define IsBadStringPtrA IsBadStringPtrCompatibleA
#define IsBadStringPtrW IsBadStringPtrCompatibleW

#ifdef __cplusplus
}
#endif

#endif	// _ISBADPTR_H_
