
__declspec(naked) void __cdecl TSSGCtrl_ReadSSRFile_DestReserve()
{
	__asm
	{
		#define signedLoop esp
		#define Step       (ebp - 0F8H)

		cmp     dword ptr [signedLoop], 0
		jne     L1
		div     dword ptr [Step]
		jmp     L2
	L1:
		idiv    dword ptr [Step]
		test    eax, eax
		jns     L2
		neg     eax
	L2:
		mov     ecx, 004FF2C0H
		jmp     ecx

		#undef signedLoop
		#undef Step
	}
}
