#if !defined(_UNICODE) && !defined(_MBCS)
#ifndef strtok
static char *strtok_context = NULL;
#define _tcstok_context strtok_context
#define _tcstok_reset strtok_reset
#endif
#define internal_tcstok internal_strtok
#elif defined(_MBCS)
#ifndef _mbstok
static unsigned char *_mbstok_context = NULL;
#define _tcstok_context _mbstok_context
#define _tcstok_reset _mbstok_reset
#endif
#define internal_tcstok internal_mbstok
#elif defined(_UNICODE)
#ifndef wcstok
static wchar_t *wcstok_context = NULL;
#define _tcstok_context wcstok_context
#define _tcstok_reset wcstok_reset
#endif
#define internal_tcstok internal_wcstok
#endif

#ifndef _M_IX86
#if !defined(_UNICODE) && !defined(_MBCS) && !defined(strtok) || defined(_MBCS) && !defined(_mbstok) || defined(_UNICODE) && !defined(wcstok)
void __cdecl _tcstok_reset()
{
	_tcstok_context = NULL;
}

TCHAR *__cdecl _tcstok(TCHAR *string, const TCHAR *delimiter)
{
	TCHAR *__fastcall internal_tcstok(TCHAR *string, const TCHAR *delimiter, TCHAR **context);

	return internal_tcstok(string, delimiter, &_tcstok_context);
}
#endif

TCHAR *__fastcall internal_tcstok(TCHAR *string, const TCHAR *delimiter, TCHAR **context)
{
	size_t n;
	TCHAR  *token;
#ifdef _MBCS
	TCHAR  c;
#endif

	if (!string && !(string = *context))
		return NULL;
	string += _tcsspn(string, delimiter);
	n = _tcscspn(string, delimiter);
	if (!n)
		return *context = NULL;
	token = string + n;
#ifdef _MBCS
	if (c = *token)
	{
		*(token++) = '\0';
		if (IsDBCSLeadByteEx(CP_THREAD_ACP, c))
		{
#endif
			if (*token)
				*(token++) = '\0';
#ifdef _MBCS
		}
	}
#endif
	*context = token;
	return string;
}
#else
#if !defined(_UNICODE) && !defined(_MBCS) && !defined(strtok) || defined(_MBCS) && !defined(_mbstok) || defined(_UNICODE) && !defined(wcstok)
__declspec(naked) void __cdecl _tcstok_reset()
{
	__asm
	{
		mov     dword ptr [_tcstok_context], 0
		ret
	}
}

__declspec(naked) TCHAR *__cdecl _tcstok(TCHAR *string, const TCHAR *delimiter)
{
	TCHAR *__fastcall internal_tcstok(TCHAR *string, const TCHAR *delimiter, TCHAR **context);

	__asm
	{
		#define string    (esp + 4)
		#define delimiter (esp + 8)

		mov     eax, dword ptr [esp]
		mov     ecx, dword ptr [string]
		mov     edx, dword ptr [delimiter]
		mov     dword ptr [esp], offset _tcstok_context
		push    eax
		jmp     internal_tcstok

		#undef string
		#undef delimiter
	}
}
#endif

__declspec(naked) TCHAR *__fastcall internal_tcstok(TCHAR *string, const TCHAR *delimiter, TCHAR **context)
{
#ifdef _UNICODE
	#define tchar_ptr    word ptr
	#define inc_tchar(r) add r, 2
#else
	#define tchar_ptr    byte ptr
	#define inc_tchar(r) inc r
#endif

	__asm
	{
		#define string    ecx
		#define delimiter edx
		#define context   (esp + 4)

		push    ebx
		push    esi
		push    edi
		mov     ebx, string
		mov     esi, delimiter
		mov     edi, dword ptr [context + 12]
		test    ebx, ebx
		jnz     L1
		mov     ebx, dword ptr [edi]
		test    ebx, ebx
		jz      L3
	L1:
		push    esi
		push    ebx
		call    _tcsspn
		add     esp, 8
#ifdef _UNICODE
		lea     ebx, [ebx + eax * 2]
#else
		add     ebx, eax
#endif
		push    esi
		push    ebx
		call    _tcscspn
		add     esp, 8
#ifdef _UNICODE
		lea     esi, [ebx + eax * 2]
#else
		lea     esi, [eax + ebx]
#endif
		test    eax, eax
		jz      L4
#ifdef _MBCS
		xor     eax, eax
		mov     al, byte ptr [esi]
		test    al, al
		jz      L2
		mov     byte ptr [esi], '\0'
		inc     esi
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
#endif
		cmp     tchar_ptr [esi], '\0'
		je      L2
		mov     tchar_ptr [esi], '\0'
		inc_tchar(esi)
	L2:
		mov     dword ptr [edi], esi
	L3:
		mov     eax, ebx
		pop     edi
		pop     esi
		pop     ebx
		ret     4

		align   16
	L4:
		mov     dword ptr [edi], eax
		pop     edi
		pop     esi
		pop     ebx
		ret     4

		#undef string
		#undef delimiter
		#undef context
	}

	#undef tchar_ptr
	#undef inc_tchar
}
#endif
