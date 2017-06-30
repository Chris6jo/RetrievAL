#pragma once

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t __fastcall _ui32to10a(unsigned __int32 value, char *buffer);
size_t __fastcall _ui32to10w(unsigned __int32 value, wchar_t *buffer);
size_t __fastcall _ui32to16a(unsigned __int32 value, char *buffer, BOOL upper);
size_t __fastcall _ui32to16w(unsigned __int32 value, wchar_t *buffer, BOOL upper);
size_t __fastcall _ui32to8a(unsigned __int32 value, char *buffer);
size_t __fastcall _ui32to8w(unsigned __int32 value, wchar_t *buffer);

#ifdef __cplusplus
}
#endif