#include <windows.h>
#include <stdlib.h>
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif

#ifdef __BORLANDC__
#include "MainUnit.h"
#define TMainForm_Guide(Mes, IsClear) (MainForm)->Guide(Mes, IsClear)
#else
#include "TMainForm.h"
#endif

EXTERN_C HANDLE hHeap;

#if defined(_MSC_VER) && !defined(_DEBUG)
EXTERN_C int __fastcall __vsnprintf(char *buffer, size_t count, const char *format, va_list argptr, const va_list endarg);
#else
#define __vsnprintf(buffer, count, format, argptr, endarg) _vsnprintf(buffer, count, format, argptr)
#endif

int __fastcall GuidePrintV(const char *format, va_list argptr, const va_list endarg);

#if defined(_MSC_VER) && defined(_M_IX86)
__declspec(naked) int __cdecl GuidePrint(const char *format, ...)
{
	__asm
	{
		mov     eax, dword ptr [esp]
		mov     ecx, dword ptr [esp + 4]
		mov     dword ptr [esp], 0
		lea     edx, [esp + 8]
		push    eax
		jmp     GuidePrintV
	}
}
#else
int __cdecl GuidePrint(const char *format, ...)
{
	va_list argptr;
	int     length;

	va_start(argptr, format);
	length = GuidePrintV(format, argptr, NULL);
	va_end(argptr);
	return length;
}
#endif

int __fastcall GuidePrintV(const char *format, va_list argptr, const va_list endarg)
{
	char stackBuffer[256];
	int  length;

	length = __vsnprintf(stackBuffer, _countof(stackBuffer), format, argptr, endarg);
	if ((unsigned int)length < _countof(stackBuffer))
	{
		TMainForm_Guide(stackBuffer, FALSE);
	}
	else if (length >= 0)
	{
		unsigned int size;
		char         *heapBuffer;

		size = length + 1;
		heapBuffer = (char *)HeapAlloc(hHeap, 0, size * sizeof(char));
		if (heapBuffer)
		{
			length = __vsnprintf(heapBuffer, size, format, argptr, endarg);
			if ((unsigned int)length < size)
				TMainForm_Guide(heapBuffer, FALSE);
			HeapFree(hHeap, 0, heapBuffer);
		}
	}
	return length;
}

