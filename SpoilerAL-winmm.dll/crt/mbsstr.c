#include <windows.h>

unsigned char * __cdecl _mbsstr(const unsigned char *str, const unsigned char *strSearch)
{
	size_t length1, length2;

	length1 = strlen((const char *)str);
	length2 = strlen((const char *)strSearch);
	if (length1 > length2)
		for (const unsigned char *end = str + length1 - length2; str <= end; str++)
			if (memcmp(str, strSearch, length2) == 0)
				return (unsigned char *)str;
			else if (IsDBCSLeadByteEx(CP_THREAD_ACP, *str))
				str++;
	else if (length1 == length2)
		if (memcmp(str, strSearch, length1) == 0)
			return (unsigned char *)str;
	return NULL;
}
