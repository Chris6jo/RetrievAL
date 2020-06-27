//***
//ulldvrm.c - unsigned long divide and remainder routine
//
//       Copyright (c) Microsoft Corporation. All rights reserved.
//
//Purpose:
//       defines the unsigned long divide and remainder routine
//           __aulldvrm
//
//*******************************************************************************

#define LOWORD(x) [x]
#define HIWORD(x) [x + 4]

//***
//ulldvrm - unsigned long divide and remainder
//
//Purpose:
//       Does a unsigned long divide and remainder of the arguments.  Arguments
//       are not changed.
//
//Entry:
//       Arguments are passed on the stack:
//               1st pushed: divisor (QWORD)
//               2nd pushed: dividend (QWORD)
//
//Exit:
//       EDX:EAX contains the quotient (dividend/divisor)
//       EBX:ECX contains the remainder (divided % divisor)
//       NOTE: this routine removes the parameters from the stack.
//
//Uses:
//       ECX
//
//Exceptions:
//
//*******************************************************************************

__declspec(naked) void __cdecl _aulldvrm()
{
#if 0
	__asm
	{
		push    esi

		// Set up the local stack and save the index registers.  When this is done
		// the stack frame will look as follows (assuming that the expression a/b will
		// generate a call to aulldvrm(a, b)):
		//
		//               -----------------
		//               |               |
		//               |---------------|
		//               |               |
		//               |--divisor (b)--|
		//               |               |
		//               |---------------|
		//               |               |
		//               |--dividend (a)-|
		//               |               |
		//               |---------------|
		//               | return addr** |
		//               |---------------|
		//       ESP---->|      ESI      |
		//               -----------------
		//

		#define DVND (esp + 8)          // stack address of dividend (a)
		#define DVSR (esp + 16)         // stack address of divisor (b)

		//
		// Now do the divide.  First look to see if the divisor is less than 4194304K.
		// If so, then we can use a simple algorithm with word divides, otherwise
		// things get a little more complex.
		//

		mov     eax, HIWORD(DVSR)       // check to see if divisor < 4194304K
		or      eax, eax
		jnz     short L1                // nope, gotta do this the hard way
		mov     ecx, LOWORD(DVSR)       // load divisor
		mov     eax, HIWORD(DVND)       // load high word of dividend
		xor     edx, edx
		div     ecx                     // get high order bits of quotient
		mov     ebx, eax                // save high bits of quotient
		mov     eax, LOWORD(DVND)       // edx:eax <- remainder:lo word of dividend
		div     ecx                     // get low order bits of quotient
		mov     esi, eax                // ebx:esi <- quotient

		//
		// Now we need to do a multiply so that we can compute the remainder.
		//
		mov     eax, ebx                // set up high word of quotient
		mul     dword ptr LOWORD(DVSR)  // HIWORD(QUOT) * DVSR
		mov     ecx, eax                // save the result in ecx
		mov     eax, esi                // set up low word of quotient
		mul     dword ptr LOWORD(DVSR)  // LOWORD(QUOT) * DVSR
		add     edx, ecx                // EDX:EAX = QUOT * DVSR
		jmp     short L2                // complete remainder calculation

		//
		// Here we do it the hard way.  Remember, eax contains DVSRHI
		//

	L1:
		mov     ecx, eax                // ecx:ebx <- divisor
		mov     ebx, LOWORD(DVSR)
		mov     edx, HIWORD(DVND)       // edx:eax <- dividend
		mov     eax, LOWORD(DVND)
	L3:
		shr     ecx, 1                  // shift divisor right one bit; hi bit <- 0
		rcr     ebx, 1
		shr     edx, 1                  // shift dividend right one bit; hi bit <- 0
		rcr     eax, 1
		or      ecx, ecx
		jnz     short L3                // loop until divisor < 4194304K
		div     ebx                     // now divide, ignore remainder
		mov     esi, eax                // save quotient

		//
		// We may be off by one, so to check, we will multiply the quotient
		// by the divisor and check the result against the orignal dividend
		// Note that we must also check for overflow, which can occur if the
		// dividend is close to 2**64 and the quotient is off by 1.
		//

		mul     dword ptr HIWORD(DVSR)  // QUOT * HIWORD(DVSR)
		mov     ecx, eax
		mov     eax, LOWORD(DVSR)
		mul     esi                     // QUOT * LOWORD(DVSR)
		add     edx, ecx                // EDX:EAX = QUOT * DVSR
		jc      short L4                // carry means Quotient is off by 1

		//
		// do long compare here between original dividend and the result of the
		// multiply in edx:eax.  If original is larger or equal, we are ok, otherwise
		// subtract one (1) from the quotient.
		//

		cmp     edx, HIWORD(DVND)       // compare hi words of result and original
		ja      short L4                // if result > original, do subtract
		jb      short L5                // if result < original, we are ok
		cmp     eax, LOWORD(DVND)       // hi words are equal, compare lo words
		jbe     short L5                // if less or equal we are ok, else subtract
	L4:
		dec     esi                     // subtract 1 from quotient
		sub     eax, LOWORD(DVSR)       // subtract divisor from result
		sbb     edx, HIWORD(DVSR)
	L5:
		xor     ebx, ebx                // ebx:esi <- quotient

	L2:
		//
		// Calculate remainder by subtracting the result from the original dividend.
		// Since the result is already in a register, we will do the subtract in the
		// opposite direction and negate the result.
		//

		sub     eax, LOWORD(DVND)       // subtract dividend from result
		sbb     edx, HIWORD(DVND)
		neg     edx                     // otherwise, negate the result
		neg     eax
		sbb     edx, 0

		//
		// Now we need to get the quotient into edx:eax and the remainder into ebx:ecx.
		//
		mov     ecx, edx
		mov     edx, ebx
		mov     ebx, ecx
		mov     ecx, eax
		mov     eax, esi
		//
		// Just the cleanup left to do.  edx:eax contains the quotient.
		// Restore the saved registers and return.
		//

		pop     esi

		ret     16

		#undef DVND
		#undef DVSR
	}
#else
	__asm
	{
		#define DVND (esp + 4)          // stack address of dividend (a)
		#define DVSR (esp + 12)         // stack address of divisor (b)

		mov     eax, HIWORD(DVND)
		mov     ecx, HIWORD(DVSR)
		mov     edx, eax
		or      eax, ecx
		jnz     large
		mov     eax, LOWORD(DVND)
		mov     ecx, LOWORD(DVSR)
		cmp     eax, ecx
		jb      smaller
		xor     edx, edx
		div     ecx
		mov     ecx, edx
		xor     edx, edx
		xor     ebx, ebx
		ret     16

		align   16
	large:
		cmp     edx, ecx
		jb      smaller
		cmp     ecx, 1 shl 4
		jae     mid
		or      ecx, ecx
		jz      biglittle
		shr     ecx, 1
		mov     ebx, LOWORD(DVSR)
		rcr     ebx, 1
		mov     eax, LOWORD(DVND)
		shr     edx, 1
		rcr     eax, 1
		or      ecx, ecx
		jz      cont
		shr     ecx, 1
		rcr     ebx, 1
		shr     edx, 1
		rcr     eax, 1
		or      ecx, ecx
		jz      cont
		shr     ecx, 1
		rcr     ebx, 1
		shr     edx, 1
		rcr     eax, 1
		or      ecx, ecx
		jz      cont
		shr     ecx, 1
		rcr     ebx, 1
		shr     edx, 1
		rcr     eax, 1
		jmp     cont

	mid:
		mov     eax, edx
		xor     edx, edx
		mov     ebx, ecx
		add     ecx, ecx
		jc      cont
		bsr     ecx, ecx
		mov     edx, ebx
		mov     ebx, LOWORD(DVSR)
		shrd    ebx, edx, cl
		mov     edx, eax
		mov     eax, LOWORD(DVND)
		shrd    eax, edx, cl
		shr     edx, cl

	cont:
		div     ebx
		push    eax
		mov     ebx, eax
		mov     eax, HIWORD(DVSR + 4)
		mul     ebx
		or      edx, edx
		jnz     wrap
		mov     ecx, eax
		mov     eax, LOWORD(DVSR + 4)
		mul     ebx
		add     edx, ecx
		mov     ecx, LOWORD(DVND + 4)
		mov     ebx, HIWORD(DVND + 4)
		sub     ecx, eax
		sbb     ebx, edx
		js      wrap
		sub     ecx, LOWORD(DVSR + 4)
		pop     eax
		sbb     ebx, HIWORD(DVSR)
		jae     undershot
		add     ecx, LOWORD(DVSR)       // add back in our undershoot
		adc     ebx, HIWORD(DVSR)
		xor     edx, edx
		ret     16

		align   16
	undershot:
		inc     eax                     // we undershot by one (already subtracted one from remainder)
		xor     edx, edx
		ret     16

		align   16
	wrap:
		pop     eax                     // we overshot by one
		xor     edx, edx
		dec     eax
		add     ecx, LOWORD(DVSR)
		adc     ebx, HIWORD(DVSR)
		ret     16

		align   16
	smaller:
		mov     ecx, LOWORD(DVND)
		mov     ebx, edx
		xor     eax, eax
		xor     edx, edx
		ret     16

		align   16
	biglittle:
		mov     eax, edx
		mov     ecx, LOWORD(DVSR)
		xor     edx, edx
		div     ecx
		mov     ebx, eax
		mov     eax, LOWORD(DVND)
		div     ecx
		mov     ecx, edx
		mov     edx, ebx
		xor     ebx, ebx
		ret     16

		#undef DVND
		#undef DVSR
	}
#endif
}

#if 0
__declspec(naked) unsigned __int64 __stdcall _ulldvrm(unsigned __int64 dividend, unsigned __int64 divisor, unsigned __int64 *remainder)
{
	__asm
	{
		push    ebx
		push    esi
		mov     esi, dword ptr [esp + 28]
		mov     ebx, dword ptr [esp + 24]
		mov     ecx, dword ptr [esp + 20]
		mov     edx, dword ptr [esp + 16]
		mov     eax, dword ptr [esp + 12]
		push    ebx
		push    ecx
		push    edx
		push    eax
		call    _aulldvrm
		mov     dword ptr [esi], ecx
		mov     dword ptr [esi + 4], ebx
		pop     esi
		pop     ebx
		ret     20
	}
}
#endif
