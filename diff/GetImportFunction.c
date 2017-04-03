#include <windows.h>
#include <stdlib.h>
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif
#include <mbstring.h>
#define PSAPI_VERSION 1
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#include "intrinsic.h"

extern HANDLE hHeap;

EXTERN_C FARPROC __stdcall GetImportFunction(HANDLE hProcess, HMODULE hModule, LPCSTR lpModuleName, LPCSTR lpProcName)
{
	LPCBYTE                  lpAddress;
	IMAGE_DOS_HEADER         DosHeader;
	DWORD                    NtSignature;
	WORD                     OptionalHdrMagic;
	IMAGE_DATA_DIRECTORY     DataDirectory;
	PIMAGE_IMPORT_DESCRIPTOR lpImportDescriptor;
	IMAGE_IMPORT_DESCRIPTOR  ImportDescriptor;
	SYSTEM_INFO              SystemInfo;
	DWORD                    dwPageSize;
	DWORD                    dwForward;
	size_t                   nPageRemainMask;
	size_t                   nModuleNameSize;
	LPSTR                    lpBuffer;

	lpAddress = (LPCBYTE)hModule;
	if (!ReadProcessMemory(hProcess, lpAddress, &DosHeader, sizeof(DosHeader), NULL))
		goto FAILED;
	if (DosHeader.e_magic != IMAGE_DOS_SIGNATURE)
		goto FAILED;
	lpAddress += DosHeader.e_lfanew;
	if (!ReadProcessMemory(hProcess, lpAddress, &NtSignature, sizeof(NtSignature), NULL))
		goto FAILED;
	if (NtSignature != IMAGE_NT_SIGNATURE)
		goto FAILED;
	lpAddress += offsetof(IMAGE_NT_HEADERS, OptionalHeader);
	if (!ReadProcessMemory(hProcess, lpAddress, &OptionalHdrMagic, sizeof(OptionalHdrMagic), NULL))
		goto FAILED;
	if (OptionalHdrMagic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
		lpAddress += offsetof(IMAGE_OPTIONAL_HEADER32, DataDirectory) + IMAGE_DIRECTORY_ENTRY_IMPORT * sizeof(IMAGE_DATA_DIRECTORY);
#ifdef _WIN64
	else if (OptionalHdrMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
		lpAddress += offsetof(IMAGE_OPTIONAL_HEADER64, DataDirectory) + IMAGE_DIRECTORY_ENTRY_IMPORT * sizeof(IMAGE_DATA_DIRECTORY);
#endif
	else
		goto FAILED;
	if (!ReadProcessMemory(hProcess, lpAddress, &DataDirectory, sizeof(DataDirectory), NULL))
		goto FAILED;
	if (DataDirectory.VirtualAddress == 0 || DataDirectory.Size < sizeof(IMAGE_IMPORT_DESCRIPTOR))
		goto FAILED;
	lpImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((LPCBYTE)hModule + DataDirectory.VirtualAddress);
	if (!ReadProcessMemory(hProcess, lpImportDescriptor, &ImportDescriptor, sizeof(ImportDescriptor), NULL))
		goto FAILED;
	if (ImportDescriptor.Characteristics == 0)
		goto FAILED;
	GetSystemInfo(&SystemInfo);
	dwPageSize = SystemInfo.dwPageSize;
	if (dwPageSize == 0)
		goto FAILED;
	_BitScanForward(&dwForward, dwPageSize);
	nPageRemainMask = (size_t)1 << dwForward;
	nPageRemainMask = nPageRemainMask == dwPageSize ?
		nPageRemainMask - 1 :
		0;
	if (lpModuleName)
	{
		if (*lpModuleName)
		{
			nModuleNameSize = strlen(lpModuleName) + 1;
			if (nModuleNameSize > dwPageSize)
				goto FAILED;
		}
		else
		{
			lpModuleName = NULL;
		}
	}
	if (!IS_INTRESOURCE(lpProcName))
	{
		size_t nProcNameSize;
		size_t nBufferedPage;

		nProcNameSize = strlen(lpProcName) + 1;
		if (nProcNameSize > dwPageSize)
			goto FAILED;
		lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, dwPageSize);
		if (!lpBuffer)
			goto FAILED;
		nBufferedPage = (size_t)NULL;
		do
		{
			size_t            nNameAddress;
			size_t            nNameInPage;
			size_t            nPage;
			size_t            nNextPage;
			size_t            nSize;
			PIMAGE_THUNK_DATA lpNameThunk;
			PIMAGE_THUNK_DATA lpFunctionThunk;

			if (lpModuleName)
			{
				nNameAddress = (size_t)hModule + ImportDescriptor.Name;
				if (nPageRemainMask)
					nNameInPage = nNameAddress & nPageRemainMask;
				else
					nNameInPage = nNameAddress % dwPageSize;
				nPage = nNameAddress - nNameInPage;
				nNextPage = nPage + dwPageSize;
				if (nBufferedPage < nPage || nBufferedPage >= nNextPage)
				{
					if (!ReadProcessMemory(hProcess, (LPCVOID)(nBufferedPage = nPage), lpBuffer, dwPageSize, NULL))
						break;
				}
				nSize = dwPageSize - nNameInPage;
				if (nSize >= nModuleNameSize)
				{
					if (_mbsicmp((LPCBYTE)lpModuleName, (LPCBYTE)lpBuffer + nNameInPage) != 0)
						continue;
				}
				else
				{
					memcpy(lpBuffer, (LPCBYTE)lpBuffer + nNameInPage, nSize);
					if (!ReadProcessMemory(hProcess, (LPCVOID)nNextPage, (LPBYTE)lpBuffer + nSize, nNameInPage, NULL))
						break;
					if (_mbsicmp((LPCBYTE)lpModuleName, (LPCBYTE)lpBuffer) != 0)
					{
						memcpy(lpBuffer, (LPCBYTE)lpBuffer + nSize, nNameInPage);
						if (!ReadProcessMemory(hProcess, (LPCBYTE)nNextPage + nNameInPage, (LPBYTE)lpBuffer + nNameInPage, nSize, NULL))
							break;
						nBufferedPage = nNextPage;
						continue;
					}
					nBufferedPage = (size_t)NULL;
				}
			}
			for (lpNameThunk = (PIMAGE_THUNK_DATA)((LPBYTE)hModule + ImportDescriptor.OriginalFirstThunk),
				lpFunctionThunk = (PIMAGE_THUNK_DATA)((LPBYTE)hModule + ImportDescriptor.FirstThunk);
				;
				lpNameThunk++,
				lpFunctionThunk++)
			{
				ULONG_PTR AddressOfData;
				LPCSTR    lpszComparand1;
				LPCSTR    lpszComparand2;
				size_t    nCompareLength;
				ULONG_PTR Function;

				if (!ReadProcessMemory(hProcess, &lpNameThunk->u1.AddressOfData, &AddressOfData, sizeof(AddressOfData), NULL))
					goto RELEASE;
				if (!AddressOfData)
					break;
				if ((LONG_PTR)AddressOfData < 0)
					continue;
				nNameAddress = (size_t)((PIMAGE_IMPORT_BY_NAME)((LPBYTE)hModule + AddressOfData))->Name;
				if (nPageRemainMask)
					nNameInPage = nNameAddress & nPageRemainMask;
				else
					nNameInPage = nNameAddress % dwPageSize;
				nPage = nNameAddress - nNameInPage;
				nNextPage = nPage + dwPageSize;
				if (nBufferedPage < nPage || nBufferedPage >= nNextPage)
				{
					if (!ReadProcessMemory(hProcess, (LPCVOID)(nBufferedPage = nPage), lpBuffer, dwPageSize, NULL))
						goto RELEASE;
				}
				nSize = dwPageSize - nNameInPage;
				if (nSize >= nProcNameSize)
				{
					lpszComparand1 = lpProcName;
					lpszComparand2 = lpBuffer + nNameInPage;
					nCompareLength = nProcNameSize;
				}
				else
				{
					if (memcmp(lpProcName, lpBuffer + nNameInPage, nSize) != 0)
						continue;
					if (!ReadProcessMemory(hProcess, (LPCVOID)(nBufferedPage = nNextPage), lpBuffer, dwPageSize, NULL))
						goto RELEASE;
					lpszComparand1 = lpProcName + nSize;
					lpszComparand2 = lpBuffer;
					nCompareLength = nProcNameSize - nSize;
				}
				if (memcmp(lpszComparand1, lpszComparand2, nCompareLength) != 0)
					continue;
				if (!ReadProcessMemory(hProcess, &lpFunctionThunk->u1.Function, &Function, sizeof(Function), NULL))
					goto RELEASE;
				HeapFree(hHeap, 0, lpBuffer);
				return (FARPROC)Function;
			}
			if (lpModuleName)
				break;
		} while (
			ReadProcessMemory(hProcess, ++lpImportDescriptor, &ImportDescriptor, sizeof(ImportDescriptor), NULL) &&
			ImportDescriptor.Characteristics);
	}
	else if (lpModuleName)
	{
		char   lpBaseName[MAX_PATH];
		size_t nBufferedPage;

		if (!lpModuleName)
		{
			DWORD cbNeeded;

			nModuleNameSize = GetModuleBaseNameA(hProcess, hModule, lpBaseName, _countof(lpBaseName));
			if (!nModuleNameSize)
				goto FAILED;
			nModuleNameSize++;
			lpModuleName = lpBaseName;
			if (!EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbNeeded))
				goto FAILED;
		}
		lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, dwPageSize);
		if (!lpBuffer)
			goto FAILED;
		nBufferedPage = (size_t)NULL;
		do
		{
			size_t            nNameAddress;
			size_t            nNameInPage;
			size_t            nPage;
			size_t            nNextPage;
			size_t            nSize;
			PIMAGE_THUNK_DATA lpOriginalThunk;
			PIMAGE_THUNK_DATA lpFunctionThunk;

			nNameAddress = (size_t)hModule + ImportDescriptor.Name;
			if (nPageRemainMask)
				nNameInPage = nNameAddress & nPageRemainMask;
			else
				nNameInPage = nNameAddress % dwPageSize;
			nPage = nNameAddress - nNameInPage;
			nNextPage = nPage + dwPageSize;
			if (nBufferedPage < nPage || nBufferedPage >= nNextPage)
			{
				if (!ReadProcessMemory(hProcess, (LPCVOID)(nBufferedPage = nPage), lpBuffer, dwPageSize, NULL))
					break;
			}
			nSize = dwPageSize - nNameInPage;
			if (nSize >= nModuleNameSize)
			{
				if (_mbsicmp((LPCBYTE)lpModuleName, (LPCBYTE)lpBuffer + nNameInPage) != 0)
					continue;
			}
			else
			{
				memcpy(lpBuffer, (LPCBYTE)lpBuffer + nNameInPage, nSize);
				if (!ReadProcessMemory(hProcess, (LPCVOID)nNextPage, (LPBYTE)lpBuffer + nSize, nNameInPage, NULL))
					break;
				if (_mbsicmp((LPCBYTE)lpModuleName, (LPCBYTE)lpBuffer) != 0)
				{
					memcpy(lpBuffer, (LPCBYTE)lpBuffer + nSize, nNameInPage);
					if (!ReadProcessMemory(hProcess, (LPCBYTE)nNextPage + nNameInPage, (LPBYTE)lpBuffer + nNameInPage, nSize, NULL))
						break;
					nBufferedPage = nNextPage;
					continue;
				}
			}
			for (lpOriginalThunk = (PIMAGE_THUNK_DATA)((LPBYTE)hModule + ImportDescriptor.OriginalFirstThunk),
				lpFunctionThunk = (PIMAGE_THUNK_DATA)((LPBYTE)hModule + ImportDescriptor.FirstThunk);
				;
				lpOriginalThunk++,
				lpFunctionThunk++)
			{
				ULONG_PTR AddressOfData;
				ULONG_PTR Function;

				if (!ReadProcessMemory(hProcess, &lpOriginalThunk->u1.AddressOfData, &AddressOfData, sizeof(AddressOfData), NULL))
					goto RELEASE;
				if (!AddressOfData)
					goto RELEASE;
				if ((LONG_PTR)AddressOfData >= 0)
					continue;
				if ((AddressOfData & MAXLONG_PTR) != (ULONG_PTR)lpProcName)
					continue;
				if (!ReadProcessMemory(hProcess, &lpFunctionThunk->u1.Function, &Function, sizeof(Function), NULL))
					goto RELEASE;
				HeapFree(hHeap, 0, lpBuffer);
				return (FARPROC)Function;
			}
		} while (
			ReadProcessMemory(hProcess, ++lpImportDescriptor, &ImportDescriptor, sizeof(ImportDescriptor), NULL) &&
			ImportDescriptor.Characteristics);
	}
RELEASE:
	HeapFree(hHeap, 0, lpBuffer);
FAILED:
	return NULL;
}
