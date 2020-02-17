static void __cdecl _gmtime64_s_jan();
static void __cdecl _gmtime64_s_feb();
static void __cdecl _gmtime64_s_mar();
static void __cdecl _gmtime64_s_apr();
static void __cdecl _gmtime64_s_may();
static void __cdecl _gmtime64_s_jun();
static void __cdecl _gmtime64_s_jul();
static void __cdecl _gmtime64_s_aug();
static void __cdecl _gmtime64_s_sep();
static void __cdecl _gmtime64_s_oct();
static void __cdecl _gmtime64_s_nov();
static void __cdecl _gmtime64_s_dec();
static void __cdecl _gmtime64_s_next_month();

#define LABEL(name)                          \
    }                                        \
}                                            \
__declspec(naked) static void __cdecl name() \
{                                            \
    __asm                                    \
    {

__declspec(naked) errno_t __cdecl _gmtime64_s(struct tm *dest, const __time64_t *source)
{
	static const void(__cdecl *JumpTable[])() = {
		_gmtime64_s_jan,
		_gmtime64_s_feb,
		_gmtime64_s_mar,
		_gmtime64_s_apr,
		_gmtime64_s_may,
		_gmtime64_s_jun,
		_gmtime64_s_jul,
		_gmtime64_s_aug,
		_gmtime64_s_sep,
		_gmtime64_s_oct,
		_gmtime64_s_nov,
		_gmtime64_s_dec,
	};

	#define DAY_SEC     (60 * 60 * 24)
	#define SINCE(year) (((year) - 1) * 365 + ((year) - 1) / 4 - ((year) - 1) / 100 + ((year) - 1) / 400)
	#define JAN_FEB     (31 + 28)
	#define YEAR        365
	#define YEAR4       (YEAR * 4 + 1)
	#define YEAR100     (YEAR4 * 25 - 1)
	#define YEAR400     (YEAR100 * 4 + 1)

	__asm
	{
		#define dest   (esp + 4)
		#define source (esp + 8)

		mov     ecx, dword ptr [dest]
		mov     edx, dword ptr [source]
		test    ecx, ecx
		jz      L8
		test    edx, edx
		jz      L7
		mov     eax, dword ptr [edx]
		mov     edx, dword ptr [edx + 4]
		test    edx, edx
		jg      L1
		jl      _gmtime32_s
		test    eax, eax
		js      L2
		jmp     _gmtime32_s

	L1:
		cmp     edx, 7
		jb      L2
		ja      L7
		cmp     eax, 9358E1CFH
		ja      L7
	L2:
		push    ebx
		push    ebp
		push    esi
		push    edi
		push    eax
		mov     esi, 72894AB7H
		mov     edi, edx
		mov     ebx, eax
		mul     esi
		mov     ecx, edx
		mov     eax, 0C22E4506H
		mul     ebx
		mov     ebx, eax
		mov     ebp, edx
		mov     eax, edi
		mul     esi
		mov     esi, eax
		mov     eax, edi
		mov     edi, edx
		mov     edx, 0C22E4506H
		mul     edx
		add     esi, ecx
		adc     edi, 0
		add     ebx, esi
		adc     ebp, 0
		mov     ebx, eax
		xor     eax, eax
		add     edi, ebp
		adc     eax, eax
		add     ebx, edi
		adc     edx, eax
		pop     edi
		shrd    ebx, edx, 16
		shr     edx, 16
		imul    eax, ebx, DAY_SEC
		sub     edi, eax
		add     ebx, SINCE(1970) - SINCE(1600) - JAN_FEB
		adc     edx, 0
		mov     eax, 04444445H
		mov     ebp, edx
		push    ebx
		mul     edi
		mov     eax, 04444445H
		mov     ecx, edx
		mul     edx
		mov     esi, ecx
		shl     esi, 4
		mov     eax, edx
		shl     eax, 4
		sub     esi, ecx
		shl     esi, 2
		sub     eax, edx
		shl     eax, 2
		sub     edi, esi
		mov     esi, dword ptr [dest + 20]
		sub     ecx, eax
		mov     dword ptr [esi], edi
		mov     dword ptr [esi + 4], ecx
		mov     dword ptr [esi + 8], edx
		mov     eax, ebx
		mov     esi, 0C8F862EDH
		mov     edi, 396B06BCH
		mul     esi
		mov     ecx, edx
		mov     eax, ebx
		mul     edi
		mov     ebx, eax
		mov     eax, ebp
		push    edx
		mul     esi
		mov     esi, eax
		mov     edi, edx
		mov     eax, ebp
		mov     edx, 396B06BCH
		mul     edx
		add     esi, ecx
		mov     ebp, eax
		adc     edi, 0
		add     ebx, esi
		pop     esi
		pop     ecx
		adc     esi, 0
		xor     eax, eax
		add     edi, esi
		adc     eax, eax
		lea     esi, [ebp + edi]
		adc     edx, eax
		mov     edi, dword ptr [dest + 16]
		shrd    esi, edx, 15
		imul    ebp, esi, 400
		imul    eax, esi, YEAR400
		sub     ecx, eax
		mov     eax, 24924925H
		lea     esi, [ecx + 2]
		mul     esi
		add     esi, edx
		lea     eax, [edx * 8]
		xor     ebx, ebx
		sub     esi, eax
		cmp     ecx, YEAR - JAN_FEB + 1
		mov     dword ptr [edi + 24], esi
		adc     ebx, 0
		jnz     L4
		mov     eax, 166DB073H
		add     ebp, 300
		sub     ecx, YEAR100 * 3
		jae     L3
		sub     ebp, 100
		add     ecx, YEAR100
		jc      L3
		sub     ebp, 100
		add     ecx, YEAR100
		jc      L3
		sub     ebp, 100
		add     ecx, YEAR100
	L3:
		cmp     ecx, YEAR - JAN_FEB + 1
		jb      L4
		mul     ecx
		shr     edx, 7
		lea     ebp, [ebp + edx * 4]
		imul    eax, edx, YEAR4
		sub     ecx, eax
		cmp     ecx, YEAR - JAN_FEB + 1
		adc     ebx, 0
		jz      L5
	L4:
		lea     ecx, [ecx + ebx + JAN_FEB - 1]
		jmp     L6

	L5:
		add     ecx, JAN_FEB - 1
		add     ebp, 4
		sub     ecx, YEAR * 4
		jae     L6
		dec     ebp
		add     ecx, YEAR
		jc      L6
		dec     ebp
		add     ecx, YEAR
		jc      L6
		dec     ebp
		add     ecx, YEAR
		jc      L6
		dec     ebp
		add     ecx, YEAR
	L6:
		mov     edx, ecx
		sub     ebp, 1900 - 1600
		shr     edx, 5
		xor     eax, eax
		mov     dword ptr [edi + 16], edx
		mov     dword ptr [edi + 20], ebp
		mov     dword ptr [edi + 28], ecx
		mov     dword ptr [edi + 32], eax
		jmp     dword ptr [JumpTable + edx * 4]

	L7:
		mov     dword ptr [ecx     ], -1
		mov     dword ptr [ecx +  4], -1
		mov     dword ptr [ecx +  8], -1
		mov     dword ptr [ecx + 12], -1
		mov     dword ptr [ecx + 16], -1
		mov     dword ptr [ecx + 20], -1
		mov     dword ptr [ecx + 24], -1
		mov     dword ptr [ecx + 28], -1
		mov     dword ptr [ecx + 32], -1
	L8:
		mov     eax, EINVAL
		ret

	LABEL(_gmtime64_s_jan)
		sub     ecx, 30
		ja      _gmtime64_s_next_month
		add     ecx, 31
		mov     dword ptr [edi + 12], ecx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_feb)
		sub     ecx, 30
		add     ebx, 28
		sub     ecx, ebx
		ja      _gmtime64_s_next_month
		add     ecx, ebx
		mov     dword ptr [edi + 12], ecx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_mar)
		sub     ecx, ebx
		sub     ecx, 89
		ja      _gmtime64_s_next_month
		add     ecx, 31
		mov     dword ptr [edi + 12], ecx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_apr)
		sub     ecx, ebx
		sub     ecx, 119
		ja      _gmtime64_s_next_month
		add     ecx, 30
		mov     dword ptr [edi + 12], ecx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_may)
		sub     ecx, ebx
		sub     ecx, 150
		ja      _gmtime64_s_next_month
		add     ecx, 31
		mov     dword ptr [edi + 12], ecx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_jun)
		sub     ecx, ebx
		sub     ecx, 180
		ja      _gmtime64_s_next_month
		add     ecx, 30
		mov     dword ptr [edi + 12], ecx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_jul)
		sub     ecx, ebx
		sub     ecx, 211
		ja      _gmtime64_s_next_month
		add     ecx, 31
		mov     dword ptr [edi + 12], ecx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_aug)
		sub     ecx, ebx
		sub     ecx, 242
		ja      _gmtime64_s_next_month
		add     ecx, 31
		mov     dword ptr [edi + 12], ecx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_sep)
		sub     ecx, ebx
		sub     ecx, 272
		ja      _gmtime64_s_next_month
		add     ecx, 30
		mov     dword ptr [edi + 12], ecx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_oct)
		sub     ecx, ebx
		sub     ecx, 303
		ja      _gmtime64_s_next_month
		add     ecx, 31
		mov     dword ptr [edi + 12], ecx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_nov)
		sub     ecx, ebx
		sub     ecx, 333
		ja      _gmtime64_s_next_month
		add     ecx, 30
		mov     dword ptr [edi + 12], ecx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_dec)
		sub     ecx, ebx
		sub     ecx, 333
		mov     dword ptr [edi + 12], ecx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_next_month)
		inc     edx
		mov     dword ptr [edi + 12], ecx
		mov     dword ptr [edi + 16], edx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef dest
		#undef source
	}

	#undef DAY_SEC
	#undef SINCE
	#undef JAN_FEB
	#undef YEAR
	#undef YEAR4
	#undef YEAR100
	#undef YEAR400
}

#undef LABEL
