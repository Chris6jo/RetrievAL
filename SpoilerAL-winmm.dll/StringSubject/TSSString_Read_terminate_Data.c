#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSString.h"

static void __fastcall TSSString_Read_terminate_Data(TSSString *this, string *Data, unsigned long Pos);

void __declspec(naked) Caller_TSSString_Read_terminate_Data()
{
	__asm
	{
		#define this ebx
		#define Data (ebp - 1CH)

		lea     edx, [Data]
		mov     ecx, this
		jmp     TSSString_Read_terminate_Data

		#undef this
		#undef Data
	}
}

static void __fastcall TSSString_Read_terminate_Data(TSSString *this, string *Data, unsigned long Pos)
{
	if (this->codePage == TSSSTRING_CP_ANSI)
		*(string_end(Data) = string_begin(Data) + Pos) = '\0';
}
