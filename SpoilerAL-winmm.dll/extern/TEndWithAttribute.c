#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "TEndWithAttribute.h"
#include "bcb6_operator.h"
#include "intrinsic.h"

#pragma warning(disable:4733)

extern const DWORD F005D54CC;

__declspec(naked) TEndWithAttribute * __cdecl new_TEndWithAttribute()
{
	static const DWORD data1[] = {
		0x004EBAC4,
		0x00002007, -4,
		0x00000000,
	};
	static const DWORD data2[] = {
		0x00000000, -40,
		0x00050000, 0x00000000,
		0x00000000, 0x00050008,
		0x00000000,
		(DWORD)data1,
	};

	__asm
	{
		push    ebp
		mov     eax, offset data2
		mov     ebp, esp
		sub     esp, 40
		call    dword ptr [F005D54CC]
#if !OPTIMIZE_ALLOCATOR
		push    32
		call    dword ptr [operator_new]
		pop     ecx
#else
		mov     ecx, 32
		call    operator_new
#endif
		test    eax, eax
		jz      L1
		mov     dword ptr [ebp - 4], eax
		mov     dword ptr [eax + 4], 0
		inc     dword ptr [ebp - 12]
		mov     dword ptr [eax], 0064030CH
		lea     ecx, [eax + 8]
		call    string_ctor
		mov     eax, dword ptr [ebp - 4]
		mov     dword ptr [eax + 4], 64
	L1:
		mov     ecx, dword ptr [ebp - 40]
		mov     dword ptr fs:[0], ecx
		mov     esp, ebp
		pop     ebp
		ret
	}
}

__declspec(naked) void __fastcall delete_TEndWithAttribute(TEndWithAttribute *attribute)
{
	static const DWORD data1[] = {
		0x00000008, 0x00100400,
		0x004EBAC4,
		0x00000002,
		BSWAP32('TEnd'),
		BSWAP32('With'),
		BSWAP32('Attr'),
		BSWAP32('ibut'),
		BSWAP32('e *['),
		BSWAP24('2]\0'),
	};
	static const DWORD data2[] = {
		(DWORD)data1,
		0x0000500F, -8,
		0x00000000,
	};
	static const DWORD data3[] = {
		0x00000000, -44,
		0x00050000, 0x00000000,
		0x00000000, 0x00050008,
		0x00000000,
		(DWORD)data2,
	};

	__asm
	{
		test    ecx, ecx
		jz      L1
		push    ebp
		mov     eax, offset data3
		mov     ebp, esp
		sub     esp, 44
		mov     dword ptr [ebp - 8], ecx
		call    dword ptr [F005D54CC]
		mov     ecx, dword ptr [ebp - 8]
		push    3
		mov     eax, dword ptr [ecx]
		push    ecx
		mov     dword ptr [ebp - 4], eax
		call    dword ptr [eax]
		mov     ecx, dword ptr [ebp - 44]
		mov     dword ptr fs:[0], ecx
		mov     esp, ebp
		pop     ebp
	L1:
		ret
	}
}
