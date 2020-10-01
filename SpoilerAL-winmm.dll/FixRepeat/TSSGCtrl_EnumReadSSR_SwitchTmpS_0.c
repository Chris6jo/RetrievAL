#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"
#include "TSSGCtrl.h"

#pragma warning(push)
#pragma warning(disable:4163)
#pragma function(memcpy)
#pragma function(memmove)
#pragma warning(pop)

#pragma warning(disable:4733)

void __cdecl TSSGCtrl_TrimString();

extern const DWORD bcb6_std_string_substr;
extern const DWORD F0050123C;
extern const DWORD __InitExceptBlockLDTC;

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSR_SwitchTmpS_0()
{
	static const DWORD data1[] = {
		0x00402634,
		0x00000004, -8,
		0x00402634,
		0x00000004, -16,
		0x00416274,
		0x00000004, -40,
		0x00416274,
		0x00000004, -64,
		0x00416274,
		0x00000004, -88,
		0x00501888,
		0x00000004, -120,
		0x00000000,
	};
	static const DWORD data2[] = {
		0x00402634,
		0x00000004, -8,
		0x00416274,
		0x00000004, -328,
		0x00402634,
		0x00000004, -16,
		0x00416274,
		0x00000004, -40,
		0x00416274,
		0x00000004, -64,
		0x00000000,
	};
	static const DWORD data3[] = {
		0x00402634,
		0x00000004, -16,
		0x00416274,
		0x00000004, -368,
		0x00000000,
	};
	static const DWORD data4[] = {
		0x00501888,
		0x00000004, -152,
		0x00000000,
	};
	static const DWORD data5[] = {
		0x00000000, -188,
		0x00050000, 0x00000000,
		(DWORD)data1,
		0x00050008, 0x00000000,
		(DWORD)data2,
		0x00050014, 0x00000005,
		(DWORD)data3,
		0x00050008, 0x00000000,
		0x00000000, 0x00050000,
		0x00000000, 0x00000000,
		0x00050000, 0x00000000,
		0x00000000, 0x00050000,
		0x00000000, 0x00000000,
		0x00050000, 0x00000000,
		0x00000000, 0x00050000,
		0x00000000, 0x00000000,
		0x00050000, 0x00000000,
		0x00000000, 0x00050000,
		0x00000000, 0x00000000,
		0x00050000, 0x00000000,
		0x00000000, 0x00050000,
		0x00000000,
		(DWORD)data4,
		0x00050098, 0x00000000,
		0x00000000, 0x00050000,
		0x00000000, 0x00000000,
		0x00050000, 0x00000000,
		0x00000000,
	};
	static const DWORD x004FFDC5 = 0x004FFDC5;
	static const DWORD x005009A5 = 0x005009A5;
	static const DWORD x00500DFF = 0x00500DFF;

	__asm
	{
		sub     al, '@' - '#'
		je      L1
		cmp     al, 'F' - '@'
		je      FormatPrefix
		jmp     dword ptr [x004FFDC5]

	L1:
		jmp     dword ptr [x005009A5]

	FormatPrefix:

		#define this     (ebp + 5A8H + 8H)
		#define LineList (ebp + 5A8H - 40H)
		#define tmpS     (ebp + 5A8H - 18H)

		push    ebp
		mov     eax, offset data5
		mov     ebp, esp
		sub     esp, 284
		push    ebx
		push    esi
		mov     ebx, dword ptr [this]
		lea     esi, [tmpS]
		add     ebx, offset TSSGCtrl.strD
		call    dword ptr [__InitExceptBlockLDTC]
		push    0
		push    0
		sub     esp, 24
		mov     edx, 00631BFDH
		mov     ecx, esp
		call    string_ctor_assign_cstr
		push    -1
		push    1
		push    0
		push    0
		sub     esp, 24
		mov     edx, 00631BFBH
		mov     ecx, esp
		call    string_ctor_assign_cstr
		push    esi
		lea     ecx, [ebp - 28H]
		push    ebx
		push    ecx
		call    TStringDivision_Half
		add     esp, 44
		lea     edx, [ebp - 28H]
		push    edx
		lea     ecx, [ebp - 40H]
		push    ecx
		call    dword ptr [bcb6_std_string_substr]
		add     esp, 16
		lea     ecx, [ebp - 40H]
		push    ecx
		lea     eax, [ebp - 58H]
		push    ebx
		push    eax
		call    TSSGCtrl_TrimString
		add     esp, 44
		lea     edx, [ebp - 58H]
		mov     byte ptr [ebp - 0C5H], 32
		lea     ecx, [ebp - 70H]
		mov     dword ptr [ebp - 0C4H], edx
		mov     byte ptr [ebp - 78H], 32
		call    string_ctor_assign
		mov     eax, dword ptr [LineList + 8H]
		lea     edx, [ebp - 78H]
		mov     dword ptr [ebp - 0C0H], edx
		lea     ecx, [LineList]
		mov     dword ptr [ebp - 0CCH], eax
		push    edx
		push    ecx
		call    dword ptr [F0050123C]
		mov     edx, dword ptr [ebp - 0CCH]
		add     esp, 8
		mov     dword ptr [ebp - 0D0H], edx
		mov     ecx, dword ptr [edx + 4H]
		mov     dword ptr [ebp - 0D4H], ecx
		mov     dword ptr [eax], edx
		mov     dword ptr [eax + 4H], ecx
		mov     dword ptr [ecx], eax
		mov     dword ptr [edx + 4H], eax
		mov     dword ptr [ebp - 0D8H], eax
		lea     ecx, [ebp - 70H]
		call    string_dtor
		lea     ecx, [ebp - 58H]
		call    string_dtor
		lea     ecx, [ebp - 40H]
		call    string_dtor
		lea     ecx, [ebp - 28H]
		call    string_dtor
		mov     ecx, dword ptr [esi]
		mov     edx, dword ptr [esi + 4H]
		call    __reg64return_TrimSpace
		mov     ecx, eax
		mov     eax, dword ptr [esi]
		cmp     ecx, dword ptr [esi + 4H]
		jne     L2
		cmp     eax, edx
		je      L5
	L2:
		sub     ecx, edx
		cmp     eax, edx
		mov     dword ptr [ebp - 0F0H], ecx
		je      L3
		push    ecx
		push    edx
		push    eax
		call    memcpy
		add     esp, 12
		mov     ecx, dword ptr [ebp - 0F0H]
	L3:
		mov     edx, dword ptr [esi + 4H]
		mov     eax, dword ptr [esi]
		sub     edx, eax
		mov     dword ptr [ebp - 0F4H], ecx
		cmp     edx, ecx
		jb      L4
		add     edx, eax
		add     eax, ecx
		mov     dword ptr [ebp - 0F8H], edx
		mov     dword ptr [ebp - 0FCH], eax
		cmp     eax, edx
		je      L5
		mov     cl, byte ptr [edx]
		mov     dword ptr [ebp - 100H], 1
		mov     dword ptr [ebp - 104H], eax
		mov     dword ptr [esi + 4H], eax
		mov     byte ptr [eax], cl
		jmp     L5

	L4:
		mov     eax, dword ptr [esi + 4H]
		add     edx, ecx
		sub     edx, eax
		push    0
		mov     ecx, esi
		call    string_append_repeat_char
	L5:
		mov     al, byte ptr ds:[00641AE4H]
		mov     edx, esi
		mov     byte ptr [ebp - 98H], al
		lea     ecx, [ebp - 90H]
		call    string_ctor_assign
		mov     eax, dword ptr [LineList + 8H]
		lea     edx, [ebp - 98H]
		mov     dword ptr [ebp - 108H], edx
		lea     ecx, [LineList]
		mov     dword ptr [ebp - 10CH], eax
		mov     dword ptr [ebp - 110H], eax
		push    edx
		push    ecx
		call    dword ptr [F0050123C]
		mov     edx, dword ptr [ebp - 110H]
		add     esp, 8
		mov     ecx, dword ptr [edx + 4H]
		mov     dword ptr [ebp - 114H], ecx
		mov     dword ptr [eax], edx
		mov     dword ptr [eax + 4H], ecx
		mov     dword ptr [ecx], eax
		mov     dword ptr [edx + 4H], eax
		mov     dword ptr [ebp - 118H], eax
		lea     ecx, [ebp - 90H]
		call    string_dtor
		mov     ecx, dword ptr [ebp - 188]
		mov     dword ptr fs:[0], ecx
		pop     esi
		pop     ebx
		mov     esp, ebp
		pop     ebp
		jmp     dword ptr [x00500DFF]

		#undef this
		#undef LineList
		#undef tmpS
	}
}
