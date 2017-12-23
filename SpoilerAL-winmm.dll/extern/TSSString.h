#pragma once

#include <windows.h>
#include "bcb6_std_string.h"
#include "TSSGSubject.h"

enum
{
	TSSSTRING_CP_ANSI = 0,
	TSSSTRING_CP_UNICODE,
	TSSSTRING_CP_UTF8,
};

#pragma pack(push, 1)
typedef struct _TSSString
{
	TSSGSubject     parent;
	bcb6_std_string lockStr;
	unsigned long   size;
	BYTE            codePage;
	BYTE            padding3[3];
	bcb6_std_string addressStr;
	bcb6_std_string endWord;
	bcb6_std_string nowValHeadStr;
} TSSString;
#pragma pack(pop)

