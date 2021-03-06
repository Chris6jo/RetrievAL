#define USING_NAMESPACE_BCB6_STD
#include "TSSGActionListner.h"

#undef TSSGActionListner_OnParsingProcess61
#undef TSSGActionListner_OnParsingDoubleProcess61
#undef TSSGActionListner_OnParsingError
#undef TSSGActionListner_OnProcessOpen
#undef TSSGActionListner_OnSubjectDisabled
#undef TSSGActionListner_OnProcessOpenError
#undef TSSGActionListner_OnSubjectReadError
#undef TSSGActionListner_OnSubjectWriteSuccess

void(__cdecl * const TSSGActionListner_OnProcessOpen)(LPVOID this, TSSGSubject *SSGS, DWORD Mode) = (LPVOID)0x0052EB20;
void(__cdecl * const TSSGActionListner_OnSubjectDisabled)(LPVOID this, TSSGSubject *SSGS, string Code) = (LPVOID)0x0052EBC4;
void(__cdecl * const TSSGActionListner_OnProcessOpenError)(LPVOID this, TSSGSubject *SSGS) = (LPVOID)0x0052EE78;
void(__cdecl * const TSSGActionListner_OnSubjectReadError)(LPVOID this, TSSGSubject *SSGS, unsigned long Address) = (LPVOID)0x0052EF8C;
void(__cdecl * const TSSGActionListner_OnSubjectWriteError)(LPVOID this, TSSGSubject *SSGS, unsigned long Address) = (LPVOID)0x0052F13C;
void(__cdecl * const TSSGActionListner_OnSubjectReadSuccess)(LPVOID this, TSSGSubject *SSGS, unsigned long Address) = (LPVOID)0x0052F31C;
void(__cdecl * const TSSGActionListner_OnSubjectWriteSuccess)(LPVOID this, TSSGSubject *SSGS, unsigned long Address) = (LPVOID)0x0052F6D0;

__declspec(naked) void __stdcall TSSGActionListner_OnParsingProcess61(LPVOID ssgActionListner, TSSGSubject *SSGS, LPCSTR Code, long TopVal)
{
	__asm
	{
		#define ssgActionListner (esp +  4)
		#define SSGS             (esp +  8)
		#define Code             (esp + 12)
		#define TopVal           (esp + 16)

		mov     eax, dword ptr [TopVal]
		sub     esp, 28
		mov     edx, dword ptr [Code + 28]
		mov     dword ptr [esp + 24], eax
		mov     ecx, esp
		call    string_ctor_assign_cstr
		mov     ecx, dword ptr [ssgActionListner + 28]
		push    0
		mov     eax, dword ptr [ecx]
		push    ecx
		call    dword ptr [eax + 44]
		add     esp, 36
		ret     16

		#undef ssgActionListner
		#undef SSGS
		#undef Code
		#undef TopVal
	}
}

__declspec(naked) void __stdcall TSSGActionListner_OnParsingDoubleProcess61(LPVOID ssgActionListner, TSSGSubject *SSGS, LPCSTR Code, double TopVal)
{
	__asm
	{
		#define ssgActionListner (esp +  4)
		#define SSGS             (esp +  8)
		#define Code             (esp + 12)
		#define TopVal           (esp + 16)

		mov     eax, dword ptr [TopVal + 4]
		mov     ecx, dword ptr [TopVal]
		push    eax
		push    ecx
		mov     edx, dword ptr [Code + 8]
		sub     esp, 24
		mov     ecx, esp
		call    string_ctor_assign_cstr
		mov     ecx, dword ptr [ssgActionListner + 32]
		push    0
		mov     eax, dword ptr [ecx]
		push    ecx
		call    dword ptr [eax + 48]
		add     esp, 40
		ret     20

		#undef ssgActionListner
		#undef SSGS
		#undef Code
		#undef TopVal
	}
}

__declspec(naked) void __stdcall TSSGActionListner_OnParsingError(LPVOID ssgActionListner, TSSGSubject *SSGS, LPCSTR Code)
{
	__asm
	{
		#define ssgActionListner (esp +  4)
		#define SSGS             (esp +  8)
		#define Code             (esp + 12)

		mov     edx, dword ptr [Code]
		sub     esp, 24
		mov     ecx, esp
		call    string_ctor_assign_cstr
		mov     ecx, dword ptr [ssgActionListner + 24]
		push    0
		mov     eax, dword ptr [ecx]
		push    ecx
		call    dword ptr [eax + 40]
		add     esp, 32
		ret     12

		#undef ssgActionListner
		#undef SSGS
		#undef Code
	}
}
