#include <windows.h>

#include <stdlib.h>
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif

#include <regex.h>
#pragma comment(lib, "regex.lib")

#include "ProcessContainsModule.h"

#ifdef __BORLANDC__
#pragma warn -8060
#endif

extern HANDLE hHeap;

typedef struct {
	BOOL    bIsRegex;
	size_t  nClassNameLength;
	LPCVOID lpClassName;
	size_t  nWindowNameLength;
	LPCVOID lpWindowName;
	LPCVOID lpModuleName;
	HWND    hWnd;
	DWORD   dwProcessId;
} ENUM_WINDOWS_PARAM;

static BOOL CALLBACK EnumWindowsProc(HWND hWnd, ENUM_WINDOWS_PARAM *param)
{
	char         lpString[1024];
	unsigned int uLength;
	DWORD        dwProcessId;

	if (!param->bIsRegex)
	{
		if (param->lpClassName)
		{
			uLength = GetClassNameA(hWnd, lpString, _countof(lpString));
			if (uLength != param->nClassNameLength)
				return TRUE;
			if (memcmp(lpString, param->lpClassName, uLength) != 0)
				return TRUE;
		}
		if (param->lpWindowName)
		{
			uLength = GetWindowTextA(hWnd, lpString, _countof(lpString));
			if (uLength < param->nWindowNameLength)
				return TRUE;
			if (memcmp(lpString, param->lpWindowName, param->nWindowNameLength) != 0)
				return TRUE;
		}
	}
	else
	{
		if (param->lpClassName)
		{
			uLength = GetClassNameA(hWnd, lpString, _countof(lpString));
			if (!uLength)
				return TRUE;
			if (regexec(param->lpClassName, lpString, 0, NULL, 0) != 0)
				return TRUE;
		}
		if (param->lpWindowName)
		{
			uLength = GetWindowTextA(hWnd, lpString, _countof(lpString));
			if (!uLength)
				return TRUE;
			if (regexec(param->lpWindowName, lpString, 0, NULL, 0) != 0)
				return TRUE;
		}
	}
	if (!GetWindowThreadProcessId(hWnd, &dwProcessId))
		return TRUE;
	if (param->lpModuleName)
		if (!ProcessContainsModule(dwProcessId, param->bIsRegex, param->lpModuleName))
			return TRUE;
	param->hWnd = hWnd;
	param->dwProcessId = dwProcessId;
	return FALSE;
}

HWND __stdcall FindWindowContainsModule(
	IN           BOOL    bIsRegex,
	IN  OPTIONAL LPCSTR  lpClassName,
	IN  OPTIONAL LPCSTR  lpWindowName,
	IN  OPTIONAL LPCSTR  lpModuleName,
	OUT OPTIONAL LPDWORD lpdwProcessId)
{
	ENUM_WINDOWS_PARAM param;
	size_t             nClassNameLength;
	size_t             nWindowNameLength;
	size_t             nModuleNameLength;
	LPSTR              lpBuffer;

	param.hWnd = NULL;
	param.dwProcessId = 0;
	if (!(param.bIsRegex = bIsRegex))
	{
		wchar_t lpWideCharStr[MAX_PATH];

		if (!(param.lpModuleName = lpModuleName) ||
			!MultiByteToWideChar(CP_ACP, 0, lpModuleName, -1, (LPWSTR)(param.lpModuleName = lpWideCharStr), _countof(lpWideCharStr)))
		{
			param.nClassNameLength  = lpClassName ? strlen(lpClassName) : 0;
			param.lpClassName       = lpClassName;
			param.nWindowNameLength = lpWindowName ? strlen(lpWindowName) : 0;
			param.lpWindowName      = lpWindowName;
			EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)&param);
		}
	}
	else if (lpBuffer = HeapAlloc(hHeap, 0,
		(nClassNameLength  = lpClassName  ? strlen(lpClassName ) : 0) + (lpClassName  ? 3 : 0) +
		(nWindowNameLength = lpWindowName ? strlen(lpWindowName) : 0) + (lpWindowName ? 3 : 0) +
		(nModuleNameLength = lpModuleName ? strlen(lpModuleName) : 0) + (lpModuleName ? 3 : 0)))
	{
		regex_t reClassName;
		regex_t reWindowName;
		regex_t reModuleName;
		LPSTR   dest;
		LPCSTR  src;

		dest = lpBuffer;
		if (lpClassName)
		{
			src = lpClassName;
			lpClassName = dest;
			*(dest++) = '^';
			memcpy(dest, src, nClassNameLength);
			dest += nClassNameLength;
			*(((LPWORD)dest)++) = (BYTE)'$';
		}
		if (!(param.lpClassName = lpClassName) ||
			regcomp((regex_t *)(param.lpClassName = &reClassName), lpClassName, REG_EXTENDED | REG_NOSUB) == 0)
		{
			if (lpWindowName)
			{
				src = lpWindowName;
				lpWindowName = dest;
				*(dest++) = '^';
				memcpy(dest, src, nWindowNameLength);
				dest += nWindowNameLength;
				*(((LPWORD)dest)++) = (BYTE)'$';
			}
			if (!(param.lpWindowName = lpWindowName) ||
				regcomp((regex_t *)(param.lpWindowName = &reWindowName), lpWindowName, REG_EXTENDED | REG_NOSUB) == 0)
			{
				if (lpModuleName)
				{
					src = lpModuleName;
					lpModuleName = dest;
					*(dest++) = '^';
					memcpy(dest, src, nModuleNameLength);
					dest += nModuleNameLength;
					*(LPWORD)dest = (BYTE)'$';
				}
				if (!(param.lpModuleName = lpModuleName) ||
					regcomp((regex_t *)(param.lpModuleName = &reModuleName), lpModuleName, REG_EXTENDED | REG_ICASE | REG_NOSUB) == 0)
				{
					param.nClassNameLength  = 0;
					param.nWindowNameLength = 0;
					EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)&param);
					if (lpModuleName)
						regfree(&reModuleName);
				}
				if (lpWindowName)
					regfree(&reWindowName);
			}
			if (lpClassName)
				regfree(&reClassName);
		}
		HeapFree(hHeap, 0, lpBuffer);
	}
	if (lpdwProcessId)
		*lpdwProcessId = param.dwProcessId;
	return param.hWnd;
}
