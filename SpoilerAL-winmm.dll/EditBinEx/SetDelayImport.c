#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <stddef.h>
#include <assert.h>
#include "Internal.h"

DWORD SetDelayImport(PVOID BaseAddress, DWORD SizeOfImage, PIMAGE_NT_HEADERS NtHeaders, BOOL PE32Plus, LPWSTR lpParameter)
{
	DWORD                 VirtualAddress;
	DWORD                 Size;
	LPWSTR                p;
	PIMAGE_DATA_DIRECTORY DataDirectory;

	Size = 0;
	for (p = lpParameter; *p != L'\0'; p++)
	{
		if (*p == L',')
		{
			*(p++) = L'\0';
			if (GetDwordNumber(p, &Size) == FALSE)
			{
				return ERROR_INVALID_PARAMETER;
			}
			break;
		}
	}
	if (GetDwordNumber(lpParameter, &VirtualAddress) == FALSE)
	{
		return ERROR_INVALID_PARAMETER;
	}

	if (IsBadReadPtr(IMAGE_FIRST_SECTION(NtHeaders), NtHeaders->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER)))
	{
		return ERROR_ACCESS_DENIED;
	}

	/* Obtain the delay import directory RVA and size */
	if (PE32Plus == FALSE)
	{
		DataDirectory = &((PIMAGE_NT_HEADERS32)NtHeaders)->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT];
	}
	else
	{
		DataDirectory = &((PIMAGE_NT_HEADERS64)NtHeaders)->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT];
	}

	DataDirectory->VirtualAddress = VirtualAddress;
	DataDirectory->Size = Size;

	assert(offsetof(IMAGE_NT_HEADERS32, OptionalHeader) == offsetof(IMAGE_NT_HEADERS64, OptionalHeader));
	assert(offsetof(IMAGE_OPTIONAL_HEADER32, CheckSum) == offsetof(IMAGE_OPTIONAL_HEADER64, CheckSum));

	UpdateCheckSum(BaseAddress, SizeOfImage, &NtHeaders->OptionalHeader.CheckSum);

	return ERROR_SUCCESS;
}

