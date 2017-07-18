#ifdef ParseArgumentT

#if _MSC_VER >= 1310
#include <intrin.h>
#include <tchar.h>

#pragma intrinsic(__movsb)
#pragma intrinsic(__movsw)
#pragma intrinsic(_tcslen)
#else
#define __movsb(Destination, Source, Count)          \
do                                                   \
{                                                    \
    unsigned char *      _Destination = Destination; \
    const unsigned char *_Source      = Source;      \
    size_t               _Count       = Count;       \
                                                     \
    __asm   mov     ecx, dword ptr [_Count]          \
    __asm   mov     esi, dword ptr [_Source]         \
    __asm   mov     edi, dword ptr [_Destination]    \
    __asm   rep movsb                                \
} while (0)
#define __movsw(Destination, Source, Count)           \
do                                                    \
{                                                     \
    unsigned short *      _Destination = Destination; \
    const unsigned short *_Source      = Source;      \
    size_t                _Count       = Count;       \
                                                      \
    __asm   mov     ecx, dword ptr [_Count]           \
    __asm   mov     esi, dword ptr [_Source]          \
    __asm   mov     edi, dword ptr [_Destination]     \
    __asm   rep movsw                                 \
} while (0)
static __inline size_t wcslen(const wchar_t *string)
{
	const wchar_t *p = string;
	while (*p)
		p++;
	return p - string;
}
#endif

#define memcpy __movsb
#define wmemcpy __movsw

#ifdef _UNICODE
#define _tmemcpy wmemcpy
#else
#define _tmemcpy memcpy
#endif

TCHAR ** __stdcall ParseArgumentT(HANDLE hHeap, const TCHAR *lpParameters, int *argc)
{
	const TCHAR *begin, *end;

	begin = lpParameters;
	end = begin + _tcslen(begin);
	while (*begin == TEXT(' '))
		begin++;
	if (end > begin)
	{
		size_t length, count, capacity;
		TCHAR  **argv;

		length = end - begin;
		count = (length + 1) / 2;
		capacity = count * sizeof(TCHAR *) + (length + 1) * sizeof(TCHAR);
		argv = (TCHAR **)HeapAlloc(hHeap, 0, capacity);
		if (argv)
		{
			TCHAR       *p;
			BOOLEAN     inDoubleQuote;
			size_t      index;
			const TCHAR *src;
#if SHRINK_BUFFER
			TCHAR       *dest;
			size_t      diff;
			LPVOID      memBlock;
#endif

			p = (TCHAR *)(argv + count);
			*(TCHAR *)(end = p + length) = TEXT('\0');
			_tmemcpy(p, begin, length);
			inDoubleQuote = FALSE;
			argv[index = 0] = p;
			while (*p)
			{
#ifndef _UNICODE
				if (!IsDBCSLeadByteEx(CP_THREAD_ACP, *p))
				{
#endif
					switch (*p)
					{
					case TEXT(' '):
						if (inDoubleQuote)
							break;
						*p = TEXT('\0');
#if !defined(SHRINK_BUFFER) || !SHRINK_BUFFER
						while (*(++p) == TEXT(' '));
#else
						if (*(++p) == TEXT(' '))
						{
							src = p;
							while (*(++src) == TEXT(' '));
							count = end - src + 1;
							end -= src - p;
							_tmemcpy(p, src, count);
						}
#endif
						if (*p)
							argv[++index] = p;
						continue;
					case TEXT('"'):
						inDoubleQuote = !inDoubleQuote;
						_tmemcpy(p, p + 1, (end--) - p);
						continue;
					case TEXT('\\'):
						length = 0;
						while (p[++length] == TEXT('\\'));
						if (p[length] == TEXT('"'))
						{
							length = (length + 1) / 2;
							src = p + length;
							_tmemcpy(p, src, end - src + 1);
							end -= length;
						}
						p += length;
						continue;
					}
#ifndef _UNICODE
				}
				else if (!*(++p))
					break;
#endif
				p++;
			}
#if !defined(SHRINK_BUFFER) || !SHRINK_BUFFER
			*argc = (int)index + 1;
			return argv;
#else
			src = argv[0];
			length = end - src;
			count = index + 1;
			dest = (TCHAR *)(argv + count);
			if (diff = (size_t)dest - (size_t)src)
				_tmemcpy(dest, src, length + 1);
			capacity = count * sizeof(TCHAR *) + (length + 1) * sizeof(TCHAR);
			memBlock = HeapReAlloc(hHeap, 0, argv, capacity);
			if (memBlock)
			{
				diff += (size_t)memBlock - (size_t)argv;
				argv = (TCHAR **)memBlock;
				if (diff)
					for (index = 0; index < count; index++)
						*(size_t *)(argv + index) += diff;
				*argc = (int)count;
				return argv;
			}
			else
			{
				HeapFree(hHeap, 0, argv);
			}
#endif
		}
	}
	*argc = 0;
	return NULL;
}
#endif
