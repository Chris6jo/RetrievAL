#include <windows.h>
#include <limits.h>

#ifdef __BORLANDC__
#pragma warn -8060
#endif

#define JISX0213(c) (       \
    (c) == '���' ? 0x82F2 : \
    (c) == '��'  ? 0x82F3 : \
    (c) == '��'  ? 0x82F4 : \
    (c) == '���' ? 0x82F5 : \
    (c) == '���' ? 0x82F6 : \
    (c) == '���' ? 0x82F7 : \
    (c) == '���' ? 0x82F8 : \
    (c) == '���' ? 0x82F9 : \
    (c) == '�J�' ? 0x8397 : \
    (c) == '�L�' ? 0x8398 : \
    (c) == '�N�' ? 0x8399 : \
    (c) == '�P�' ? 0x839A : \
    (c) == '�R�' ? 0x839B : \
    (c) == '�Z�' ? 0x839C : \
    (c) == '�c�' ? 0x839D : \
    (c) == '�g�' ? 0x839E : \
    (c) == '�N'  ? 0x83EC : \
    (c) == '�V'  ? 0x83ED : \
    (c) == '�X'  ? 0x83EE : \
    (c) == '�g'  ? 0x83EF : \
    (c) == '�k'  ? 0x83F0 : \
    (c) == '�n'  ? 0x83F1 : \
    (c) == '�q'  ? 0x83F2 : \
    (c) == '�t'  ? 0x83F3 : \
    (c) == '�w'  ? 0x83F4 : \
    (c) == '�z'  ? 0x83F5 : \
    (c) == '�v'  ? 0x83F6 : \
    (c) == '��'  ? 0x83F7 : \
    (c) == '��'  ? 0x83F8 : \
    (c) == '��'  ? 0x83F9 : \
    (c) == '��'  ? 0x83FA : \
    (c) == '��'  ? 0x83FB : \
    (c) == '��'  ? 0x83FC : \
    (c) == '���' ? 0x8492 : \
    (c) == '���' ? 0x8493 : \
    (c) == '���' ? 0x8494 : \
    (c) == '���' ? 0x8495 : 0)

int __stdcall LCMapStringJISX0213(
	IN           LCID   Locale,
	IN           DWORD  dwMapFlags,
	IN           LPCSTR lpSrcStr,
	IN           int    cchSrc,
	OUT OPTIONAL LPSTR  lpDestStr,
	IN           int    cchDest)
{
	LPSTR        lpBufferStr;
	unsigned int cchBuffer;
	HANDLE       hHeap;

	cchBuffer = cchSrc != -1 ? cchSrc : strlen(lpSrcStr) + 1;
	if (!(lpBufferStr = (LPSTR)HeapAlloc(hHeap = GetProcessHeap(), 0, cchBuffer * sizeof(char)))) {
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return 0;
	}
	memcpy(lpBufferStr, lpSrcStr, cchBuffer * sizeof(char));
	if (PRIMARYLANGID(LANGIDFROMLCID(Locale)) == LANG_JAPANESE &&
		(dwMapFlags & (LCMAP_FULLWIDTH | LCMAP_HALFWIDTH | LCMAP_KATAKANA | LCMAP_HIRAGANA)))
	{
		unsigned char  *dest, *src;
		size_t         count;
		unsigned short prev;

		src = dest = lpBufferStr;
		count = cchBuffer;
		prev = 0;
		while (count--) {
			unsigned short c;
			unsigned char  high;

			if ((c = *(src++)) >= 0x81 && (c <= 0x9F || (c >= 0xE0 && c <= 0xFC)) && count) {
				count--;
				c = (c << 8) | *(src++);
			}
			if (dwMapFlags & LCMAP_FULLWIDTH) {
				unsigned short out;

				out = 0;
				switch (c) {
				case 0x00DE/*'�' */:
				case 0x814A/*'�J'*/:
					if (dwMapFlags & LCMAP_HIRAGANA) {
						switch (prev) {
						case 0x00B3/*'�' */:
						case 0x82A4/*'��'*/:
						case 0x8345/*'�E'*/: goto FULLWIDTH_82F2;
						}
					} else if (dwMapFlags & LCMAP_KATAKANA) {
						switch (prev) {
						case 0x82A4/*'��'*/: out = 0x8394/*'��'*/; break;
						case 0x00DC/*'�' */:
						case 0x82ED/*'��'*/:
						case 0x838F/*'��'*/: goto FULLWIDTH_8492;
						case 0x82EE/*'��'*/:
						case 0x8390/*'��'*/: goto FULLWIDTH_8493;
						case 0x82EF/*'��'*/:
						case 0x8391/*'��'*/: goto FULLWIDTH_8494;
						case 0x00A6/*'�' */:
						case 0x82F0/*'��'*/:
						case 0x8392/*'��'*/: goto FULLWIDTH_8495;
						}
					} else {
						switch (prev) {
						case 0x82A4/*'��'*/: FULLWIDTH_82F2: out = 0x82F2/*JISX0213('���')*/; break;
						case 0x00DC/*'�' */:
						case 0x82ED/*'��'*/:
						case 0x838F/*'��'*/: FULLWIDTH_8492: out = 0x8492/*JISX0213('���')*/; break;
						case 0x82EE/*'��'*/:
						case 0x8390/*'��'*/: FULLWIDTH_8493: out = 0x8493/*JISX0213('���')*/; break;
						case 0x82EF/*'��'*/:
						case 0x8391/*'��'*/: FULLWIDTH_8494: out = 0x8494/*JISX0213('���')*/; break;
						case 0x00A6/*'�' */:
						case 0x82F0/*'��'*/:
						case 0x8392/*'��'*/: FULLWIDTH_8495: out = 0x8495/*JISX0213('���')*/; break;
						}
					}
					break;
				case 0x00DF/*'�' */:
				case 0x814B/*'�K'*/:
					if (dwMapFlags & LCMAP_HIRAGANA) {
						switch (prev) {
						case 0x00B6/*'�' */:
						case 0x82A9/*'��'*/:
						case 0x834A/*'�J'*/: goto FULLWIDTH_82F5;
						case 0x00B7/*'�' */:
						case 0x82AB/*'��'*/:
						case 0x834C/*'�L'*/: goto FULLWIDTH_82F6;
						case 0x00B8/*'�' */:
						case 0x82AD/*'��'*/:
						case 0x834E/*'�N'*/: goto FULLWIDTH_82F7;
						case 0x00B9/*'�' */:
						case 0x82AF/*'��'*/:
						case 0x8350/*'�P'*/: goto FULLWIDTH_82F8;
						case 0x00BA/*'�' */:
						case 0x82B1/*'��'*/:
						case 0x8352/*'�R'*/: goto FULLWIDTH_82F9;
						}
					} else if (dwMapFlags & LCMAP_KATAKANA) {
						switch (prev) {
						case 0x00B6/*'�' */:
						case 0x82A9/*'��'*/:
						case 0x834A/*'�J'*/: goto FULLWIDTH_8397;
						case 0x00B7/*'�' */:
						case 0x82AB/*'��'*/:
						case 0x834C/*'�L'*/: goto FULLWIDTH_8398;
						case 0x00B8/*'�' */:
						case 0x82AD/*'��'*/:
						case 0x834E/*'�N'*/: goto FULLWIDTH_8399;
						case 0x00B9/*'�' */:
						case 0x82AF/*'��'*/:
						case 0x8350/*'�P'*/: goto FULLWIDTH_839A;
						case 0x00BA/*'�' */:
						case 0x82B1/*'��'*/:
						case 0x8352/*'�R'*/: goto FULLWIDTH_839B;
						case 0x00BE/*'�' */:
						case 0x82B9/*'��'*/:
						case 0x835A/*'�Z'*/: goto FULLWIDTH_839C;
						case 0x00C2/*'�' */:
						case 0x82C2/*'��'*/:
						case 0x8363/*'�c'*/: goto FULLWIDTH_839D;
						case 0x00C4/*'�' */:
						case 0x82C6/*'��'*/:
						case 0x8367/*'�g'*/: goto FULLWIDTH_839E;
						}
					} else {
						switch (prev) {
						case 0x82A9/*'��'*/: FULLWIDTH_82F5: out = 0x82F5/*JISX0213('���')*/; break;
						case 0x82AB/*'��'*/: FULLWIDTH_82F6: out = 0x82F6/*JISX0213('���')*/; break;
						case 0x82AD/*'��'*/: FULLWIDTH_82F7: out = 0x82F7/*JISX0213('���')*/; break;
						case 0x82AF/*'��'*/: FULLWIDTH_82F8: out = 0x82F8/*JISX0213('���')*/; break;
						case 0x82B1/*'��'*/: FULLWIDTH_82F9: out = 0x82F9/*JISX0213('���')*/; break;
						case 0x00B6/*'�' */:
						case 0x834A/*'�J'*/: FULLWIDTH_8397: out = 0x8397/*JISX0213('�J�')*/; break;
						case 0x00B7/*'�' */:
						case 0x834C/*'�L'*/: FULLWIDTH_8398: out = 0x8398/*JISX0213('�L�')*/; break;
						case 0x00B8/*'�' */:
						case 0x834E/*'�N'*/: FULLWIDTH_8399: out = 0x8399/*JISX0213('�N�')*/; break;
						case 0x00B9/*'�' */:
						case 0x8350/*'�P'*/: FULLWIDTH_839A: out = 0x839A/*JISX0213('�P�')*/; break;
						case 0x00BA/*'�' */:
						case 0x8352/*'�R'*/: FULLWIDTH_839B: out = 0x839B/*JISX0213('�R�')*/; break;
						case 0x00BE/*'�' */:
						case 0x835A/*'�Z'*/: FULLWIDTH_839C: out = 0x839C/*JISX0213('�Z�')*/; break;
						case 0x00C2/*'�' */:
						case 0x8363/*'�c'*/: FULLWIDTH_839D: out = 0x839D/*JISX0213('�c�')*/; break;
						case 0x00C4/*'�' */:
						case 0x8367/*'�g'*/: FULLWIDTH_839E: out = 0x839E/*JISX0213('�g�')*/; break;
						}
					}
					break;
				}
				if (out) {
					size_t is_wide;

					dest -= (is_wide = prev > UCHAR_MAX) + 1;
					cchBuffer -= is_wide + (c > UCHAR_MAX);
					c = out;
				}
			} else if (dwMapFlags & LCMAP_HIRAGANA) {
				switch (c) {
				case 0x8394/*'��'           */: c = 0x82F2/*JISX0213('���')*/; break;
				case 0x8395/*'��'           */: c = 0x82F3/*JISX0213('��' )*/; break;
				case 0x8396/*'��'           */: c = 0x82F4/*JISX0213('��' )*/; break;
				case 0x8397/*JISX0213('�J�')*/: c = 0x82F5/*JISX0213('���')*/; break;
				case 0x8398/*JISX0213('�L�')*/: c = 0x82F6/*JISX0213('���')*/; break;
				case 0x8399/*JISX0213('�N�')*/: c = 0x82F7/*JISX0213('���')*/; break;
				case 0x839A/*JISX0213('�P�')*/: c = 0x82F8/*JISX0213('���')*/; break;
				case 0x839B/*JISX0213('�R�')*/: c = 0x82F9/*JISX0213('���')*/; break;
				}
			} else if (dwMapFlags & LCMAP_KATAKANA) {
				if (!(dwMapFlags & LCMAP_HALFWIDTH)) {
					switch (c) {
					case 0x82F2/*JISX0213('���')*/: c = 0x8394/*'��'           */; break;
					case 0x82F3/*JISX0213('��' )*/: c = 0x8395/*'��'           */; break;
					case 0x82F4/*JISX0213('��' )*/: c = 0x8396/*'��'           */; break;
					case 0x82F5/*JISX0213('���')*/: c = 0x8397/*JISX0213('�J�')*/; break;
					case 0x82F6/*JISX0213('���')*/: c = 0x8398/*JISX0213('�L�')*/; break;
					case 0x82F7/*JISX0213('���')*/: c = 0x8399/*JISX0213('�N�')*/; break;
					case 0x82F8/*JISX0213('���')*/: c = 0x839A/*JISX0213('�P�')*/; break;
					case 0x82F9/*JISX0213('���')*/: c = 0x839B/*JISX0213('�R�')*/; break;
					}
				} else {
					switch (c) {
					case 0x82F2/*JISX0213('���')*/: c = 0xB3DE/*'��'*/; break;
					case 0x82F5/*JISX0213('���')*/:
					case 0x8397/*JISX0213('�J�')*/: goto HALFWIDTH_B6DF;
					case 0x82F6/*JISX0213('���')*/:
					case 0x8398/*JISX0213('�L�')*/: goto HALFWIDTH_B7DF;
					case 0x82F7/*JISX0213('���')*/:
					case 0x8399/*JISX0213('�N�')*/: goto HALFWIDTH_B8DF;
					case 0x82F8/*JISX0213('���')*/:
					case 0x839A/*JISX0213('�P�')*/: goto HALFWIDTH_B9DF;
					case 0x82F9/*JISX0213('���')*/:
					case 0x839B/*JISX0213('�R�')*/: goto HALFWIDTH_BADF;
					case 0x839C/*JISX0213('�Z�')*/: goto HALFWIDTH_BEDF;
					case 0x839D/*JISX0213('�c�')*/: goto HALFWIDTH_C2DF;
					case 0x839E/*JISX0213('�g�')*/: goto HALFWIDTH_C4DF;
					case 0x8492/*JISX0213('���')*/: goto HALFWIDTH_DCDE;
					case 0x8495/*JISX0213('���')*/: goto HALFWIDTH_A6DE;
					}
				}
			} else if (dwMapFlags & LCMAP_HALFWIDTH) {
				switch (c) {
				case 0x8397/*JISX0213('�J�')*/: HALFWIDTH_B6DF: c = 0xB6DF/*'��'*/; break;
				case 0x8398/*JISX0213('�L�')*/: HALFWIDTH_B7DF: c = 0xB7DF/*'��'*/; break;
				case 0x8399/*JISX0213('�N�')*/: HALFWIDTH_B8DF: c = 0xB8DF/*'��'*/; break;
				case 0x839A/*JISX0213('�P�')*/: HALFWIDTH_B9DF: c = 0xB9DF/*'��'*/; break;
				case 0x839B/*JISX0213('�R�')*/: HALFWIDTH_BADF: c = 0xBADF/*'��'*/; break;
				case 0x839C/*JISX0213('�Z�')*/: HALFWIDTH_BEDF: c = 0xBEDF/*'��'*/; break;
				case 0x839D/*JISX0213('�c�')*/: HALFWIDTH_C2DF: c = 0xC2DF/*'��'*/; break;
				case 0x839E/*JISX0213('�g�')*/: HALFWIDTH_C4DF: c = 0xC4DF/*'��'*/; break;
				case 0x8492/*JISX0213('���')*/: HALFWIDTH_DCDE: c = 0xDCDE/*'��'*/; break;
				case 0x8495/*JISX0213('���')*/: HALFWIDTH_A6DE: c = 0xA6DE/*'��'*/; break;
				}
			}
			if (high = (unsigned char)((prev = c) >> 8))
				*(dest++) = high;
			*(dest++) = (unsigned char)c;
		}
	}
	cchDest = LCMapStringA(Locale, dwMapFlags, lpBufferStr, cchBuffer, lpDestStr, cchDest);
	HeapFree(hHeap, 0, lpBufferStr);
	return cchDest;
}

