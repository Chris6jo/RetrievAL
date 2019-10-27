#define wmemichr inline_wmemichr
#define wmemicmp inline_wmemicmp
#include <wchar.h>
#undef wmemichr
#undef wmemicmp
#include <tchar.h>

#ifdef _UNICODE
#define type wchar_t
#define _tmemimem _wmemimem
#define _tmemichr _wmemichr
#define _tmemicmp _wmemicmp
extern wchar_t * __cdecl _wmemichr(const wchar_t *buffer, wint_t c, size_t count);
extern int __cdecl _wmemicmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
#else
#define type void
#define _tmemimem _memimem
#define _tmemichr _memichr
#define _tmemicmp _memicmp
extern void * __cdecl _memichr(const void *buffer, int c, size_t count);
#endif

#ifndef _M_IX86
type * __cdecl _tmemimem(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen)
{
	if (needlelen && haystacklen >= needlelen)
	{
		TCHAR *p, *end, c;

		end = (p = (TCHAR *)haystack) + haystacklen - needlelen + 1;
		c = *(TCHAR *)needle;
		do
			if (!(p = _tmemichr(p, c, end - p)))
				break;
			else if (_tmemicmp(p, needle, needlelen) == 0)
				return p;
		while (end > ++p);
	}
	return NULL;
}
#else
__declspec(naked) type * __cdecl _tmemimem(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen)
{
#ifdef _UNICODE
	#define inc_tchar(r) add r, 2
#else
	#define inc_tchar(r) inc r
#endif

	__asm
	{
		#define haystack    (esp + 4)
		#define haystacklen (esp + 8)
		#define needle      (esp + 12)
		#define needlelen   (esp + 16)

		push    ebx
		push    esi
		push    edi
		xor     ebx, ebx
		mov     ecx, dword ptr [needlelen + 12]
		sub     esp, 12
		test    ecx, ecx
		jz      not_found
		mov     eax, dword ptr [haystacklen + 24]
		mov     edx, dword ptr [needle + 24]
		sub     eax, ecx
		jb      not_found
		mov     esi, dword ptr [haystack + 24]
#ifdef _UNICODE
		lea     eax, [eax * 2 + 2]
#else
		inc     eax
#endif
		mov     bl, byte ptr [edx]
		lea     edi, [eax + esi]

		align   16
	loop_begin:
#ifdef _UNICODE
		shr     eax, 1
#endif
		mov     dword ptr [esp], esi
		mov     dword ptr [esp + 4], ebx
		mov     dword ptr [esp + 8], eax
		call    _tmemichr
		test    eax, eax
		jz      not_found
		mov     ecx, dword ptr [needle + 24]
		mov     edx, dword ptr [needlelen + 24]
		mov     esi, eax
		mov     dword ptr [esp], eax
		mov     dword ptr [esp + 4], ecx
		mov     dword ptr [esp + 8], edx
		call    _tmemicmp
		test    eax, eax
		jz      found
		mov     eax, edi
		inc_tchar(esi)
		sub     eax, esi
		ja      loop_begin
	not_found:
		xor     esi, esi
	found:
		add     esp, 12
		mov     eax, esi
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef haystack
		#undef haystacklen
		#undef needle
		#undef needlelen
	}

	#undef inc_tchar
}
#endif
