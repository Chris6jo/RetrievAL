#include <windows.h>
#include "TMainForm.h"
#undef MainForm
#include "TMenuItem.h"

extern WORD wMaxMenuId;
WORD wDebugWithoutMouseOverModeId;
WORD wCollapseDirsId;
WORD wNowValueDrawId;

BOOL __stdcall AppendDebugWithoutMouseOverModeMenu(TMainForm *MainForm)
{
	MENUITEMINFOA mii;

	wDebugWithoutMouseOverModeId = ++wMaxMenuId;
	mii.cbSize = sizeof(mii);

	mii.fMask = MIIM_ID | MIIM_STRING | MIIM_FTYPE;
	mii.fType = MFT_STRING;
	mii.wID = wCollapseDirsId = ++wMaxMenuId;
	mii.dwTypeData = "�K�w�����(&D)";
	InsertMenuItemA(TMenuItem_GetHandle(MainForm->M_View), 0, TRUE, &mii);

	mii.fMask = MIIM_ID | MIIM_STRING | MIIM_FTYPE;
	mii.fType = MFT_STRING;
	mii.wID = wNowValueDrawId = ++wMaxMenuId;
	mii.dwTypeData = "���ݒl�̕`��(&V)";
	InsertMenuItemA(TMenuItem_GetHandle(MainForm->M_View), -1, TRUE, &mii);

	mii.fMask = MIIM_ID | MIIM_TYPE;
	mii.fType = MFT_STRING | MFT_RADIOCHECK;
	mii.wID = wDebugWithoutMouseOverModeId;
	mii.hbmpChecked = NULL;
	mii.dwTypeData = "SSG�f�o�b�O���[�h - �}�E�X�I�[�o�[������ (&4)";
	mii.cch = 49;
	return InsertMenuItemA(TMenuItem_GetHandle(MainForm->M_UserMode), 4, TRUE, &mii);
}
