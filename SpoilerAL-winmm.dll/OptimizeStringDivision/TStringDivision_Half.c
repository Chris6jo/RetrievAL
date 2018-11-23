#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

string * __cdecl TStringDivision_Half(
	OUT string          *Result,
	IN  TStringDivision *this,
	OUT string          *Src,
	IN  string          Token,
	IN  unsigned long   Index,
	IN  unsigned long   Option)
{
	size_t     srcLength;
	size_t     tokenLength;
	const char *lastFound, *end, *p;
	size_t     nest;
	size_t     length;

	if (!Src)
		goto FAILED;
	tokenLength = string_length(&Token);
	if (!tokenLength)
		goto FAILED;
	srcLength = string_length(Src);
	if (srcLength < tokenLength)
		goto FAILED;
	lastFound = NULL;
	p = Src->_M_start;
	end = Src->_M_finish - tokenLength + 1;
	nest = 0;
	do
	{
		char c;

		switch (*p)
		{
		case '(':
			nest++;
			break;
		case ')':
			if (nest)
				nest--;
			break;
		case '\\':
			if (!(Option & dtESCAPE))
				break;
			if (++p < end)
				goto CHECK_LEADBYTE;
			else
				goto FAILED;
		case '"':
			if (++p >= end)
				goto FAILED;
			while (*p != '"')
			{
				if (*p == '\\' && ++p >= end)
					goto FAILED;
				if (__intrinsic_isleadbyte(*p) && ++p >= end)
					goto FAILED;
				if (++p >= end)
					goto FAILED;
			}
			break;
		case '<':
			if ((c = *(p + 1)) != '#' && c != '@')
				goto DEFAULT;
			if ((p += 2) >= end)
				goto FAILED;
			while (*p != c || *(p + 1) != '>')
			{
				if (*p == '\\' && (Option & dtESCAPE) && ++p >= end)
					goto FAILED;
				if (__intrinsic_isleadbyte(*p) && ++p >= end)
					goto FAILED;
				if (++p >= end)
					goto FAILED;
			}
			if ((p += 2) < end)
				continue;
			else
				goto FAILED;
		case '!':
			if (*(p + 1) == ']' && tokenLength == 2 && *(LPWORD)Token._M_start == BSWAP16('!]'))
				goto MATCHED;
		default:
		DEFAULT:
			if (memcmp(p, Token._M_start, tokenLength) != 0)
				goto CHECK_LEADBYTE;
			lastFound = p;
			if (nest)
				goto CHECK_LEADBYTE;
		MATCHED:
			if (!Index)
				goto SUCCESS;
			Index--;
			if ((p += tokenLength) < end)
				continue;
			else
				goto FAILED;
		CHECK_LEADBYTE:
			if (__intrinsic_isleadbyte(*p) && ++p >= end)
				goto FAILED;
			break;
		}
		p++;
	} while (p < end);
FAILED:
	if (*(LPWORD)Token._M_start == BSWAP16(':\0') && lastFound)
	{
		p = lastFound;
		goto SUCCESS;
	}
	string_ctor_assign(Result, &Token);
	string_dtor(&Token);
	return Result;

SUCCESS:
	string_ctor_assign_cstr_with_length(Result, Src->_M_start, p - Src->_M_start);
	p += tokenLength;
	length = Src->_M_finish - p;
	Src->_M_finish = Src->_M_start + length;
	memcpy(Src->_M_start, p, length + 1);
	if (Option & etSOME_EDIT)
	{
		string s;

		s = *Src;
		TStringDivision_Editing(Src, this, &s, Option);
		string_dtor(&s);
		s = *Result;
		TStringDivision_Editing(Result, this, &s, Option);
		string_dtor(&s);
	}
	string_dtor(&Token);
	return Result;
}
