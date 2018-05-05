#include <windows.h>
#include <limits.h>
#include "intrinsic.h"

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
    (c) == '���' ? 0x848F : \
    (c) == '���' ? 0x8490 : \
    (c) == '���' ? 0x8491 : \
    (c) == '���' ? 0x8492 : 0)

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
		(dwMapFlags & (LCMAP_FULLWIDTH | LCMAP_HALFWIDTH | LCMAP_KATAKANA | LCMAP_HIRAGANA)) &&
		cchBuffer > 1)
	{
		unsigned char *p;
		ptrdiff_t     offset;
		unsigned int  prev, c;
		size_t        erase;

		p = lpBufferStr + (cchBuffer - 1);
		offset = -(ptrdiff_t)(cchBuffer - 1);
		prev = 0;
		do {
			if ((c = p[offset]) >= 0x81 && (c <= 0x9F || (c >= 0xE0 && c <= 0xFC)))
				c = (c << 8) | p[offset + 1];
			if (dwMapFlags & LCMAP_FULLWIDTH) {
				erase = 0;
				switch (c) {
				case 0x00DE/*'�'*/:
				case 0x814A/*'�J'*/:
					switch (prev) {
					case 0x00B3/*'�'*/:
						if (!(dwMapFlags & LCMAP_HIRAGANA))
							break;
						erase = c > UCHAR_MAX;
						*(unsigned short *)&p[offset - 1] = BSWAP16(0x82F2/*JISX0213('���')*/);
						break;
					case 0x00DC/*'�'*/:
					case 0x00A6/*'�'*/:
						erase = c > UCHAR_MAX;
						*(unsigned short *)&p[offset - 1] = _byteswap_ushort(
							(0x00DC/*'�'*/ - prev) / 16 + 0x8492/*JISX0213('���')*/);
						break;
					case 0x82A4/*'��'*/:
						erase = c > UCHAR_MAX ? 2 : 1;
						*(unsigned short *)&p[offset - 2] = !(dwMapFlags & LCMAP_KATAKANA) ?
							BSWAP16(0x82F2/*JISX0213('���')*/) :
							BSWAP16(0x8394/*'��'*/);
						break;
					case 0x82ED/*'��'*/:
					case 0x82EE/*'��'*/:
					case 0x82EF/*'��'*/:
					case 0x82F0/*'��'*/:
						if (!(dwMapFlags & LCMAP_KATAKANA))
							break;
						erase = c > UCHAR_MAX ? 2 : 1;
						*(unsigned short *)&p[offset - 2] = _byteswap_ushort(
							prev - 0x82ED/*'��'*/ + 0x8492/*JISX0213('���')*/);
						break;
					case 0x8345/*'�E'*/:
						if (!(dwMapFlags & LCMAP_HIRAGANA))
							break;
						erase = c > UCHAR_MAX ? 2 : 1;
						*(unsigned short *)&p[offset - 2] = BSWAP16(0x82F2/*JISX0213('���')*/);
						break;
					case 0x838F/*'��'*/:
					case 0x8390/*'��'*/:
					case 0x8391/*'��'*/:
					case 0x8392/*'��'*/:
						erase = c > UCHAR_MAX ? 2 : 1;
						*(unsigned short *)&p[offset - 2] = _byteswap_ushort(
							prev - 0x838F/*'��'*/ + 0x8492/*JISX0213('���')*/);
						break;
					}
					break;
				case 0x00DF/*'�'*/:
				case 0x814B/*'�K'*/:
					switch (prev) {
					case 0x00B6/*'�'*/:
					case 0x00B7/*'�'*/:
					case 0x00B8/*'�'*/:
					case 0x00B9/*'�'*/:
					case 0x00BA/*'�'*/:
						erase = c > UCHAR_MAX;
						*(unsigned short *)&p[offset - 1] = _byteswap_ushort(
							prev + ((dwMapFlags & LCMAP_HIRAGANA) ?
								0x82F5/*JISX0213('���')*/ - 0x00B6/*'�'*/ :
								0x8397/*JISX0213('�J�')*/ - 0x00B6/*'�'*/));
						break;
					case 0x00BE/*'�'*/:
					case 0x00C2/*'�'*/:
					case 0x00C4/*'�'*/:
						if (dwMapFlags & LCMAP_HIRAGANA)
							break;
						erase = c > UCHAR_MAX;
						*(unsigned short *)&p[offset - 1] = _byteswap_ushort(
							(prev - 0x00BE/*'�'*/ + 2) / 4 + 0x839C/*JISX0213('�Z�')*/);
						break;
					case 0x82A9/*'��'*/:
					case 0x82AB/*'��'*/:
					case 0x82AD/*'��'*/:
					case 0x82AF/*'��'*/:
					case 0x82B1/*'��'*/:
						erase = c > UCHAR_MAX ? 2 : 1;
						*(unsigned short *)&p[offset - 2] = _byteswap_ushort(
							(prev - 0x82A9/*'��'*/) / 2 + (!(dwMapFlags & LCMAP_KATAKANA) ?
								0x82F5/*JISX0213('���')*/ :
								0x8397/*JISX0213('�J�')*/));
						break;
					case 0x82B9/*'��'*/:
					case 0x82C2/*'��'*/:
					case 0x82C6/*'��'*/:
						if (!(dwMapFlags & LCMAP_KATAKANA))
							break;
						erase = c > UCHAR_MAX ? 2 : 1;
						*(unsigned short *)&p[offset - 2] = _byteswap_ushort(
							(prev - 0x82B9/*'��'*/ + 3) / 8 + 0x839C/*JISX0213('�Z�')*/);
						break;
					case 0x834A/*'�J'*/:
					case 0x834C/*'�L'*/:
					case 0x834E/*'�N'*/:
					case 0x8350/*'�P'*/:
					case 0x8352/*'�R'*/:
						erase = c > UCHAR_MAX ? 2 : 1;
						*(unsigned short *)&p[offset - 2] = _byteswap_ushort(
							(prev - 0x834A/*'�J'*/) / 2 + ((dwMapFlags & LCMAP_HIRAGANA) ?
								0x82F5/*JISX0213('���')*/ :
								0x8397/*JISX0213('�J�')*/));
						break;
					case 0x835A/*'�Z'*/:
					case 0x8363/*'�c'*/:
					case 0x8367/*'�g'*/:
						if (dwMapFlags & LCMAP_HIRAGANA)
							break;
						erase = c > UCHAR_MAX ? 2 : 1;
						*(unsigned short *)&p[offset - 2] = _byteswap_ushort(
							(prev - 0x835A/*'�Z'*/ + 3) / 8 + 0x839C/*JISX0213('�Z�')*/);
						break;
					}
					break;
				}
				if (erase) {
					cchBuffer -= erase;
					offset += erase;
					p -= erase;
					memcpy(p + offset, p + offset + erase, -offset + 1);
					if (!offset)
						break;
				}
			} else if (dwMapFlags & LCMAP_KATAKANA) {
				switch (c) {
				case 0x82F2/*JISX0213('���')*/:
					*(unsigned short *)&p[offset] = !(dwMapFlags & LCMAP_HALFWIDTH) ?
						BSWAP16(0x8394/*'��'*/) :
						BSWAP16(0xB3DE/*'��'*/);
					break;
				case 0x82F3/*JISX0213('��')*/:
					*(unsigned short *)&p[offset] = BSWAP16(0x8395/*'��'*/);
					break;
				case 0x82F4/*JISX0213('��')*/:
					*(unsigned short *)&p[offset] = BSWAP16(0x8396/*'��'*/);
					break;
				case 0x82F5/*JISX0213('���')*/:
				case 0x82F6/*JISX0213('���')*/:
				case 0x82F7/*JISX0213('���')*/:
				case 0x82F8/*JISX0213('���')*/:
				case 0x82F9/*JISX0213('���')*/:
					*(unsigned short *)&p[offset] = !(dwMapFlags & LCMAP_HALFWIDTH) ?
						_byteswap_ushort(c - 0x82F5/*JISX0213('���')*/ + 0x8397/*JISX0213('�J�')*/) :
						c - 0x82F5/*JISX0213('���')*/ + BSWAP16(0xB6DF/*'��'*/);
					break;
				}
			} else if (dwMapFlags & LCMAP_HIRAGANA) {
				switch (c) {
				case 0x8394/*'��'*/:
				case 0x8395/*'��'*/:
				case 0x8396/*'��'*/:
				case 0x8397/*JISX0213('�J�')*/:
				case 0x8398/*JISX0213('�L�')*/:
				case 0x8399/*JISX0213('�N�')*/:
				case 0x839A/*JISX0213('�P�')*/:
				case 0x839B/*JISX0213('�R�')*/:
					*(unsigned short *)&p[offset] = _byteswap_ushort(
						c - 0x8394/*'��'*/ + 0x82F2/*JISX0213('���')*/);
					break;
				}
			} else if (dwMapFlags & LCMAP_HALFWIDTH) {
				switch (c) {
				case 0x8394/*'��'*/:
					*(unsigned short *)&p[offset] = BSWAP16(0xB3DE/*'��'*/);
					break;
				case 0x8397/*JISX0213('�J�')*/:
				case 0x8398/*JISX0213('�L�')*/:
				case 0x8399/*JISX0213('�N�')*/:
				case 0x839A/*JISX0213('�P�')*/:
				case 0x839B/*JISX0213('�R�')*/:
					*(unsigned short *)&p[offset] = c - 0x8397/*JISX0213('�J�')*/ + BSWAP16(0xB6DF/*'��'*/);
					break;
				case 0x839C/*JISX0213('�Z�')*/:
					*(unsigned short *)&p[offset] = BSWAP16(0xBEDF/*'��'*/);
					break;
				case 0x839D/*JISX0213('�c�')*/:
					*(unsigned short *)&p[offset] = BSWAP16(0xC2DF/*'��'*/);
					break;
				case 0x839E/*JISX0213('�g�')*/:
					*(unsigned short *)&p[offset] = BSWAP16(0xC4DF/*'��'*/);
					break;
				case 0x8492/*JISX0213('���')*/:
					*(unsigned short *)&p[offset] = BSWAP16(0xDCDE/*'��'*/);
					break;
				case 0x8495/*JISX0213('���')*/:
					*(unsigned short *)&p[offset] = BSWAP16(0xA6DE/*'��'*/);
					break;
				}
			}
			if (c > UCHAR_MAX && !++offset)
				break;
			prev = c;
		} while (++offset);
	}
	cchDest = LCMapStringA(Locale, dwMapFlags, lpBufferStr, cchBuffer, lpDestStr, cchDest);
	HeapFree(hHeap, 0, lpBufferStr);
	return cchDest;
}

