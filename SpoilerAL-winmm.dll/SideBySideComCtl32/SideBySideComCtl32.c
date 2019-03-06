#include <windows.h>

#if defined _M_IX86
#  pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#  pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#  pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#  pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

static HMODULE hSideBySide = NULL;

BOOL __cdecl LoadComCtl32();
void __cdecl FreeComCtl32();

static __inline void ReplaceExportFunctions(HMODULE hModule)
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

		ProcAddress = GetProcAddress(hSideBySide, MAKEINTRESOURCEA(ExportDirectory->Base + Index));
		if (!ProcAddress)
			continue;
#ifdef _M_IX86
		AddressOfFunctions[Index] = (DWORD)ProcAddress - (DWORD)hModule;
#endif
	}
	for (Index = 0; Index < ExportDirectory->NumberOfNames; Index++)
	{
		FARPROC ProcAddress;

		ProcAddress = GetProcAddress(hSideBySide, (LPCSTR)hModule + AddressOfNames[Index]);
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

BOOL __cdecl LoadComCtl32()
{
	wchar_t lpModuleName[MAX_PATH];
	UINT    uLength;
	HMODULE hModule;

	if (hSideBySide)
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
	hSideBySide = LoadLibraryW(L"comctl32.dll");
	if (!hSideBySide)
		return FALSE;
	if (hSideBySide != hModule)
	{
		ReplaceExportFunctions(hModule);
		return TRUE;
	}
	else
	{
		FreeComCtl32();
		return FALSE;
	}
}

void __cdecl FreeComCtl32()
{
	if (hSideBySide)
	{
		FreeLibrary(hSideBySide);
		hSideBySide = NULL;
	}
}
