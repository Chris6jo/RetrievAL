#ifndef _UI64TON_H_
#define _UI64TON_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t __fastcall _ui64to10a(unsigned __int64 value, char *buffer);
size_t __fastcall _ui64to10w(unsigned __int64 value, wchar_t *buffer);
size_t __fastcall _ui64to2a(unsigned __int64 value, char *buffer);
size_t __fastcall _ui64to2w(unsigned __int64 value, wchar_t *buffer);
size_t __fastcall _ui64to4a(unsigned __int64 value, char *buffer);
size_t __fastcall _ui64to4w(unsigned __int64 value, wchar_t *buffer);
size_t __fastcall _ui64to8a(unsigned __int64 value, char *buffer);
size_t __fastcall _ui64to8w(unsigned __int64 value, wchar_t *buffer);
size_t __fastcall _ui64to16a(unsigned __int64 value, char *buffer, BOOL upper);
size_t __fastcall _ui64to16w(unsigned __int64 value, wchar_t *buffer, BOOL upper);
size_t __fastcall _ui64to32a(unsigned __int64 value, char *buffer, BOOL upper);
size_t __fastcall _ui64to32w(unsigned __int64 value, wchar_t *buffer, BOOL upper);

#ifdef __cplusplus
}
#endif

#endif	// _UI64TON_H_
