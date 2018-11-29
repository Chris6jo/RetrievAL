#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "bcb6_std_stack.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSDir.h"
#include "TSSGAttributeElement.h"

extern HANDLE hHeap;

void __stdcall Attribute_variable_open(TSSGCtrl *this, LPVOID ParentStack, LPCSTR Code, LPCSTR EndOfCode)
{
	#define stack_PTSSDir_top(Stack) ((TSSDir *)stack_dword_top((stack_dword *)Stack))

	TEndWithAttribute *lpNewVariable;
	TEndWithAttribute *lpParentVariable;
	string            *lpParentCode;
	size_t            nParentCodeLength;
	size_t            nCodeLength;

	lpNewVariable = new_TEndWithAttribute();
	if (lpNewVariable == NULL)
		return;
	lpNewVariable->type = atVARIABLE;
	lpParentVariable = (TEndWithAttribute *)TSSGCtrl_GetAttribute(this, &stack_PTSSDir_top(ParentStack)->super, atVARIABLE);
	if (lpParentVariable != NULL)
	{
		lpParentCode = &lpParentVariable->code;
		nParentCodeLength = lpParentCode->_M_finish - lpParentCode->_M_start;
	}
	else
	{
		nParentCodeLength = 0;
	}
	nCodeLength = EndOfCode - Code;
	// semicolon(;) is not the lead and trail byte of codepage 932.
	// it can scan from backward.
	if (nParentCodeLength != 0 || nCodeLength == 0 || *(EndOfCode - 1) != ';')
	{
		LPSTR lpszCode;

		lpszCode = HeapAlloc(hHeap, 0, nParentCodeLength + nCodeLength + 2);
		if (lpszCode != NULL)
		{
			size_t length;

			if (nParentCodeLength != 0)
				__movsb(lpszCode, lpParentCode->_M_start, nParentCodeLength);
			__movsb(lpszCode + nParentCodeLength, Code, nCodeLength + 1);
			length = nParentCodeLength + nCodeLength;
			if (!length || *(lpszCode + length - 1) != ';')
				*(LPWORD)(lpszCode + length++) = BSWAP16(';\0');
			TEndWithAttribute_Setting(lpNewVariable, lpszCode, length);
			HeapFree(hHeap, 0, lpszCode);
		}
	}
	else
	{
		TEndWithAttribute_Setting(lpNewVariable, Code, EndOfCode - Code);
	}
	TSSGAttributeSelector_PushElement(&this->attributeSelector, lpNewVariable);
}

void __stdcall Attribute_variable_close(TSSGCtrl *this)
{
	TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atVARIABLE);
}
