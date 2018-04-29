#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
TCHAR * __cdecl _tcslcpy(TCHAR *string1, const TCHAR *string2, size_t count)
{
	if (!count)
		return string1;
	string1[count = _tcsnlen(string2, count - 1)] = '\0';
	return (TCHAR *)memcpy(string1, string2, count * sizeof(TCHAR));
}
#else
#pragma function(memcpy)
__declspec(naked) TCHAR * __cdecl _tcslcpy(TCHAR *string1, const TCHAR *string2, size_t count)
{
	__asm
	{
#ifdef _UNICODE
		#define tchar_ptr word ptr
#else
		#define tchar_ptr byte ptr
#endif

		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		mov     eax, dword ptr [count]
		mov     ecx, dword ptr [string2]
		sub     eax, 1
		jb      L1
		push    eax
		push    ecx
		call    _tcsnlen
#ifdef _UNICODE
		add     eax, eax
		mov     ecx, dword ptr [string1 + 8]
		mov     dword ptr [count + 8], eax
		add     esp, 8
#else
		mov     ecx, dword ptr [string1 + 8]
		add     esp, 8
		mov     dword ptr [count], eax
#endif
		mov     tchar_ptr [eax + ecx], '\0'
		jmp     memcpy

		align   16
	L1:
		mov     eax, dword ptr [string1]
		ret

		#undef tchar_ptr
		#undef string1
		#undef string2
		#undef count
	}
}
#endif
