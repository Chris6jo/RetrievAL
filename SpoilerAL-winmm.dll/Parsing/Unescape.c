#include <windows.h>
#include "atoitbl.h"

char * __fastcall UnescapeA(char *first, char **plast, BOOL breakSingleQuate)
{
	unsigned char *p, *last;

	if ((p = first) < (last = *plast))
	{
		for (; ; )
		{
			unsigned char c, *src, x;
			size_t        size;

			if ((c = *(p++)) != '\\')
			{
				if (c != '"' && (c != '\'' || !breakSingleQuate))
					if (p < last && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c) || ++p < last))
						continue;
					else
						break;
				p--;
				break;
			}
			if ((src = p--) >= last)
				break;
			switch (c = *(src++))
			{
			case '0':
				*(p++) = '\0';
				break;
			case 'a':
				*(p++) = '\a';
				break;
			case 'b':
				*(p++) = '\b';
				break;
			case 'f':
				*(p++) = '\f';
				break;
			case 'n':
				*(p++) = '\n';
				break;
			case 'r':
				*(p++) = '\r';
				break;
			case 't':
				*(p++) = '\t';
				break;
			case 'v':
				*(p++) = '\v';
				break;
			case '1': case '2': case '3': case '4': case '5': case '6': case '7':
				c -= '0';
				while (++src < last && (x = *src - '0') < '7' - '0' + 1)
					c = c * 8 + x;
				goto PUTCHAR;
			case 'x':
				if (src < last)
				{
					x = *src;
					if (ACTOI(&x, 'f', 16))
					{
						c = x;
						while (++src < last)
						{
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							c = c * 0x10 + x;
						}
					}
				}
				goto PUTCHAR;
			case 'U':
			case 'u':
				if (src < last)
				{
					x = *src;
					if (ACTOI(&x, 'f', 16))
					{
						wchar_t w;

						w = x;
						while (++src < last)
						{
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							w = w * 0x10 + x;
						}
						p += (unsigned int)WideCharToMultiByte(CP_THREAD_ACP, 0, &w, 1, p, 2, NULL, NULL);
						break;
					}
				}
			PUTCHAR:
				*(p++) = c;
				break;
			default:
				*(p++) = c;
				if (src < last && IsDBCSLeadByteEx(CP_THREAD_ACP, c))
					*(p++) = *(src++);
				break;
			}
			if (!(size = (last -= src - p) - p))
				break;
			memcpy(p, src, size);
		}
		*plast = last;
	}
	return p;
}

wchar_t * __fastcall UnescapeW(wchar_t *first, wchar_t **plast, BOOL breakSingleQuate)
{
	wchar_t *p, *last;

	if ((p = first) < (last = *plast))
	{
		for (; ; )
		{
			wchar_t c, *src, x;
			size_t  size;

			if ((c = *(p++)) != L'\\')
			{
				if (c != L'"' && (c != L'\'' || !breakSingleQuate))
					if (p < last)
						continue;
					else
						break;
				p--;
				break;
			}
			if ((src = p--) >= last)
				break;
			switch (c = *(src++))
			{
			case L'0':
				*(p++) = L'\0';
				break;
			case L'a':
				*(p++) = L'\a';
				break;
			case L'b':
				*(p++) = L'\b';
				break;
			case L'f':
				*(p++) = L'\f';
				break;
			case L'n':
				*(p++) = L'\n';
				break;
			case L'r':
				*(p++) = L'\r';
				break;
			case L't':
				*(p++) = L'\t';
				break;
			case L'v':
				*(p++) = L'\v';
				break;
			case L'1': case L'2': case L'3': case L'4': case L'5': case L'6': case L'7':
				c -= L'0';
				while (++src < last && (x = *src - L'0') < L'7' - L'0' + 1)
					c = c * 8 + x;
				goto DEFAULT;
			case L'U':
			case L'u':
			case L'x':
				if (src < last)
				{
					x = *src;
					if (WCTOI(&x, L'f', 16))
					{
						c = x;
						while (++src < last)
						{
							x = *src;
							if (!WCTOI(&x, L'f', 16))
								break;
							c = c * 0x10 + x;
						}
					}
				}
			default:
			DEFAULT:
				*(p++) = c;
				break;
			}
			if (!(size = ((char *)last -= (char *)src - (char *)p) - (char *)p))
				break;
			memcpy(p, src, size);
		}
		*plast = last;
	}
	return p;
}

unsigned char * __fastcall UnescapeU(unsigned char *first, unsigned char **plast, BOOL breakSingleQuate)
{
	unsigned char *p, *last;

	if ((p = first) < (last = *plast))
	{
		for (; ; )
		{
			unsigned char c, *src, x;
			size_t        size;

			if ((c = *(p++)) != '\\')
			{
				if (c != '"' && (c != '\'' || !breakSingleQuate))
					if (p < last)
						continue;
					else
						break;
				p--;
				break;
			}
			if ((src = p--) >= last)
				break;
			switch (c = *(src++))
			{
			case '0':
				*(p++) = '\0';
				break;
			case 'a':
				*(p++) = '\a';
				break;
			case 'b':
				*(p++) = '\b';
				break;
			case 'f':
				*(p++) = '\f';
				break;
			case 'n':
				*(p++) = '\n';
				break;
			case 'r':
				*(p++) = '\r';
				break;
			case 't':
				*(p++) = '\t';
				break;
			case 'v':
				*(p++) = '\v';
				break;
			case '1': case '2': case '3': case '4': case '5': case '6': case '7':
				c -= '0';
				while (++src < last && (x = *src - '0') < '7' - '0' + 1)
					c = c * 8 + x;
				goto DEFAULT;
			case 'U':
			case 'u':
			case 'x':
				if (src < last)
				{
					if (ACTOI(&x, 'f', 16))
					{
						unsigned long u;

						u = x;
						while (++src < last)
						{
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							u = u * 0x10 + x;
						}
						do
							*(p++) = (unsigned char)u;
						while (u >>= 8);
						break;
					}
				}
			default:
			DEFAULT:
				*(p++) = c;
				break;
			}
			if (!(size = (last -= src - p) - p))
				break;
			memcpy(p, src, size);
		}
		*plast = last;
	}
	return p;
}

__int64 __fastcall UnescapeAnsiCharA(const char **pfirst, const char *last)
{
	unsigned long       n;
	size_t              length;
	const unsigned char *p, *src;
	unsigned char       c;

	n = 0;
	length = 0;
	for (p = *pfirst; (src = p) < last; n = n * 0x100 + c, length++)
	{
		unsigned char       x, lpMultiByteStr[2];
		wchar_t             w;
		unsigned int        cbMultiByte;
		const unsigned char *xptr;

		if ((c = *(p++)) != '\\')
			if (c != '\'')
				goto DEFAULT;
			else
				break;
		if (p >= last)
			break;
		switch (c = *(p++))
		{
		default:
		DEFAULT:
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
				continue;
			n = n * 0x100 + c;
			length++;
			if ((src = p) >= last)
				break;
			c = *(p++);
			continue;
		case '0':
			c = '\0';
			continue;
		case 'a':
			c = '\a';
			continue;
		case 'b':
			c = '\b';
			continue;
		case 'f':
			c = '\f';
			continue;
		case 'n':
			c = '\n';
			continue;
		case 'r':
			c = '\r';
			continue;
		case 't':
			c = '\t';
			continue;
		case 'v':
			c = '\v';
			continue;
		case '1': case '2': case '3': case '4': case '5': case '6': case '7':
			c -= '0';
			while (++p < last && (x = *p - '0') < '7' - '0' + 1)
				c = c * 8 + x;
			continue;
		case 'x':
			if (p >= last)
				continue;
			x = *p;
			if (!ACTOI(&x, 'f', 16))
				continue;
			c = x;
			while (++p < last)
			{
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					break;
				c = c * 0x10 + x;
			}
			continue;
		case 'U':
		case 'u':
			if (p >= last)
				continue;
			x = *p;
			if (!ACTOI(&x, 'f', 16))
				continue;
			w = x;
			while (++p < last)
			{
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					break;
				w = w * 0x10 + x;
			}
			cbMultiByte = WideCharToMultiByte(CP_THREAD_ACP, 0, &w, 1, lpMultiByteStr, 2, NULL, NULL);
			if (cbMultiByte < 1)
				break;
			xptr = lpMultiByteStr;
			if (cbMultiByte != 1)
			{
				n = n * 0x100 + *(xptr++);
				length++;
			}
			c = *xptr;
			continue;
		}
		break;
	}
	*pfirst = src;
	return (length != 1 || (char)n >= 0) ? n : (~(__int64)0xFF | n);
}

unsigned long __fastcall UnescapeUnicodeCharA(const char **pfirst, const char *last)
{
	unsigned long       n;
	const unsigned char *p, *src;
	wchar_t             w;

	n = 0;
	for (p = *pfirst; (src = p) < last; n = n * 0x10000 + w)
	{
		unsigned char c, x;
		unsigned int  cbMultiByte;

		if ((c = *(p++)) != '\\')
			if (c != '\'')
				goto DEFAULT;
			else
				break;
		if (p >= last)
			break;
		switch (c = *(p++))
		{
		default:
		DEFAULT:
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
				cbMultiByte = 1;
			else if (p < last)
				cbMultiByte = 2;
			else
				break;
			if (!MultiByteToWideChar(CP_THREAD_ACP, 0, --p, cbMultiByte, &w, 1))
				break;
			p += cbMultiByte;
			continue;
		case '0':
			w = L'\0';
			continue;
		case 'a':
			w = L'\a';
			continue;
		case 'b':
			w = L'\b';
			continue;
		case 'f':
			w = L'\f';
			continue;
		case 'n':
			w = L'\n';
			continue;
		case 'r':
			w = L'\r';
			continue;
		case 't':
			w = L'\t';
			continue;
		case 'v':
			w = L'\v';
			continue;
		case '1': case '2': case '3': case '4': case '5': case '6': case '7':
			w = c - '0';
			while (++p < last && (x = *p - '0') < '7' - '0' + 1)
				w = w * 8 + x;
			continue;
		case 'U':
		case 'u':
		case 'x':
			w = c;
			if (p >= last)
				continue;
			x = *p;
			if (!ACTOI(&x, 'f', 16))
				continue;
			w = x;
			while (++p < last)
			{
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					break;
				w = w * 0x10 + x;
			}
			continue;
		}
		break;
	}
	*pfirst = src;
	return n;
}

unsigned long __fastcall UnescapeUtf8CharA(const char **pfirst, const char *last)
{
	unsigned long       n, u, bits;
	const unsigned char *p, *src;

	n = 0;
	for (p = *pfirst; (src = p) < last; n = (n << bits) + u)
	{
		unsigned char c, x;
		unsigned int  cbMultiByte, cbUtf8;
		wchar_t       w;

		if ((c = *(p++)) != '\\')
			if (c != '\'')
				goto DEFAULT;
			else
				break;
		if (p >= last)
			break;
		bits = 8;
		switch (c = *(p++))
		{
		default:
		DEFAULT:
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
				cbMultiByte = 1;
			else if (p < last)
				cbMultiByte = 2;
			else
				break;
			if (!MultiByteToWideChar(CP_THREAD_ACP, 0, --p, cbMultiByte, &w, 1))
				break;
			p += cbMultiByte;
			cbUtf8 = WideCharToMultiByte(CP_UTF8, 0, &w, 1, (char *)&u, sizeof(u), NULL, NULL);
			if (bits = cbUtf8 * 8)
				continue;
			break;
		case '0':
			u = '\0';
			continue;
		case 'a':
			u = '\a';
			continue;
		case 'b':
			u = '\b';
			continue;
		case 'f':
			u = '\f';
			continue;
		case 'n':
			u = '\n';
			continue;
		case 'r':
			u = '\r';
			continue;
		case 't':
			u = '\t';
			continue;
		case 'v':
			u = '\v';
			continue;
		case '1': case '2': case '3': case '4': case '5': case '6': case '7':
			u = c - '0';
			while (++p < last && (x = *p - '0') < '7' - '0' + 1)
				u = u * 8 + x;
			continue;
		case 'U':
		case 'u':
		case 'x':
			u = c;
			if (p >= last)
				continue;
			x = *p;
			if (!ACTOI(&x, 'f', 16))
				continue;
			if (!(u = x))
			{
				do
					if (++p >= last)
						goto CONTINUE;
				while ((x = *p) == '0');
				u = x;
			}
			while (++p < last)
			{
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					break;
				u = u * 0x10 + x;
				bits += 8;
			}
			if (bits > 32)
				bits = 32;
			continue;
		}
		break;
	CONTINUE:;
	}
	*pfirst = src;
	return n;
}
