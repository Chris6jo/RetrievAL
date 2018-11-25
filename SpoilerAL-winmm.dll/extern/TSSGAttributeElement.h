#pragma once

#include <windows.h>
#include "bcb6_std_string.h"
#include "bcb6_std_map.h"
#include "TSSArg.h"
#include "TStringDivision.h"

typedef LPVOID(__cdecl * const LPFN_TSSGATTRIBUTEELEMENT_ISEQUAL)(LPCVOID elem1, LPCVOID elem2);
#define TSSGAttributeElement_IsEqual(elem1, elem2) (*(*(LPFN_TSSGATTRIBUTEELEMENT_ISEQUAL **)(elem1) + 1))((LPCVOID)(elem1), (LPCVOID)(elem2))

typedef enum AttrType {
	//atUNKNOWN,
	atDIR_LEVEL    = 1,
	atADJUST       = 2,
	atREPLACE      = 4,
	atSIZE         = 8,
	atALL          = 15,
	atFUNNEL       = 16,
	atIO_FEP       = 32,
	atE_WITH       = 64,
	atENABLED      = 128,
	atADJUST_CHECK = 256,
	atCHILD_RW     = 512,
	atCAUTION      = 1024,
	atVARIABLE     = 0x0800,
	atDEFINE       = 0x1000,
	atERRORSKIP    = 0x2000,
	atSCOPE        = 0x4000,
} AtType;// for debugger

//---------------------------------------------------------------------
//�u�N���X�錾�v
//---------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TSSGAttributeElement {
	LPVOID *VTable;
	AtType type;
} TSSGAttributeElement, TSimpleAdjustmentAttribute;
#pragma pack(pop)

EXTERN_C TSSGAttributeElement * __cdecl new_TSSGAttributeElement();
EXTERN_C void __fastcall delete_TSSGAttributeElement(TSSGAttributeElement *this);
#define TSSGAttributeElement_GetType(/*TSSGAttributeElement * */this) \
	(this)->type
//----------------------------------------------------------------------------
//�u�K�w�����N���X�v
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TDirAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	unsigned long level;
} TDirAttribute;
#pragma pack(pop)

EXTERN_C TDirAttribute * __cdecl new_TDirAttribute();
EXTERN_C void __fastcall delete_TDirAttribute(TDirAttribute *this);
#define TDirAttribute_Setting(/*TDirAttribute * */this, /*unsigned long */Level) \
	((this)->level = Level)
//----------------------------------------------------------------------------
//�u�ǂݏ����T�C�Y�w�葮���N���X�v
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TSizeAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	unsigned long size;
} TSizeAttribute;
#pragma pack(pop)

EXTERN_C TSizeAttribute * __cdecl new_TSizeAttribute();
EXTERN_C void __fastcall delete_TSizeAttribute(TSizeAttribute *this);
#define TSizeAttribute_Setting(/*TSizeAttribute * */this, /*unsigned long */Size) \
	((this)->size = Size)
//----------------------------------------------------------------------------
//�u�A�h���X�u�����������N���X�v
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TReplaceAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	bcb6_std_string offsetCode;
	bcb6_std_string fileName;
	unsigned long   offsetNum;
	DWORD           padding;
} TReplaceAttribute;
#pragma pack(pop)

EXTERN_C TReplaceAttribute * __cdecl new_TReplaceAttribute();
EXTERN_C void __fastcall delete_TReplaceAttribute(TReplaceAttribute *this);
EXTERN_C void(__cdecl * const TReplaceAttribute_Setting)(TReplaceAttribute *, TStringDivision *, const char *);
//----------------------------------------------------------------------------
//�u�t�@���l���i���s�������݁j�����N���X�v
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TFunnelAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	bcb6_std_string fileName;
} TFunnelAttribute;
#pragma pack(pop)

EXTERN_C TFunnelAttribute * __cdecl new_TFunnelAttribute();
EXTERN_C void __fastcall delete_TFunnelAttribute(TFunnelAttribute *this);
#define TFunnelAttribute_Setting(/*TFunnelAttribute * */this, /*const bcb6_std_string * */FileName) \
	bcb6_std_string_assign(&(this)->fileName, FileName)
//----------------------------------------------------------------------------
//�u���o�͎��ϊ������N���X�v
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TIO_FEPAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	bcb6_std_string inputCode;
	bcb6_std_string outputCode;
} TIO_FEPAttribute;
#pragma pack(pop)

EXTERN_C TIO_FEPAttribute * __cdecl new_TIO_FEPAttribute();
EXTERN_C void __fastcall delete_TIO_FEPAttribute(TIO_FEPAttribute *this);
EXTERN_C void(__cdecl * const TIO_FEPAttribute_Setting)(TIO_FEPAttribute *this, TStringDivision *StrD, const char *Code);
#define TIO_FEPAttribute_GetInputCode(/*TIO_FEPAttribute * */this) \
	(&(this)->inputCode);
#define TIO_FEPAttribute_GetOutputCode(/*TIO_FEPAttribute * */this) \
	(&(this)->outputCode);
//----------------------------------------------------------------------------
//�u�R�[�h�t�������N���X�v
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TEndWithAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	bcb6_std_string code;
} TEndWithAttribute;
#pragma pack(pop)

#define TEndWithAttribute_Procedures (LPVOID *)0x0064030C
EXTERN_C TEndWithAttribute * __cdecl new_TEndWithAttribute();
EXTERN_C void __fastcall delete_TEndWithAttribute(TEndWithAttribute *this);
#define TEndWithAttribute_GetCode(/*TEndWithAttribute * */this) \
	(&(this)->code)
#define TEndWithAttribute_Setting(/*TEndWithAttribute * */this, /*const bcb6_std_string * */Code) \
	bcb6_std_string_assign(&(this)->code, Code)
#define TEndWithAttribute_Setting_cstr(/*TEndWithAttribute * */this, /*LPCSTR */Code, /*size_t */Length) \
	bcb6_std_string_assign_cstr_with_length(&(this)->code, Code, Length)
//----------------------------------------------------------------------------
//�u�L����ԑ����N���X�v
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TEnabledAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	bcb6_std_string code;
} TEnabledAttribute;
#pragma pack(pop)

EXTERN_C TEnabledAttribute * __cdecl new_TEnabledAttribute();
EXTERN_C void __fastcall delete_TEnabledAttribute(TEnabledAttribute *this);
#define TEnabledAttribute_Setting(/*TEnabledAttribute * */this, /*const bcb6_std_string * */Code) \
	bcb6_std_string_assign(&(this)->code, Code)
//----------------------------------------------------------------------------
//�u���ʍ��ڈꊇ�ۑ����N���X�v
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TChildRWAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	unsigned char prohibit;
	BYTE          padding[3];
} TChildRWAttribute;
#pragma pack(pop)

EXTERN_C TChildRWAttribute * __cdecl new_TChildRWAttribute();
EXTERN_C void __fastcall delete_TChildRWAttribute(TChildRWAttribute *this);
EXTERN_C void __cdecl TChildRWAttribute_Setting(TChildRWAttribute *this, TStringDivision *StrD, const char *Code);
//----------------------------------------------------------------------------
//�u���ӎ����w�葮���N���X�v
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TCautionAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	bcb6_std_string fileName;
} TCautionAttribute;
#pragma pack(pop)

EXTERN_C TCautionAttribute * __cdecl new_TCautionAttribute();
EXTERN_C void __fastcall delete_TCautionAttribute(TCautionAttribute *this);
#define TCautionAttribute_Setting(/*TCautionAttribute * */this, /*const bcb6_std_string * */FileName) \
	bcb6_std_string_assign(&(this)->fileName, FileName)
//----------------------------------------------------------------------------
//�u�A�h���X�␳�̎���Ȃǂŗp����A�␳���쎞�ɗD��g�p���邩�Ȃǂ̑����N���X�v
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TAdjustCheckAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	BOOLEAN check;
	BOOLEAN mustCheck;
	BYTE    padding[2];
} TAdjustCheckAttribute;
#pragma pack(pop)

EXTERN_C TAdjustCheckAttribute * __cdecl new_TAdjustCheckAttribute();
EXTERN_C void __fastcall delete_TAdjustCheckAttribute(TAdjustCheckAttribute *this);
EXTERN_C void(__cdecl * const TAdjustCheckAttribute_Setting)(TAdjustCheckAttribute *this, TStringDivision *StrD, const char *Code);
//----------------------------------------------------------------------------
//�u��{�␳�������ۃN���X�v
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TAdjustmentAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSSGAttributeElement super;
	};
	unsigned long status;
	unsigned long checkType;
	unsigned long adjustVal;
	int           elemOrder;// define here for convenience' sake.
} TAdjustmentAttribute;
#pragma pack(pop)

#define TAdjustmentAttribute_Setting(Attr, SSGC, Code) \
	((void(__cdecl *)(void *, void *, const char *))(Attr)->VTable[3])(Attr, SSGC, Code)
//----------------------------------------------------------------------------
//�u�q�[�v�␳�����N���X�v
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _THeapAdjustmentAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TAdjustmentAttribute super;
	};
	bcb6_std_map heapMap;
} THeapAdjustmentAttribute, TScopeAttribute;
#pragma pack(pop)
//----------------------------------------------------------------------------
//�uCRC�␳�����N���X�v
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TCRCAdjustmentAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSimpleAdjustmentAttribute super;
	};
	BYTE         padding[16];
	bcb6_std_map crcMap;
} TCRCAdjustmentAttribute;
#pragma pack(pop)
//----------------------------------------------------------------------------
//�u�T�[�`�␳�����N���X�v
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TSearchAdjustmentAttribute {
	union {
		struct {
			LPVOID *VTable;
			AtType type;
		};
		TSimpleAdjustmentAttribute super;
	};
	BYTE            padding1[16];
	bcb6_std_string startAddressStr;
	bcb6_std_string minAddressStr;
	bcb6_std_string maxAddressStr;
	unsigned long   step;
	DWORD           padding2;
	bcb6_std_string searchCode;
	bcb6_std_map    diffMap;
} TSearchAdjustmentAttribute;
#pragma pack(pop)
//----------------------------------------------------------------------------
//�u�_�C�A���O�␳�����N���X�v
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TDialogAdjustmentAttribute {
	union {
		struct {
			LPVOID          *VTable;
			AtType          type;
			BYTE            padding1[16];
			bcb6_std_string startAddressStr;
			bcb6_std_string minAddressStr;
			bcb6_std_string maxAddressStr;
			unsigned long   step;
			DWORD           padding2;
			bcb6_std_string searchCode;
			bcb6_std_map    diffMap;
		};
		TSearchAdjustmentAttribute super;
	};
	bcb6_std_string        fileName;
	bcb6_std_string        valStr;
	BOOLEAN                isMemoWordWrap;
	BYTE                   padding3[7];
	bcb6_std_vector_string memo;
	bcb6_std_vector_string answerVec;
	bcb6_std_vector_byte   flagVec;
	bcb6_std_vector        subjectVec;
} TDialogAdjustmentAttribute;
#pragma pack(pop)
//----------------------------------------------------------------------------
typedef struct value_type {
	unsigned long key;
	struct {
		unsigned long low;
		unsigned long high;
	};
} heapMapValue;
//----------------------------------------------------------------------------

