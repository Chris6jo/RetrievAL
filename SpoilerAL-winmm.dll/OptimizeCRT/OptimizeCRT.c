#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

#pragma comment(lib, "crt\\asmlib\\libacof32o.lib")

EXTERN_C void * __cdecl bcb6_calloc(size_t num, size_t size);
EXTERN_C void * __cdecl bcb6_malloc(size_t size);
EXTERN_C void __cdecl bcb6_free(void *memblock);
EXTERN_C void * __cdecl bcb6_realloc(void *memblock, size_t size);
EXTERN_C int __cdecl bcb6_snprintf(char *buffer, size_t count, const char *format, ...);
EXTERN_C unsigned long __cdecl bcb6_strtol(const char *nptr, char **endptr, int base);
EXTERN_C unsigned long __cdecl bcb6_strtoul(const char *nptr, char **endptr, int base);
EXTERN_C int __stdcall _lstrcmpA(LPCSTR lpString1, LPCSTR lpString2);
EXTERN_C LPSTR __stdcall _lstrcpyA(LPSTR lpString1, LPCSTR lpString2);
EXTERN_C int __stdcall _lstrlenA(LPCSTR lpString);

#define JMP_REL32 (BYTE)0xE9
#define NOP       (BYTE)0x90
#define NOP_X2    (WORD)0x9090

EXTERN_C void __cdecl OptimizeCRT()
{
	*(LPBYTE )0x005D4458 = JMP_REL32;
	*(LPDWORD)0x005D4459 = (DWORD)bcb6_calloc - (0x005D4459 + sizeof(DWORD));

	*(LPBYTE )0x005D4B3C = JMP_REL32;
	*(LPDWORD)0x005D4B3D = (DWORD)bcb6_malloc - (0x005D4B3D + sizeof(DWORD));
	*(LPBYTE )0x005D4B41 = NOP;

	*(LPBYTE )0x005D4B4C = JMP_REL32;
	*(LPDWORD)0x005D4B4D = (DWORD)bcb6_free - (0x005D4B4D + sizeof(DWORD));
	*(LPBYTE )0x005D4B51 = NOP;

	*(LPBYTE )0x005D4B5C = JMP_REL32;
	*(LPDWORD)0x005D4B5D = (DWORD)bcb6_realloc - (0x005D4B5D + sizeof(DWORD));
	*(LPBYTE )0x005D4B61 = NOP;

	*(LPBYTE )0x005D5108 = JMP_REL32;
	*(LPDWORD)0x005D5109 = (DWORD)memcpy - (0x005D5109 + sizeof(DWORD));

	*(LPBYTE )0x005D512C = JMP_REL32;
	*(LPDWORD)0x005D512D = (DWORD)memmove - (0x005D512D + sizeof(DWORD));

	*(LPBYTE )0x005D5178 = JMP_REL32;
	*(LPDWORD)0x005D5179 = (DWORD)memset - (0x005D5179 + sizeof(DWORD));
	*(LPWORD )0x005D517D = NOP_X2;

	*(LPBYTE )0x005D521C = JMP_REL32;
	*(LPDWORD)0x005D521D = (DWORD)strcat - (0x005D521D + sizeof(DWORD));

	*(LPBYTE )0x005D5258 = JMP_REL32;
	*(LPDWORD)0x005D5259 = (DWORD)strlen - (0x005D5259 + sizeof(DWORD));
	*(LPBYTE )0x005D525D = NOP;

	*(LPBYTE )0x005D534C = JMP_REL32;
	*(LPDWORD)0x005D534D = (DWORD)memcmp - (0x005D534D + sizeof(DWORD));

	*(LPBYTE )0x005D7EE8 = JMP_REL32;
	*(LPDWORD)0x005D7EE9 = (DWORD)bcb6_snprintf - (0x005D7EE9 + sizeof(DWORD));
	*(LPWORD )0x005D7EED = NOP_X2;

	*(LPBYTE )0x005D9C38 = JMP_REL32;
	*(LPDWORD)0x005D9C39 = (DWORD)_itoa - (0x005D9C39 + sizeof(DWORD));
	*(LPBYTE )0x005D9C3D = NOP;

	*(LPBYTE )0x005D9C88 = JMP_REL32;
	*(LPDWORD)0x005D9C89 = (DWORD)_ltoa - (0x005D9C89 + sizeof(DWORD));
	*(LPBYTE )0x005D9C8D = NOP;

	*(LPBYTE )0x005D9C68 = JMP_REL32;
	*(LPDWORD)0x005D9C69 = (DWORD)_ultoa - (0x005D9C69 + sizeof(DWORD));

	*(LPBYTE )0x005DAE5C = JMP_REL32;
	*(LPDWORD)0x005DAE5D = (DWORD)bcb6_strtol - (0x005DAE5D + sizeof(DWORD));
	*(LPBYTE )0x005DAE61 = NOP;

	*(LPBYTE )0x005DAF54 = JMP_REL32;
	*(LPDWORD)0x005DAF55 = (DWORD)bcb6_strtoul - (0x005DAF55 + sizeof(DWORD));
	*(LPBYTE )0x005DAF59 = NOP;

	*(LPBYTE )0x00600EA8 = JMP_REL32;
	*(LPDWORD)0x00600EA9 = (DWORD)_lstrcmpA - (0x00600EA9 + sizeof(DWORD));
	*(LPBYTE )0x00600EAD = NOP;

	*(LPBYTE )0x00600EAE = JMP_REL32;
	*(LPDWORD)0x00600EAF = (DWORD)_lstrcpyA - (0x00600EAF + sizeof(DWORD));
	*(LPBYTE )0x00600EB3 = NOP;

	*(LPBYTE )0x00600EBA = JMP_REL32;
	*(LPDWORD)0x00600EBB = (DWORD)_lstrlenA - (0x00600EBB + sizeof(DWORD));
	*(LPBYTE )0x00600EBF = NOP;
}
