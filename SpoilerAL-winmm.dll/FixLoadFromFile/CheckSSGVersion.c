#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"

extern HANDLE hHeap;

const char lpSSGVersion[4] = { '6', '.', '2', '\0' };

BOOL EnableParserFix = FALSE;

void __fastcall CheckSSGVersion(const char *begin, const char *end)
{
	EnableParserFix = FALSE;
	if (end - begin > 21 &&
		*(LPDWORD) begin       == BSWAP32('SSG ') &&
		*(LPDWORD)(begin +  4) == BSWAP32('for ') &&
		*(LPDWORD)(begin +  8) == BSWAP32('Spoi') &&
		*(LPDWORD)(begin + 12) == BSWAP32('lerA') &&
		*(LPDWORD)(begin + 16) == BSWAP32('L ve') &&
		*(LPBYTE )(begin + 20) ==         'r')
	{
		begin += 21;
		while (*begin == ' ' || *begin == '\t')
			begin++;
		if (strcmp(begin, lpSSGVersion) >= 0)
			EnableParserFix = TRUE;
	}
}
