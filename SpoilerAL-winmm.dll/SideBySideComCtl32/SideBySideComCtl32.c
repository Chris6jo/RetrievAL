#include <windows.h>
#include <tlhelp32.h>

#if defined _M_IX86
#  pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#  pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#  pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#  pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

extern HMODULE hHeap;

HMODULE hComCtl32 = NULL;

/***********************************************************************
 *      LoadComCtl32
 */
BOOL __cdecl LoadComCtl32()
{
	static __inline void ReplaceImportAddressTable(HMODULE hModule);
	static __inline void ReplaceExportAddressTable(HMODULE hModule);

	wchar_t lpModuleName[MAX_PATH];
	UINT    uLength;
	HMODULE hModule;

	if (hComCtl32)
		return TRUE;
	uLength = GetSystemDirectoryW(lpModuleName, _countof(lpModuleName));
	if (uLength == 0 || uLength >= _countof(lpModuleName))
		return FALSE;
	if (lpModuleName[uLength - 1] != L'\\')
		lpModuleName[uLength++] = L'\\';
	if (uLength >= _countof(lpModuleName) - 13)
		return FALSE;
	lpModuleName[uLength     ] = L'c';
	lpModuleName[uLength +  1] = L'o';
	lpModuleName[uLength +  2] = L'm';
	lpModuleName[uLength +  3] = L'c';
	lpModuleName[uLength +  4] = L't';
	lpModuleName[uLength +  5] = L'l';
	lpModuleName[uLength +  6] = L'3';
	lpModuleName[uLength +  7] = L'2';
	lpModuleName[uLength +  8] = L'.';
	lpModuleName[uLength +  9] = L'd';
	lpModuleName[uLength + 10] = L'l';
	lpModuleName[uLength + 11] = L'l';
	lpModuleName[uLength + 12] = L'\0';
	hModule = GetModuleHandleW(lpModuleName);
	if (!hModule)
		return FALSE;
	hComCtl32 = LoadLibraryW(L"comctl32.dll");
	if (!hComCtl32)
		return FALSE;
	if (hComCtl32 != hModule)
	{
		ReplaceImportAddressTable(hModule);
		ReplaceExportAddressTable(hModule);
		return TRUE;
	}
	else
	{
		FreeLibrary(hComCtl32);
		hComCtl32 = NULL;
		return FALSE;
	}
}

/***********************************************************************
 *      ReplaceImportAddressTable
 */
static __inline void ReplaceImportAddressTable(HMODULE hModule)
{
	HANDLE         hSnapshot;
	MODULEENTRY32W entry;

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return;
	entry.dwSize = sizeof(entry);
	if (!Module32FirstW(hSnapshot, &entry))
		goto FINALLY;
	do
	{
		PIMAGE_DOS_HEADER        lpDosHeader;
		PIMAGE_NT_HEADERS        lpNtHeader;
		PIMAGE_IMPORT_DESCRIPTOR lpDescriptor;

		if (entry.modBaseAddr == (LPBYTE)hModule ||
			entry.modBaseAddr == (LPBYTE)hComCtl32)
			continue;
		lpDosHeader = (PIMAGE_DOS_HEADER)entry.modBaseAddr;
		if (lpDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
			continue;
		lpNtHeader = (PIMAGE_NT_HEADERS)(entry.modBaseAddr + lpDosHeader->e_lfanew);
		if (lpNtHeader->Signature != IMAGE_NT_SIGNATURE)
			continue;
		if (!lpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress ||
			!lpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size)
			continue;
		lpDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(entry.modBaseAddr +
			lpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
		if (!lpDescriptor->Name)
			continue;
		do
		{
			LPCSTR            lpName;
			PIMAGE_THUNK_DATA lpThunkINT, lpThunkIAT;

			lpName = (LPCSTR)(entry.modBaseAddr + lpDescriptor->Name);
			if (lpName[ 0] != 'C' && lpName[ 0] != 'c' ||
				lpName[ 1] != 'O' && lpName[ 1] != 'o' ||
				lpName[ 2] != 'M' && lpName[ 2] != 'm' ||
				lpName[ 3] != 'C' && lpName[ 3] != 'c' ||
				lpName[ 4] != 'T' && lpName[ 4] != 't' ||
				lpName[ 5] != 'L' && lpName[ 5] != 'l' ||
				lpName[ 6] != '3'                      ||
				lpName[ 7] != '2'                      ||
				lpName[ 8] != '.'                      ||
				lpName[ 9] != 'D' && lpName[ 9] != 'd' ||
				lpName[10] != 'L' && lpName[10] != 'l' ||
				lpName[11] != 'L' && lpName[11] != 'l' ||
				lpName[12] != '\0')
				continue;
			lpThunkINT = (PIMAGE_THUNK_DATA)(entry.modBaseAddr + lpDescriptor->OriginalFirstThunk);
			lpThunkIAT = (PIMAGE_THUNK_DATA)(entry.modBaseAddr + lpDescriptor->FirstThunk);
			while (lpThunkINT->u1.AddressOfData)
			{
				LPCSTR  lpProcName;
				FARPROC lpFunction;
				DWORD   dwProtect;

				lpProcName = IMAGE_SNAP_BY_ORDINAL(lpThunkINT->u1.AddressOfData) ?
					(LPCSTR)IMAGE_ORDINAL(lpThunkINT->u1.AddressOfData) :
					((PIMAGE_IMPORT_BY_NAME)(entry.modBaseAddr + lpThunkINT->u1.AddressOfData))->Name;
				lpFunction = GetProcAddress(hComCtl32, lpProcName);
				if (lpFunction && VirtualProtect(&lpThunkIAT->u1.Function, sizeof(ULONG_PTR), PAGE_READWRITE, &dwProtect))
				{
					lpThunkIAT->u1.Function = (ULONG_PTR)lpFunction;
					VirtualProtect(&lpThunkIAT->u1.Function, sizeof(ULONG_PTR), dwProtect, &dwProtect);
				}
				lpThunkINT++;
				lpThunkIAT++;
			}
			break;
		} while ((++lpDescriptor)->Characteristics);
	} while (Module32NextW(hSnapshot, &entry));
FINALLY:
	CloseHandle(hSnapshot);
}

/***********************************************************************
 *      ReplaceExportAddressTable
 */
static __inline void ReplaceExportAddressTable(HMODULE hModule)
{
	PIMAGE_DATA_DIRECTORY   DataDirectory;
	PIMAGE_EXPORT_DIRECTORY ExportDirectory;
	LPDWORD                 AddressOfFunctions;
	LPDWORD                 AddressOfNames;
	LPWORD                  AddressOfNameOrdinals;
	DWORD                   Protect;
	DWORD                   Index;

#ifndef _M_IX86
	#error "x86 architecture not defined"
#endif

	#define DOS_HEADER(hModule) ((PIMAGE_DOS_HEADER)hModule)
	#define NT_HEADERS(hModule) ((PIMAGE_NT_HEADERS)((LPCBYTE)hModule + DOS_HEADER(hModule)->e_lfanew))

	DataDirectory = NT_HEADERS(hModule)->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT;
	if (!DataDirectory->VirtualAddress || DataDirectory->Size < sizeof(IMAGE_EXPORT_DIRECTORY))
		return;
	ExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((LPCBYTE)hModule + DataDirectory->VirtualAddress);
	AddressOfFunctions    = (LPDWORD)((LPCBYTE)hModule + ExportDirectory->AddressOfFunctions   );
	AddressOfNames        = (LPDWORD)((LPCBYTE)hModule + ExportDirectory->AddressOfNames       );
	AddressOfNameOrdinals = (LPWORD )((LPCBYTE)hModule + ExportDirectory->AddressOfNameOrdinals);
	if (!VirtualProtect(AddressOfFunctions, ExportDirectory->NumberOfFunctions * sizeof(DWORD), PAGE_READWRITE, &Protect))
		return;
	for (Index = 0; Index < ExportDirectory->NumberOfFunctions; Index++)
	{
		FARPROC ProcAddress;

		ProcAddress = GetProcAddress(hComCtl32, MAKEINTRESOURCEA(ExportDirectory->Base + Index));
		if (!ProcAddress)
			continue;
#ifdef _M_IX86
		AddressOfFunctions[Index] = (DWORD)ProcAddress - (DWORD)hModule;
#endif
	}
	for (Index = 0; Index < ExportDirectory->NumberOfNames; Index++)
	{
		FARPROC ProcAddress;

		ProcAddress = GetProcAddress(hComCtl32, (LPCSTR)hModule + AddressOfNames[Index]);
		if (!ProcAddress)
			continue;
#ifdef _M_IX86
		AddressOfFunctions[AddressOfNameOrdinals[Index]] = (DWORD)ProcAddress - (DWORD)hModule;
#endif
	}
	VirtualProtect(AddressOfFunctions, ExportDirectory->NumberOfFunctions * sizeof(DWORD), Protect, &Protect);

	#undef DOS_HEADER
	#undef NT_HEADERS
}

