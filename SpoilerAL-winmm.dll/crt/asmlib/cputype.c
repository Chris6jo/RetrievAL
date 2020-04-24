__declspec(naked) void __cdecl CpuType(int *vendor, int *family, int *model)
{
	__asm
	{
		push    ebx
		push    esi
		push    edi

		// parameters
		#define vendor (esp + 16)
		#define family (esp + 20)
		#define model  (esp + 24)

		xor     esi, esi                                    // vendor
		xor     edi, edi                                    // family

		// detect if CPUID instruction supported by microprocessor:
		pushfd
		pop     eax
		btc     eax, 21                                     // check if CPUID bit can toggle
		push    eax
		popfd
		pushfd
		pop     ebx
		xor     ebx, eax
		bt      ebx, 21
		jc      C900                                        // CPUID not supported

		xor     eax, eax
		cpuid                                               // get number of CPUID functions

		// get vendor
		// ecx = last  4 characters of vendor string
		// ebx = first 4 characters of vendor string
		cmp     ecx, 'ntel'                                 // 'GenuineIntel'
		je      C110
		cmp     ecx, 'cAMD'                                 // 'AuthenticAMD'
		je      C120
		cmp     ebx, 'Cent'                                 // 'CentaurHauls'
		je      C130
		cmp     ebx, 'VIA '                                 // 'VIA VIA VIA '
		je      C130
		cmp     ebx, 'Cyri'                                 // 'CyrixInstead'
		je      C140
		cmp     ebx, 'NexG'                                 // 'NexGenDriven'
		je      C150
		jmp     C200                                        // other

	C110:
		or      esi, 1
		jmp     C200

	C120:
		or      esi, 2
		jmp     C200

	C130:
		or      esi, 3
		jmp     C200

	C140:
		or      esi, 4
		jmp     C200

	C150:
		or      esi, 5

	C200:
		test    eax, eax
		jz      C900                                        // function 1 not supported

		// Get family and model
		mov     eax, 1
		cpuid
		mov     ebx, eax
		mov     edi, eax
		shr     ebx, 8
		and     ebx, 0FH                                    // Family
		shr     edi, 20
		and     edi, 0FFH                                   // Extended family
		add     edi, ebx                                    // Family + extended family

		mov     ebx, eax
		shr     ebx, 4
		and     ebx, 0FH                                    // Model
		mov     ecx, eax
		shr     ecx, 12
		and     ecx, 0F0H                                   // Extended model
		or      ebx, ecx                                    // extended model - Model
#if 1
		jmp     C300

	C900:
		// no cpuid
		xor     ebx, ebx
#endif

	C300:
		// return esi = vendor, edi = family, ebx = model
		mov     eax, dword ptr [vendor]
		test    eax, eax
		jz      C310
		mov     dword ptr [eax], esi

	C310:
		mov     eax, dword ptr [family]
		test    eax, eax
		jz      C320
		mov     dword ptr [eax], edi

	C320:
		mov     eax, dword ptr [model]
		test    eax, eax
		jz      C330
		mov     dword ptr [eax], ebx

	C330:
#if 0
		xor     eax, eax
#endif
		// return
		pop     edi
		pop     esi
		pop     ebx
		ret
#if 0

	C900:
		// no cpuid
		xor     ebx, ebx
		jmp     C300
#endif

		#undef vendor
		#undef family
		#undef model
	}
}
