#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <stddef.h>

/***********************************************************************
 *      CalcCheckSum (internal)
 */
__forceinline DWORD CalcCheckSum(LPVOID BaseAddress, DWORD SizeOfImage)
{
	DWORD  Sum;
	LPWORD CurPtr, EndPtr;

	Sum = 0;
	CurPtr = (LPWORD)BaseAddress;
	EndPtr = (LPWORD)((LPBYTE)BaseAddress + (SizeOfImage & ~(sizeof(WORD) - 1)));
	for (; ; )
	{
		WORD WordData;

		if (CurPtr < EndPtr)
		{
			WordData = *CurPtr;
		}
		else if (CurPtr == EndPtr && (SizeOfImage & (sizeof(WORD) - 1)))
		{
			WordData = (WORD)*(LPBYTE)CurPtr;
		}
		else
		{
			break;
		}
		CurPtr++;
		Sum += WordData;
		if (HIWORD(Sum) != 0)
		{
			Sum = LOWORD(Sum) + HIWORD(Sum);
		}
	}

	return (DWORD)(LOWORD(Sum) + HIWORD(Sum));
}

/***********************************************************************
 *      UpdateCheckSum
 */
void UpdateCheckSum(LPVOID BaseAddress, DWORD SizeOfImage, LPDWORD Checksum)
{
	DWORD CalcSum;
	DWORD HdrSum;

	CalcSum = CalcCheckSum(BaseAddress, SizeOfImage);

	HdrSum = *Checksum;

	/* Subtract image checksum from calculated checksum. */
	/* fix low word of checksum */
	if (LOWORD(CalcSum) >= LOWORD(HdrSum))
	{
		CalcSum -= LOWORD(HdrSum);
	}
	else
	{
		CalcSum = ((LOWORD(CalcSum) - LOWORD(HdrSum)) & 0xFFFF) - 1;
	}

	/* fix high word of checksum */
	if (LOWORD(CalcSum) >= HIWORD(HdrSum))
	{
		CalcSum -= HIWORD(HdrSum);
	}
	else
	{
		CalcSum = ((LOWORD(CalcSum) - HIWORD(HdrSum)) & 0xFFFF) - 1;
	}

	/* add file length */
	CalcSum += SizeOfImage;

	*Checksum = CalcSum;
}

//
// The function walks through the section headers, finds out the section
// the given RVA belongs to, and uses the section header to determine
// the file offset that corresponds to the given RVA
//
// Return value: "TRUE" if succeeded, "FALSE" if failed
//
BOOL GetFileOffsetFromRVA(PIMAGE_NT_HEADERS NtHeaders, DWORD RVA, PDWORD FileOffset)
{
	/* Check parameters */
	if (NtHeaders != NULL)
	{
		PIMAGE_SECTION_HEADER SectionHeader;
		int                   i;

		/* Look up the section the RVA belongs to */
		SectionHeader = IMAGE_FIRST_SECTION(NtHeaders);
		for (i = 0; i < NtHeaders->FileHeader.NumberOfSections; i++, SectionHeader++)
		{
			DWORD SectionSize;

			SectionSize = SectionHeader->Misc.VirtualSize;
			if (SectionSize == 0)
			{
				/* compensate for Watcom linker strangeness, according to Matt Pietrek */
				SectionHeader->SizeOfRawData;
			}
			if ((RVA >= SectionHeader->VirtualAddress) &&
				(RVA < SectionHeader->VirtualAddress + SectionSize))
			{
				/* Yes, the RVA belongs to this section */
				/* Look up the file offset using the section header */
				*FileOffset = RVA - (SectionHeader->VirtualAddress - SectionHeader->PointerToRawData);
				return TRUE;
			}
		}
	}
	/* Section not found */
	return FALSE;
}

BOOL GetDecimalNumber(LPCWSTR lpString, LPDWORD lpdwNumber)
{
	DWORD   n;
	LPCWSTR p;

	n = 0;
	for (p = lpString; *p != L'\0'; p++)
	{
		if (*p >= L'0' && *p <= L'9')
		{
			n *= 10;
			n += *p - L'0';
		}
		else
		{
			return FALSE;
		}
	}
	*lpdwNumber = n;
	return TRUE;
}

BOOL GetDwordNumber(LPCWSTR lpString, LPDWORD lpdwNumber)
{
	LPCWSTR p;

	p = lpString;
	if (*p == L'0' && (*(p + 1) == L'x' || *(p + 1) == L'X'))
	{
		DWORD n;

		n = 0;
		for (p += 2; *p != L'\0'; p++)
		{
			if (*p >= L'0' && *p <= L'9')
			{
				n *= 0x10;
				n += *p - L'0';
			}
			else if (*p >= L'A' && *p <= L'F')
			{
				n *= 0x10;
				n += 0x0A + *p - L'A';
			}
			else if (*p >= L'a' && *p <= L'f')
			{
				n *= 0x10;
				n += 0x0A + *p - L'a';
			}
			else
			{
				return FALSE;
			}
		}
		*lpdwNumber = n;
		return TRUE;
	}
	return GetDecimalNumber(p, lpdwNumber);
}

