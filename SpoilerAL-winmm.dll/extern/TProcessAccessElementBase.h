#pragma once

#include "bcb6_std_vector.h"

typedef enum ProcessAccessElementType {
//	atUNKNOWN           =  0,
	atSKIP              =  1,
	atJUMP              =  2,
	atIF                =  3,
	atDATA              =  4,
	atMASK_DATA         =  5,
	atSIGNED_NUM_DATA   =  6,
	atUNSIGNED_NUM_DATA =  7,
	atLOGICAL_OR        =  8,
	atLOGICAL_AND       =  9,
	atSELECT            = 10,
	atLOOP              = 11,
	atFLOAT_NUM_DATA    = 12,
	atNUM_DATA          = 13,
	atBIT_DATA          = 14,
} AccessElementType;

typedef struct _TProcessAccessElementBase {
	LPVOID           *VTable;
	AccessElementType type;
} TProcessAccessElementBase;

#define TProcessAccessElement_dtor(PAEB, bfDel)\
	((void (__cdecl *)(void*, BYTE))(PAEB)->VTable[0])(PAEB, bfDel)

#define delete_TProcessAccessElement(PAEB)\
	TProcessAccessElement_dtor(PAEB, 0x03)

#define TProcessAccessElement_GetType(PAEB)\
	(PAEB)->type

#define TProcessAccessElement_GetSize(PAEB, IsTrueMode)\
	((unsigned long (__cdecl *)(void*, BOOLEAN))(PAEB)->VTable[1])(PAEB, IsTrueMode)

#define TProcessAccessElement_SetSize(PAEB, Val, IsTrueMode)\
	((void (__cdecl *)(void*, unsigned long, BOOLEAN))(PAEB)->VTable[2])(PAEB, Val, IsTrueMode)

typedef struct _TProcessAccessElementLoop {
	LPVOID               *VTable;
	AccessElementType     type;
	bcb6_std_vector_dword loopVec;
	bcb6_std_vector_dword surplusVec;
	unsigned long         loopCount;
	DWORD                 paddding;
} TProcessAccessElementLoop;
