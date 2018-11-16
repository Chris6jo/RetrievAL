#pragma once

#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#include "bcb6_std_map.h"
#include "bcb6_std_set.h"

#define DT_NEST      0x00000001
#define DT_ESCAPE    0x00000002
#define ET_TRIM_L    0x00000004
#define ET_TRIM_R    0x00000008
#define ET_TRIM      (ET_TRIM_L | ET_TRIM_R)
#define ET_REPLACE   0x00000010
#define ET_SOME_EDIT (ET_TRIM | ET_REPLACE)

typedef struct
{
	LPVOID          *VTable;
	bcb6_std_string nestStartTag;
	bcb6_std_string nestEndTag;
	bcb6_std_string escapeTag;
	bcb6_std_map    replaceMap;
	bcb6_std_set    trimSet;
	LPVOID          padding1;

} TStringDivision;

EXTERN_C unsigned long __cdecl TStringDivision_Find(
	IN  TStringDivision       *this,
	IN  const bcb6_std_string *Src,
	IN  bcb6_std_string       Token,
	IN  unsigned long         FromIndex,
	IN  unsigned long         ToIndex,
	IN  unsigned long         Option);

EXTERN_C unsigned long __cdecl TStringDivision_FindBack(
	IN  TStringDivision       *this,
	IN  const bcb6_std_string *Src,
	IN  bcb6_std_string       Token,
	IN  unsigned long         FromIndex,
	IN  unsigned long         ToIndex,
	IN  unsigned long         Option);

EXTERN_C unsigned long __cdecl TStringDivision_FindByVector(
	IN  TStringDivision              *this,
	IN  const bcb6_std_string        *Src,
	IN  const bcb6_std_vector_string *Token,
	OUT unsigned long                *FindElementIndex,
	IN  unsigned long                FromIndex,
	IN  unsigned long                ToIndex,
	IN  unsigned long                Option);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_Half(
	OUT bcb6_std_string *Result,
	IN  TStringDivision *this,
	OUT bcb6_std_string *Src,
	IN  bcb6_std_string Token,
	IN  unsigned long   Index,
	IN  unsigned long   Option);

EXTERN_C unsigned long TStringDivision_List(
	IN  TStringDivision        *this,
	IN  const bcb6_std_string  *Src,
	IN  bcb6_std_string        Token,
	OUT bcb6_std_vector_string *List,
	IN  unsigned long          Option);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_Editing(
	IN  bcb6_std_string       *Result,
	IN  TStringDivision       *this,
	IN  const bcb6_std_string *Src,
	IN  unsigned long         Option);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_Remove(
	OUT bcb6_std_string       *Result,
	IN  TStringDivision       *Reserved1,
	IN  const bcb6_std_string *Src,
	IN  bcb6_std_string       Token,
	IN  const char            *Dest,
	IN  unsigned long         Reserved2);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_RemoveByMap(
	OUT bcb6_std_string       *Result,
	IN  TStringDivision       *this,
	IN  const bcb6_std_string *Src,
	IN  const bcb6_std_map    *ReplaceMap,
	IN  unsigned long         Reserved);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_TrimDefault(
	OUT bcb6_std_string       *Result,
	IN  TStringDivision       *Reserved1,
	IN  const bcb6_std_string *Src,
	IN  const void            *Reserved2,
	IN  unsigned long         Reserved3);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_TrimFull(
	OUT bcb6_std_string       *Result,
	IN  TStringDivision       *Reserved1,
	IN  const bcb6_std_string *Src,
	IN  const void            *Reserved2,
	IN  unsigned long         Reserved3);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_Trim(
	OUT bcb6_std_string       *Result,
	IN  TStringDivision       *Reserved,
	IN  const bcb6_std_string *Src,
	IN  const bcb6_std_set    *TrimSet,
	IN  unsigned long         Option);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_ToString(
	OUT bcb6_std_string *Result,
	IN  unsigned long   Src,
	IN  const char      *Format);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_ToStringDouble(
	OUT bcb6_std_string *Result,
	IN  double          Src,
	IN  const char      *Format);

EXTERN_C BOOLEAN __cdecl TStringDivision_WCList(
	IN  TStringDivision        *this,
	IN  const bcb6_std_string  *Val1,
	IN  const bcb6_std_string  *Val2,
	OUT bcb6_std_vector_string *List1,
	OUT bcb6_std_vector_string *List2);

EXTERN_C unsigned long(__cdecl * const TStringDivision_ToULongDef)(const bcb6_std_string* Src, unsigned long Default);

