#include <windows.h>
#include <stdint.h>
#include <errno.h>
#include "intrinsic.h"
#include "HashBytes.h"
#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"
#include "SubjectStringTable.h"

EXTERN_C void __stdcall ReplaceDefine(TSSGAttributeSelector *attributeSelector, string *line);

__inline TScopeAttribute *new_TScopeAttribute()
{
	static const char tag[] = "heap";
	static const string Tag = { (LPSTR)tag, (LPSTR)tag + _countof(tag) - 1, NULL, NULL, tag, MAXDWORD };

	THeapAdjustmentAttribute *const NewAElem = TSSGCtrl_MakeAdjustmentClass(&Tag);
	assert(NewAElem && map_end(&NewAElem->heapMap)->_M_parent == NULL);
	NewAElem->type = atSCOPE;
	return (TScopeAttribute *)NewAElem;
}

vector *__cdecl TSSGCtrl_ReadSSG_PushElement(TSSGAttributeSelector *attributeSelector, TDirAttribute *rootDirAttr)
{
	TScopeAttribute *scope = new_TScopeAttribute();
	scope->super.adjustVal = 0;// global scope setup
	TSSGAttributeSelector_PushElement(attributeSelector, rootDirAttr);
	return TSSGAttributeSelector_AddElement(attributeSelector, scope);
}

__inline void Attribute_scope_open(TSSGCtrl *this, string *code)
{
	string label, Token;
	vector_string tmpV = { NULL };

	TScopeAttribute *scope = new_TScopeAttribute();
	scope->super.adjustVal = -(intptr_t)scope;// guarantee unique

	ReplaceDefine(TSSGCtrl_GetAttributeSelector(this), code);
	TStringDivision_Half_WithoutTokenDtor(&label, &this->strD, code, ":", 1, 0, FALSE);
	if (string_at(&label, 0) != ':')
	{
		LPSTR last;
		ULONG val = strtoul(string_c_str(&label), &last, 0);
		if (last == string_end(&label)) scope->super.adjustVal = val;
	}
	string_dtor(&label);

	TSSGAttributeSelector_AddElement(&this->attributeSelector, scope);
	scope = *(TScopeAttribute **)list_end(this->attributeSelector.nowAttributeList)->_M_prev->_M_data;

	string_ctor_assign_char(&Token, ',');
	TStringDivision_List(&this->strD, code, Token, &tmpV, etTRIM);
	for (string *tmpS = vector_begin(&tmpV); tmpS < vector_end(&tmpV); ++tmpS)
	{
		TStringDivision_Half_WithoutTokenDtor(&label, &this->strD, tmpS, "=", 1, 0, etTRIM);
		if (!string_empty(&label) && !string_empty(tmpS))
		{
			BOOL assign = string_at(&label, 0) != '=';
			string *var = assign ? &label : tmpS;
			LPSTR key[] = { &string_at(var, string_at(var, 0) == SCOPE_PREFIX), string_end(var) };
			ScopeVariant sv = { SubjectStringTable_insert((string *)&key), 0, 0 };
			map_iterator it = map_lower_bound(&scope->heapMap, &sv.Identity);
			if (it == map_end(&scope->heapMap) || ((ScopeVariant *)pair_first(it))->Identity != sv.Identity)
				map_dword_dw_dw_insert(&it, &scope->heapMap, it, &sv);
			if (assign)
			{
				char       *endptr;

				errno = 0;
				*(uint64_t *)pair_second(it, sv.Identity) = _strtoui64(string_c_str(tmpS), &endptr, 0);
				do	/* do { ... } while (0); */
				{
					if (errno != ERANGE)
						switch (*endptr)
						{
						case '\0':
						case '\t':
						case '\n':
						case '\r':
						case ' ':
						case '#':
						case '/':
						case ';':
							continue;
						}
					*(double *)pair_second(it, sv.Identity) = strtod(string_c_str(tmpS), NULL);
				} while (0);
			}
		}
		string_dtor(&label);
	}
	vector_string_dtor(&tmpV);
}

__inline void Attribute_scope_close(TSSGCtrl *this)
{
	TSSGAttributeSelector_EraseElementByType(&this->attributeSelector, atSCOPE);
}
