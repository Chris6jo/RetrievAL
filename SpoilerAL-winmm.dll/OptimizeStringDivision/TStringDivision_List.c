#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

unsigned long __stdcall TStringDivision_List_WithoutTokenDtor(
	IN     TStringDivision *this,
	IN     const string    *Src,
	IN     const char      *Token,
	IN     size_t          TokenLength,
	OUT    vector_string   *List,
	IN     unsigned long   Option)
{
	const char *split;
	string     *elem;

	vector_string_clear(List);
	split = Src->_M_start;
	if (TokenLength && string_length(Src) >= TokenLength)
	{
		const char *end, *p;
		size_t     nest;

		end = Src->_M_finish - TokenLength + 1;
		p = split;
		nest = 0;
		do
		{
			char c;

			switch (*p)
			{
			case '(':
				// '\x2B' == '+'
				if (TokenLength != 3 || *(LPDWORD)Token != (DWORD)'\'\x2B\'')
					nest++;
				break;
			case ')':
				// '\x2B' == '+'
				if ((TokenLength != 3 || *(LPDWORD)Token != (DWORD)'\'\x2B\'') && nest)
					nest--;
				break;
			case '\\':
				if (!(Option & dtESCAPE))
					break;
				if (++p < end)
					goto CHECK_LEADBYTE;
				else
					goto NESTED_BREAK;
			case '"':
				if (++p >= end)
					goto NESTED_BREAK;
				while (*p != '"')
				{
					if (*p == '\\' &&  ++p >= end)
						goto NESTED_BREAK;
					if (__intrinsic_isleadbyte(*p) && ++p >= end)
						goto NESTED_BREAK;
					if (++p >= end)
						goto NESTED_BREAK;
				}
				break;
			case '<':
				if ((c = *(p + 1)) != '#' && c != '@')
					goto DEFAULT;
				if ((p += 2) >= end)
					goto NESTED_BREAK;
				while (*p != c || *(p + 1) != '>')
				{
					if (*p == '[' && *(p + 1) == '!' && TokenLength == 2 && *(LPWORD)Token == BSWAP16('[!'))
						goto MATCHED;
					if (*p == '\\' && (Option & dtESCAPE) && ++p >= end)
						goto NESTED_BREAK;
					if (__intrinsic_isleadbyte(*p) && ++p >= end)
						goto NESTED_BREAK;
					if (++p >= end)
						goto NESTED_BREAK;
				}
				if ((p += 2) < end)
					continue;
				else
					goto NESTED_BREAK;
			case '[':
				if (*(p + 1) == '!' && TokenLength == 2 && *(LPWORD)Token == BSWAP16('[!'))
					goto MATCHED;
			default:
			DEFAULT:
				if (nest)
					goto CHECK_LEADBYTE;
				if (memcmp(p, Token, TokenLength) != 0)
					goto CHECK_LEADBYTE;
			MATCHED:
				vector_string_push_back_range(List, split, p);
				elem = List->_M_finish - 1;
				if (Option & etSOME_EDIT)
				{
					string s;

					s = *elem;
					TStringDivision_Editing(elem, this, &s, Option);
					string_dtor(&s);
				}
				split = (p += TokenLength);
				continue;
			CHECK_LEADBYTE:
				if (__intrinsic_isleadbyte(*p) && ++p >= end)
					goto NESTED_BREAK;
				break;
			}
			p++;
		} while (p < end);
	}
NESTED_BREAK:
	vector_string_push_back_range(List, split, Src->_M_finish);
	elem = List->_M_finish - 1;
	if (Option & etSOME_EDIT)
	{
		string s;

		s = *elem;
		TStringDivision_Editing(elem, this, &s, Option);
		string_dtor(&s);
	}
	return vector_size(List);
}

unsigned long TStringDivision_List(
	IN     TStringDivision *this,
	IN     const string    *Src,
	IN     string          Token,
	OUT    vector_string   *List,
	IN     unsigned long   Option)
{
	unsigned long Result = TStringDivision_List_WithoutTokenDtor(
		this,
		Src,
		string_c_str(&Token),
		string_length(&Token),
		List,
		Option);
	string_dtor(&Token);
	return Result;
}
