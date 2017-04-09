#ifdef __BORLANDC__
#define bcb6_std_string_empty(s)             (s)->empty()
#define bcb6_std_string_length(s)            (s)->length()
#define bcb6_std_string_begin(s)             (s)->begin()
#define bcb6_std_string_end(s)               (s)->end()
#define bcb6_std_string_assign_cstr(s, cstr) *(s) = (cstr);
#else
#include <windows.h>
#include "bcb6_std_string.h"
#endif

#include <mbstring.h>
#include "intrinsic.h"
#include "tlhelp32fix.h"

#include "ProcessMonitor.h"
#include "FindWindowContainsModule.h"
#include "GetFileTitlePointer.h"

extern HANDLE hHeap;

static char * __fastcall FindInvalidChar(const char *string)
{
	while (*string)
	{
		if (!__intrinsic_isleadbyte(*string))
		{
			switch (*string)
			{
			case '\\':
			case '/':
			case ':':
			case '*':
			case '?':
			case '"':
			case '<':
			case '>':
			case '|':
				return (char *)string;
			}
		}
		else
		{
			if (!*(++string))
				break;
		}
		string++;
	}
	return NULL;
}

__inline char *TrimRight(const char *left, const char *right)
{
	while (--right >= left && *right == ' ');
	return (char *)right + 1;
}

static char ** __stdcall ParseArgument(const char *begin, const char *end, size_t *argc)
{
	*argc = 0;
	while (*begin == ' ')
		begin++;
	if (end > begin)
	{
		size_t length, maxArgCount, capacity;
		char   **argv;

		length = end - begin;
		maxArgCount = (length + 1) / 2;
		capacity = maxArgCount * sizeof(char *) + (length + 1) * sizeof(char);
		argv = (char **)HeapAlloc(hHeap, 0, capacity);
		if (argv)
		{
			char    *p;
			BOOLEAN inDoubleQuote;
			size_t  index;

			p = (char *)(argv + maxArgCount);
			*(char *)(end = p + length) = '\0';
			memcpy(p, begin, length * sizeof(char));
			inDoubleQuote = FALSE;
			argv[index = 0] = p;
			while (*p)
			{
				if (!__intrinsic_isleadbyte(*p))
				{
					switch (*p)
					{
					case ' ':
						if (inDoubleQuote)
							break;
						*p = '\0';
						while (*(++p) == ' ');
						if (*p)
							argv[++index] = p;
						continue;
					case '"':
						inDoubleQuote = !inDoubleQuote;
						memcpy(p, p + 1, ((end--) - p) * sizeof(char));
						continue;
					case '\\':
						length = 0;
						while (p[++length] == '\\');
						if (p[length] == '"')
						{
							const char *src;

							length = (length + 1) / 2;
							src = p + length;
							memcpy(p, src, (end - src + 1) * sizeof(char));
							end -= length;
						}
						p += length;
						continue;
					}
				}
				else if (!*(++p))
					break;
				p++;
			}
			*argc = index + 1;
			return argv;
		}
	}
	return NULL;
}

#ifdef __BORLANDC__
unsigned long TProcessCtrl::FindProcess(string *ProcessName, PROCESSENTRY32 *Entry)
#else
unsigned long __cdecl TProcessCtrl_FindProcess(LPVOID _this, bcb6_std_string *ProcessName, PROCESSENTRY32A *Entry)
#endif
{
	#define CLASSNAME_BRACKET_OPEN  '<'
	#define CLASSNAME_BRACKET_CLOSE '>'
	#define MODULENAME_DELIMITER    ':'
	#define WINDOWNAME_DELIMITER    '*'

	do	/* do { ... } while (0); */
	{
		DWORD           dwProcessId;
		HANDLE          hSnapshot;
		PROCESSENTRY32A pe;

		if (bcb6_std_string_empty(ProcessName))
			break;
		if (*bcb6_std_string_begin(ProcessName) == CLASSNAME_BRACKET_OPEN)
		{
			char   **argv;
			size_t argc;

			argv = ParseArgument(bcb6_std_string_begin(ProcessName) + 1, bcb6_std_string_end(ProcessName), &argc);
			if (!argv)
				break;
			dwProcessId = 0;
			do	/* do { ... } while (0); */
			{
				LPSTR  lpClassName;
				LPSTR  lpWindowName;
				LPSTR  lpModuleName;
				size_t length;

				lpClassName = argv[0];
				length = strlen(lpClassName);
				if (length <= 1 || lpClassName[--length] != CLASSNAME_BRACKET_CLOSE)
					break;
				lpClassName[length] = '\0';
				lpWindowName = NULL;
				lpModuleName = NULL;
				for (size_t i = 1; i < argc; i++)
				{
					switch (*argv[i])
					{
					case WINDOWNAME_DELIMITER:
						lpWindowName = argv[i] + 1;
						break;
					case MODULENAME_DELIMITER:
						lpModuleName = argv[i] + 1;
						break;
					}
				}
				if (FindWindowContainsModule(lpClassName, lpWindowName, lpModuleName, &dwProcessId))
					StopProcessMonitor();
			} while (0);
			HeapFree(hHeap, 0, argv);
		}
		else
		{
			LPSTR lpParameters;

			lpParameters = FindInvalidChar(bcb6_std_string_begin(ProcessName));
			if (!lpParameters)
			{
				dwProcessId = FindProcessId(bcb6_std_string_begin(ProcessName), bcb6_std_string_length(ProcessName), NULL);
			}
			else
			{
				char   **argv;
				size_t argc;
				LPSTR  lpModuleName;
				LPSTR  lpEndOfProcessName;
				char   cProcessNameSplitChar;

				argv = ParseArgument(lpParameters, bcb6_std_string_end(ProcessName), &argc);
				if (!argv)
					break;
				lpModuleName = NULL;
				for (size_t i = 0; i < argc; i++)
				{
					if (*argv[i] == MODULENAME_DELIMITER)
					{
						lpModuleName = argv[i] + 1;
						break;
					}
				}
				lpEndOfProcessName = TrimRight(bcb6_std_string_begin(ProcessName), lpParameters);
				cProcessNameSplitChar = *lpEndOfProcessName;
				*lpEndOfProcessName = '\0';
				dwProcessId = FindProcessId(
					bcb6_std_string_begin(ProcessName),
					lpEndOfProcessName - bcb6_std_string_begin(ProcessName),
					lpModuleName);
				*lpEndOfProcessName = cProcessNameSplitChar;
				HeapFree(hHeap, 0, argv);
			}
		}
		if (!dwProcessId)
			break;
		hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot == INVALID_HANDLE_VALUE)
			break;
		pe.dwSize = sizeof(PROCESSENTRY32A);
		if (Process32FirstA(hSnapshot, &pe))
		{
			do
			{
				if (pe.th32ProcessID != dwProcessId)
					continue;
				CloseHandle(hSnapshot);
				bcb6_std_string_assign_cstr(ProcessName, GetFileTitlePointerA(pe.szExeFile));
				*Entry = pe;
				return 0;
			} while (Process32NextA(hSnapshot, &pe));
		}
		CloseHandle(hSnapshot);
	} while (0);
	return 1;

	#undef CLASSNAME_BRACKET_OPEN
	#undef CLASSNAME_BRACKET_CLOSE
	#undef MODULENAME_DELIMITER
	#undef WINDOWNAME_DELIMITER
}
