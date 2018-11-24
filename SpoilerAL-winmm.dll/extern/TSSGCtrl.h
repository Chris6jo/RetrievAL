#pragma once

#include <windows.h>
#include "bcb6_std_vector_string.h"
#include "TSSGAttributeSelector.h"
#include "TStringDivision.h"
#include "TSSGSubject.h"
#include "TProcessCtrl.h"
#include "TSSGScriptStruct.h"
#include "TSSGAttributeElement.h"

typedef struct _TSSGCtrl
{
	LPVOID                 *VTable;
	BYTE                   padding1[28];
	TSSGAttributeSelector  attributeSelector;
	LPVOID                 adjustmentListner;
	LPVOID                 ssgActionListner;
	unsigned long          lockTimerInterval;
	BYTE                   padding2[4];
	bcb6_std_string        ssgDir;
	bcb6_std_string        scriptDir;
	bcb6_std_string        exeDir;
	BOOLEAN                canLockWrite;
	BYTE                   padding3[7];
	bcb6_std_map           lockMap;
	bcb6_std_map           operatorMap;
	bcb6_std_map           stringOperatorMap;
	TSSGScriptStruct       script;
	bcb6_std_vector_string memo;
	bcb6_std_vector_string processNameVec;
	BOOLEAN                isMemoWordWrap;
	BYTE                   padding4[7];
	bcb6_std_map           dataFileMap;
	struct _TSSGSubject    *rootSubject;
	BYTE                   padding5[4];
	TProcessCtrl           processCtrl;
	TStringDivision        strD;
} TSSGCtrl;

#define TSSGCtrl_GetAttributeSelector(/*IN TSSGCtrl * */this) \
	(&(this)->attributeSelector)

#define TSSGCtrl_GetSSGActionListner(/*IN TSSGCtrl * */this) \
	(this)->ssgActionListner

EXTERN_C struct _TSSGSubject *(__cdecl * const TSSGCtrl_MakeSubjectClass)(const bcb6_std_string *Tag);
EXTERN_C LPVOID(__cdecl * const TSSGCtrl_MakeAdjustmentClass)(bcb6_std_string *);
EXTERN_C void(__cdecl * const TSSGCtrl_EnumReadSSG)(TSSGCtrl *this, bcb6_std_vector_string *SSGFile, LPVOID ParentStack, TDialogAdjustmentAttribute *ADJElem, DWORD RepeatIndex, DWORD ParentRepeat);
EXTERN_C struct _TSSGSubject *(__cdecl * const TSSGCtrl_GetTargetSubject)(struct _TSSGSubject *SSGS);
EXTERN_C void(__cdecl * const TSSGCtrl_SetSSGDataFile)(TSSGCtrl *this, bcb6_std_vector_string *Data, bcb6_std_string *FileName, BOOLEAN IsSSL);
EXTERN_C BOOLEAN (__cdecl * const TSSGCtrl_IsLocked)(TSSGCtrl *this, struct _TSSGSubject *SSGS);
EXTERN_C bcb6_std_vector_string *(__cdecl * const TSSGCtrl_ReadSSRFile)(bcb6_std_vector_string *Result, TSSGCtrl *this, const bcb6_std_string *Code, bcb6_std_vector_dword *Indices, struct _TSSGSubject *SSGS);
EXTERN_C void(__cdecl * const TSSGCtrl_LoopSSRFile)(TSSGCtrl *this, bcb6_std_vector *FormatVec, bcb6_std_vector *Dest, unsigned long LoopVal);
EXTERN_C LPVOID(__cdecl * const TSSGCtrl_GetAttribute)(TSSGCtrl *this, struct _TSSGSubject *SSGS, unsigned long Type);
EXTERN_C unsigned long(__cdecl * const TSSGCtrl_GetAddress)(TSSGCtrl *this, struct _TSSGSubject *SSGS, const bcb6_std_string *AddressStr, unsigned long Mode);
EXTERN_C unsigned long(__cdecl * const TSSGCtrl_GetDirLevel)(TSSGCtrl *this, struct _TSSGSubject *SSGS);
EXTERN_C unsigned long(__cdecl * const TSSGCtrl_AddressAttributeFilter)(TSSGCtrl *this, struct _TSSGSubject *SSGS, DWORD *Address, unsigned long Mode);
EXTERN_C BOOLEAN(__cdecl * const TSSGCtrl_CheckFunnel)(TSSGCtrl *this, struct _TSSGSubject *SSGS, unsigned long Val);
EXTERN_C BOOLEAN(__cdecl * const TSSGCtrl_Funneling)(TSSGCtrl *this, struct _TSSGSubject *SSGS, const bcb6_std_string *FileName, unsigned long Val);
EXTERN_C unsigned long(__cdecl * const TSSGCtrl_CheckIO_FEP)(TSSGCtrl *this, struct _TSSGSubject *SSGS, unsigned long Val, BOOLEAN IsInput);
EXTERN_C double(__cdecl * const TSSGCtrl_CheckIO_FEPDouble)(TSSGCtrl *this, struct _TSSGSubject *SSGS, double Val, BOOLEAN IsInput);

EXTERN_C BOOLEAN __fastcall TSSGCtrl_IsRemoteProcess(LPCSTR p);

EXTERN_C bcb6_std_vector_string *(__cdecl * const TSSGCtrl_GetSSGDataFile)(TSSGCtrl *this, struct _TSSGSubject *SSGS, bcb6_std_string FName, bcb6_std_string DefaultExt, bcb6_std_string *CurrentDir);

EXTERN_C bcb6_std_string * __cdecl TSSGCtrl_GetNameString(bcb6_std_string *Result, TSSGCtrl *this, struct _TSSGSubject *SSGS, const bcb6_std_string *NameStr);

EXTERN_C bcb6_std_string *(__cdecl * const TSSGCtrl_GetSimpleByteCode)(bcb6_std_string *Result, TSSGCtrl *this, struct _TSSGSubject *SSGS, bcb6_std_string CodeStr);
EXTERN_C BOOLEAN(__cdecl * const TSSGCtrl_IsChildRWProhibit)(TSSGCtrl *this, struct _TSSGSubject *SSGS);
HANDLE __cdecl TSSGCtrl_Open(TSSGCtrl *this, struct _TSSGSubject *SSGS, DWORD Mode);
EXTERN_C BOOLEAN(__cdecl * const TSSGCtrl_OneRead)(TSSGCtrl *this, struct _TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size);
EXTERN_C BOOLEAN(__cdecl * const TSSGCtrl_OneWrite)(TSSGCtrl *this, struct _TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size);

EXTERN_C void(__cdecl * const TSSGCtrl_SetLock)(TSSGCtrl *this, BOOLEAN IsLock, struct _TSSGSubject *Target, void *Value);
