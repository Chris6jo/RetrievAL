#include <windows.h>
#include <assert.h>
#include "intrinsic.h"

#if (!defined(_MSC_VER) || _MSC_VER < 1200) && !defined(__assume)
#define __assume(expression)
#endif

#ifdef __BORLANDC__
#ifndef SIZE_MAX
#ifdef _WIN64
#define SIZE_MAX _UI64_MAX
#else
#define SIZE_MAX UINT_MAX
#endif
#endif
#define vector_string                                 vector<string>
#define vector_string_clear(v)                        (v)->clear()
#define vector_string_push_back_range(v, first, last) (v)->push_back(string(first, last))
#else
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#endif

extern HANDLE hHeap;

#ifdef __BORLANDC__
unsigned long TStringFiler::LoadFromFile(
#else
unsigned long __cdecl TStringFiler_LoadFromFile(
#endif
	vector_string  *SList,
	const char     *FileName,
	unsigned long  GetSize,
	unsigned long  Mode,
	unsigned long  StartPos,
	unsigned long  EndPos,
	const char     *StartWord,
	const char     *EndWord)
{
	#define MODE_END_LINE        0x0001
	#define MODE_END_STR         0x0002
	#define MODE_END_BYTE        0x0004
	#define MODE_START_BYTE      0x0010
	#define MODE_LINE_FEED_COUNT 0x0100
	#define READ_BLOCK_SIZE      0x00010000

	HANDLE    hFile;
	DWORD     dwFileSize;
	LPSTR     tmpS;
	size_t    tmpSLength;
	size_t    tmpSBufferSize;
	ptrdiff_t nDifference;
	DWORD     dwNumberOfBytesRead;

	assert((Mode & MODE_END_LINE) == 0);
	assert((Mode & MODE_END_STR) == 0);
	assert(*StartWord == '\0');
	assert(*EndWord == '\0');

	__assume(GetSize == 0xFFFF);
	__assume((Mode & MODE_END_LINE) == 0);
	__assume((Mode & MODE_END_STR) == 0);
	__assume(*StartWord == '\0');
	__assume(*EndWord == '\0');

	hFile = CreateFileA(
		FileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);

	// �ǂݍ��݃G���[
	if (hFile == INVALID_HANDLE_VALUE)
		goto FAILED1;

	dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == MAXDWORD)
		goto FAILED2;

	// ��U��ɂȂ�܂�
	vector_string_clear(SList);

	if (dwFileSize == 0)
		goto DONE;

	if (EndPos > dwFileSize)
		EndPos = dwFileSize;

	// �O�X�L�b�v
	if ((Mode & MODE_START_BYTE) && StartPos)
	{
		DWORD dwFilePosition;

		// �t�@�C���T�C�Y���傫����c
		if (EndPos <= StartPos)
			goto DONE;
		// �o�C�g���w��
		dwFilePosition = SetFilePointer(hFile, StartPos, NULL, FILE_BEGIN);
		if (dwFilePosition == INVALID_SET_FILE_POINTER)
			goto DONE;
	}

	tmpS = (char *)HeapAlloc(hHeap, 0, READ_BLOCK_SIZE * 2);
	if (tmpS == NULL)
		goto FAILED2;
	tmpSLength = 0;
	tmpSBufferSize = READ_BLOCK_SIZE * 2;

	nDifference = 0;
	while (ReadFile(hFile, tmpS + tmpSLength, READ_BLOCK_SIZE, &dwNumberOfBytesRead, NULL) && dwNumberOfBytesRead)
	{
		LPSTR  p, end, line;
		size_t length, index;

		//--------------
		// ���s�Ő؂蕪��
		p = tmpS + tmpSLength + nDifference;
		tmpSLength += dwNumberOfBytesRead;
		end = tmpS + tmpSLength;
		*end = '\0';

		line = tmpS;
		while (p < end)
		{
			char c;

			c = *p;
			if (!__intrinsic_isleadbyte(c))
			{
				LPSTR next;

				next = p + 1;
				switch (c)
				{
				case '\r':
					if (next == end)
						goto NESTED_BREAK;
					if (*next == '\n')
						next++;
				case '\n':
					vector_string_push_back_range(SList, line, !(Mode & MODE_LINE_FEED_COUNT) ? p : next);
					line = p = next;
					break;
				case '\\':
					if (next == end)
						goto NESTED_BREAK;
					c = *(next++);
					switch (c)
					{
					case '\r':
						if (next == end)
							goto NESTED_BREAK;
						if (*next == '\n')
							next++;
					case '\n':
						memcpy(p, next, end - next + 1);
						length = next - p;
						tmpSLength -= length;
						end -= length;
						break;
					default:
						p = next;
						if (__intrinsic_isleadbyte(c))
							p++;
						break;
					}
					break;
				default:
					p = next;
					break;
				}
			}
			else
			{
				p += 2;
			}
		}
	NESTED_BREAK:
		nDifference = p - end;

		index = line - tmpS;
		if (tmpSLength -= index)
		{
			size_t nRequired;

			nRequired = tmpSLength + READ_BLOCK_SIZE + 1;
			if (nRequired > tmpSBufferSize)
			{
				LPVOID lpMem;

				if ((INT_PTR)tmpSBufferSize >= 0)
					tmpSBufferSize <<= 1;
				else
					tmpSBufferSize = nRequired;
				lpMem = HeapReAlloc(hHeap, 0, tmpS, tmpSBufferSize);
				if (lpMem == NULL)
					goto FAILED3;
				tmpS = (LPSTR)lpMem;
				line = (LPSTR)lpMem + index;
			}
			memcpy(tmpS, line, tmpSLength);
		}
		//------
	}

	// �ŏI�s���i�[
	if (tmpSLength)
		vector_string_push_back_range(SList, tmpS, tmpS + tmpSLength);

	HeapFree(hHeap, 0, tmpS);

DONE:
	CloseHandle(hFile);
	return 0;

FAILED3:
	HeapFree(hHeap, 0, tmpS);
FAILED2:
	CloseHandle(hFile);
FAILED1:
	return 1;

	#undef MODE_END_LINE
	#undef MODE_END_STR
	#undef MODE_END_BYTE
	#undef MODE_START_BYTE
	#undef MODE_LINE_FEED_COUNT
	#undef READ_BLOCK_SIZE
}

