#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
TCHAR * __cdecl _tcsncpy(TCHAR *string1, const TCHAR *string2, size_t count)
{
	if (count)
	{
		size_t length;

		memcpy(string1, string2, (length = _tcsnlen(string2, count)) * sizeof(TCHAR));
		memset(string1 + length, 0, (count - length) * sizeof(TCHAR));
	}
	return string1;
}
#else
#pragma function(memcpy, memset)
__declspec(naked) TCHAR * __cdecl _tcsncpy(TCHAR *string1, const TCHAR *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		mov     ecx, dword ptr [count]                  // ecx = count
		mov     eax, dword ptr [string2]                // eax = src
		test    ecx, ecx
		jz      L1                                      // jump if count == 0
		push    ecx
		push    eax
		call    _tcsnlen
		mov     ecx, dword ptr [count + 8]              // ecx = count
		mov     edx, dword ptr [string1 + 8]            // edx = dest
		sub     ecx, eax                                // ecx = count - length
		sub     esp, 4
#ifdef _UNICODE
		add     ecx, ecx
		add     eax, eax
#endif
		mov     dword ptr [esp + 8], ecx                // store memset 3rd param
		push    eax                                     // store memcpy 3rd param
		add     eax, edx                                // eax = dest + length
		mov     ecx, dword ptr [string2 + 16]           // ecx = src
		mov     dword ptr [esp + 8], 0                  // store memset 2nd param
		mov     dword ptr [esp + 4], eax                // store memset 1st param
		push    ecx                                     // store memcpy 2nd param
		push    edx                                     // store memcpy 1st param
		call    memcpy
		add     esp, 12
		call    memset
		mov     eax, dword ptr [string1 + 12]           // return dest
		add     esp, 12
		ret

		align   16
	L1:
		mov     eax, dword ptr [string1]
		ret

		#undef string1
		#undef string2
		#undef count
	}
}
#endif
