#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"

EXTERN_C void __cdecl Caller_TSSGCtrl_GetSSGDataFile_CheckNocacheParam();
EXTERN_C void __cdecl Caller_TSSGCtrl_GetSSGDataFile_Parsing();
EXTERN_C void __cdecl TSSGCtrl_GetSSGDataFile_TrimString();
EXTERN_C void __cdecl TSSGCtrl_GetSSGDataFile_ExtractStringStub();
EXTERN_C void __cdecl TSSGCtrl_GetSimpleByteCode_unless_Unicode();
EXTERN_C void __cdecl TSSGCtrl_GetSSGDataFile_CopyOrMapping();
EXTERN_C void __cdecl TStringDivision_Find_unless_TokenIsEmpty();
EXTERN_C void __cdecl TSSGCtrl_GetSSGDataFile_FixSetSSGDataFile();
EXTERN_C void __cdecl TSSGCtrl_GetSSGDataFile_ClearAtBreak();
EXTERN_C void __cdecl TSSGCtrl_SetSSGDataFile_IgnoreEmptyData();

map_iterator(__cdecl * const map_string_lower_bound)(map*, string* key) = (LPVOID)0x004F20E4;
map_iterator(__cdecl * const map_string_vector_insert)(map_iterator*, map*, map_iterator pos, void* pair) = (LPVOID)0x004F2424;

static void __fastcall TSSGCtrl_SetSSGDataFile_IsSSL(
	TSSGCtrl*  const SSGC,
	map*       const tmpM,
	register string* VIt,
	string*    const VEnd)
{
	extern BOOL FixTheProcedure;
	for (string tag, Token; VIt < VEnd; VIt++) {
		if (string_empty(VIt)) continue;
		string_ctor_assign_cstr_with_length(&Token, "]", 1);
		TStringDivision_Half(&tag, &SSGC->strD, VIt, Token, 0, 0);
		if (string_length(&tag) == 6 && *(LPDWORD)&string_at(&tag, 0) == BSWAP32('[gro') && *(LPWORD)&string_at(&tag, 4) == BSWAP16('up')) {
			vector_string* Data;
			string* line = VIt + 1;
			map_iterator it = map_string_lower_bound(tmpM, VIt);
			if (it == map_end(tmpM) || !string_equals((string*)pair_first(it), VIt)) {
				struct {
					string        GroupTag;
					vector_string GroupV;
				} tmpMpair = { NULL };
				string_ctor_assign(&tmpMpair.GroupTag, VIt);
				map_string_vector_insert(&it, tmpM, it, &tmpMpair);
				string_dtor(&tmpMpair.GroupTag);
			}
			while (++VIt < VEnd && (string_length(VIt) != 8 ||
									*(LPDWORD)&string_at(VIt, 0) != BSWAP32('[/gr') ||
									*(LPDWORD)&string_at(VIt, 4) != BSWAP32('oup]')));
			Data = (vector_string*)pair_second_aligned(it, string);
			if (!FixTheProcedure) vector_string_clear(Data);
			bcb6_std_vector_string_reserve(Data, vector_size_by_type(Data, string) + (VIt - line));
			while (line < VIt) bcb6_std_vector_string_push_back(Data, line++);
		}
		string_dtor(&tag);
	}
}

static __declspec(naked) map_iterator __cdecl TSSGCtrl_SetSSGDataFile_findStub(map* dataFileMap, string* Path) {
	EXTERN_C map_iterator(__cdecl * const map_string_find)();
	__asm {// compatible with __msfastcall
		mov eax, [edx]
		cmp byte ptr [eax], '_'
		je  EXTRACT
		jmp map_string_find

	EXTRACT:// return dataFileMap.end()
		mov eax, [ecx + 8]
		ret
	}
}

#define PUSH_ECX                   (BYTE)0x51
#define PUSH_IMM8                  (BYTE)0x6A
#define MOV_EAX_DWORD_PTR_EBP_IMM8 (WORD)0x458B
#define LEA_ECX_EBP_ADD_IMM8       (WORD)0x4D8D
#define NOP                        (BYTE)0x90
#define NOP_X2                     (WORD)0x9066
#define CALL_REL                   (BYTE)0xE8
#define JMP_REL32                  (BYTE)0xE9
#define JMP_REL8                   (BYTE)0xEB

EXTERN_C void __cdecl Attach_NocachedMemoryList()
{
	// TSSGCtrl::GetSSGDataFile
#if 0
	*(LPBYTE )0x004EE11B = PUSH_IMM8;
	*(LPDWORD)0x004EE11C = 0x401F0F00;
	*(LPBYTE )0x004EE120 = 0x00;// nop
#endif

	*(LPDWORD)(0x004EE180 + 1) = (DWORD)Caller_TSSGCtrl_GetSSGDataFile_CheckNocacheParam - (0x004EE180 + 1 + sizeof(DWORD));

	*(LPBYTE )(0x004EE19E + 2) = 0x07;
	*(LPDWORD)(0x004EE1B9 + 2) = 0x000000A8;
	*(LPDWORD)(0x004EE23F + 1) = 0x00000007;

	/*
		if (!IsNocache)
		{
			StrSize = TStringDivision::ToULongDef(tmpV[2], 1);
			RowSize = TStringDivision::ToULongDef(tmpV[3], 1);
			StepSize = TStringDivision::ToULongDef(tmpV[4], 1);
		}
		else
		{
			StrSize = Parsing(SSGS, tmpV[2], 4, "List", Address, 0);
			RowSize = Parsing(SSGS, tmpV[3], 4, "List", Address, 0);
			StepSize = Parsing(SSGS, tmpV[4], 4, "List", Address, 0);
		}
	*/
	*(LPBYTE )0x004EE30D = JMP_REL32;
	*(LPDWORD)0x004EE30E = (DWORD)Caller_TSSGCtrl_GetSSGDataFile_Parsing - (0x004EE30E + sizeof(DWORD));

	*(LPDWORD)(0x004EE3B5 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_TrimString - (0x004EE3B5 + 1 + sizeof(DWORD));

	// *(LPWSTR)&tmpC[StrSize] = L'\0';
	*(LPBYTE )0x004EEDC5 = 0x66;
	*(LPWORD )0x004EEDC6 = 0x04C7;
	*(LPDWORD)0x004EEDC8 = 0xE9000030;
	*(LPDWORD)0x004EEDCC = 0;// jmp $

	// SIt => tmpC
	*(LPDWORD)0x004EEE55 = -0x0420;
	// SKIP memcpy(tmpC, SIt, StrSize)
	*(LPBYTE )0x004EEE75 = JMP_REL8;
	*(LPBYTE )0x004EEE76 = 0x004EEE8C - (0x004EEE76 + sizeof(BYTE));
	*(LPBYTE )0x004EEE77 = 0x0F;// NOP DWORD PTR [EAX + EAX*1 + 0]
	*(LPDWORD)0x004EEE78 = 0x0000441F;

#if 1
	// }else{	//�I�[�����w��
	*(LPDWORD)(0x004EEE3E + 2) = (DWORD)TSSGCtrl_GetSSGDataFile_ExtractStringStub - (0x004EEE3E + 2 + sizeof(DWORD));

	// SIt => tmpC
	*(LPDWORD)0x004EF087 = -0x0420;
	// SKIP memcpy(tmpC, SIt, StrSize)
	*(LPBYTE )0x004EF0A7 = JMP_REL8;
	*(LPBYTE )0x004EF0A8 = 0x004EF0BE - (0x004EF0A8 + sizeof(BYTE));
	*(LPBYTE )0x004EF0A9 = 0x0F;// NOP DWORD PTR [EAX + EAX*1 + 0]
	*(LPWORD )0x004EF0AA = 0x441F;
	*(LPWORD )0x004EF0AC = 0x0000;
#else
	/*
		string EndCode((const string &)(EndWord != "unicode" ? GetSimpleByteCode(SSGS, EndWord) : string()));
	*/
	*(LPDWORD)(0x004EEFE6 + 1) = (DWORD)TSSGCtrl_GetSimpleByteCode_unless_Unicode - (0x004EEFE6 + 1 + sizeof(DWORD));

	// SIt => tmpC
	*(LPDWORD)0x004EF087 = -0x0420;

	/*
		if (!EndCode.empty())
			memcpy(SIt, tmpC, StrSize);
		else
			WideCharToMultiByte(CP_THREAD_ACP, 0, (LPCWSTR)tmpC, StrSize + 2 >> 1, SIt, StrSize + 1, NULL, NULL);
	*/
	*(LPDWORD)(0x004EF0A8 + 2) = -0x0420;
	*(LPDWORD)(0x004EF0AF + 2) = -0x041C;
	*(LPDWORD)(0x004EF0B6 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_CopyOrMapping - (0x004EF0B6 + 1 + sizeof(DWORD));

	/*
		unsigned long Pos = !EndCode.empty() ? strD.Find(string(SIt), EndCode) : string::npos;
	*/
	*(LPDWORD)(0x004EF0EF + 2) = -0x041C;
	*(LPDWORD)(0x004EF11E + 1) = (DWORD)TStringDivision_Find_unless_TokenIsEmpty - (0x004EF11E + 1 + sizeof(DWORD));
	// SIt[Pos] = (char)NULL;
	*(LPDWORD)(0x004EF1A2 + 2) = -0x041C;

	// tmpV.push_back(SIt);
	*(LPDWORD)(0x004EF1C8 + 2) = -0x041C;
#endif

#if 0
	*(LPDWORD)(0x004EE307 + 2) = (DWORD)TSSGCtrl_GetSSGDataFile_ClearAtBreak - (0x004EE307 + 2 + sizeof(DWORD));
	*(LPDWORD)(0x004EE585 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_ClearAtBreak - (0x004EE585 + 1 + sizeof(DWORD));
	*(LPDWORD)(0x004EE780 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_ClearAtBreak - (0x004EE780 + 1 + sizeof(DWORD));
	*(LPDWORD)(0x004EEC13 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_ClearAtBreak - (0x004EEC13 + 1 + sizeof(DWORD));

	// TSSGCtrl::SetSSGDataFile
	*(LPBYTE )0x004F0A48 = JMP_REL32;
	*(LPDWORD)0x004F0A49 = (DWORD)TSSGCtrl_SetSSGDataFile_IgnoreEmptyData - (0x004F0A49 + sizeof(DWORD));
	*(LPDWORD)0x004F0A4D = 0x00401F0F;// nop
#else
	*(LPBYTE)(0x004EF416 + 1) = 0x004EF430 - (0x004EF416 + 1 + sizeof(BYTE));
	// failed break
	*(LPBYTE )0x004EF423 = JMP_REL32;
	*(LPDWORD)0x004EF424 = 0x004EF581 - (0x004EF424 + sizeof(DWORD));

	*(LPDWORD)0x004EF428 = 0x00841F0F;
	*(LPDWORD)0x004EF42C = 0;// nop

	*(LPWORD )0x004EF430 = 0x458B;
	*(LPBYTE )0x004EF432 = 0xE8;
	*(LPWORD )0x004EF436 = 0x453B;
	*(LPBYTE )0x004EF438 = 0xEC;
#endif

#if 0
	*(LPBYTE )0x004EF423 = JMP_REL8;
	*(LPBYTE )0x004EF424 = 0x004EF43B - (0x004EF424 + sizeof(BYTE));
	*(LPBYTE )0x004EF425 = NOP;

	*(LPWORD )0x004EF43B = MOV_EAX_DWORD_PTR_EBP_IMM8;
	*(LPBYTE )0x004EF43D = (char)0x08;
	*(LPWORD )0x004EF43E = LEA_ECX_EBP_ADD_IMM8;
	*(LPBYTE )0x004EF440 = (char)0x10;
	*(LPBYTE )0x004EF441 = PUSH_ECX;
	*(LPWORD )0x004EF442 = LEA_ECX_EBP_ADD_IMM8;
	*(LPBYTE )0x004EF444 = (char)-0x30;
	*(LPBYTE )0x004EF445 = PUSH_ECX;
#endif

#if 0
	*(LPDWORD)(0x004EF447 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_FixSetSSGDataFile - (0x004EF447 + 1 + sizeof(DWORD));
#else
	// TSSGCtrl::SetSSGDataFile
	*(LPDWORD)0x004F0B08 = BSWAP32(0x52568D55);
	*(LPDWORD)0x004F0B0C = BSWAP32(0xE08B4D08);
	*(LPBYTE )0x004F0B10 = CALL_REL;
	*(LPDWORD)0x004F0B11 = (DWORD)TSSGCtrl_SetSSGDataFile_IsSSL - (0x004F0B11 + sizeof(DWORD));
	*(LPBYTE )0x004F0B15 = JMP_REL32;

	*(LPDWORD)(0x004F19DA + 1) = (DWORD)TSSGCtrl_SetSSGDataFile_findStub - (0x004F19DA + 1 + sizeof(DWORD));
#endif
}
