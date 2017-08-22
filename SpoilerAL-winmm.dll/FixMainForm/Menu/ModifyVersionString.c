#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "TMainForm.h"
#undef MainForm
#include "TMenuItem.h"
#include "BuildDate.h"

extern const char lpSSGVersion[4];

void __stdcall ModifyVersionString(TMainForm *MainForm)
{
	const char lpVersion[] = {
		'v', 'e', 'r', 's', 'i', 'o', 'n', ' ', ':', ' ',
		lpSSGVersion[0], lpSSGVersion[1], lpSSGVersion[2],
		' ', '-', ' ', BUILD_DATE_ARRAY('.', '.'), ' ',
		'(', '&', 'R', ')',
		'\0'
	};

	TMenuItem_SetCaption(MainForm->M_Version, lpVersion);
}
