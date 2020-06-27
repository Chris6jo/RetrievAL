//***
//ulldiv.c - unsigned long divide routine
//
//       Copyright (c) Microsoft Corporation. All rights reserved.
//
//Purpose:
//       defines the unsigned long divide routine
//           __aulldiv
//
//*******************************************************************************

#define LOWORD(x) [x]
#define HIWORD(x) [x + 4]

//***
//ulldiv - unsigned long divide
//
//Purpose:
//       Does a unsigned long divide of the arguments.  Arguments are
//       not changed.
//
//Entry:
//       Arguments are passed on the stack:
//               1st pushed: divisor (QWORD)
//               2nd pushed: dividend (QWORD)
//
//Exit:
//       EDX:EAX contains the quotient (dividend/divisor)
//       NOTE: this routine removes the parameters from the stack.
//
//Uses:
//       ECX
//
//Exceptions:
//
//*******************************************************************************

__declspec(naked) void __cdecl _aulldiv()
{
#if 0
	__asm
	{
		push    ebx
		push    esi

		// Set up the local stack and save the index registers.  When this is done
		// the stack frame will look as follows (assuming that the expression a/b will
		// generate a call to uldiv(a, b)):
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
		//               |      EBX      |
		//               |---------------|
		//       ESP---->|      ESI      |
		//               -----------------
		//

		#define DVND (esp + 12)         // stack address of dividend (a)
		#define DVSR (esp + 20)         // stack address of divisor (b)

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
		mov     edx, ebx                // edx:eax <- quotient hi:quotient lo
		jmp     short L2                // restore stack and return

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
	L5:
		xor     edx, edx                // edx:eax <- quotient
		mov     eax, esi

		//
		// Just the cleanup left to do.  edx:eax contains the quotient.
		// Restore the saved registers and return.
		//

	L2:

		pop     esi
		pop     ebx

		ret     16

		#undef DVND
		#undef DVSR
	}
#else
	__asm
	{
		push    ebx
		push    esi

		// Set up the local stack and save the index registers.  When this is done
		// the stack frame will look as follows (assuming that the expression a/b will
		// generate a call to uldiv(a, b)):
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
		//               |      EBX      |
		//               |---------------|
		//       ESP---->|      ESI      |
		//               -----------------
		//

		#define DVND (esp + 12)         // stack address of dividend (a)
		#define DVSR (esp + 20)         // stack address of divisor (b)

		//
		// Now do the divide.  First look to see if the divisor is less than 4194304K.
		// If so, then we can use a simple algorithm with word divides, otherwise
		// things get a little more complex.
		//

		mov     edx, HIWORD(DVSR)       // load high word of divisor
		mov     eax, HIWORD(DVND)       // load high word of dividend
		or      edx, edx                // check to see if divisor < 4194304K
		jnz     short L1                // nope, gotta do this the hard way
		mov     ecx, LOWORD(DVSR)       // load divisor
		div     ecx                     // get high order bits of quotient
		mov     ebx, eax                // save high bits of quotient
		mov     eax, LOWORD(DVND)       // edx:eax <- remainder:lo word of dividend
		div     ecx                     // get low order bits of quotient
		mov     edx, ebx                // edx:eax <- quotient hi:quotient lo
		jmp     short L2                // restore stack and return

		//
		// Here we do it the hard way.  Remember, eax contains DVSRHI
		//

		align   16
	L1:
		mov     ebx, edx
		jns     short shift
		xor     edx, edx
		jmp     short divide

		align   16
	shift:
		cmp     edx, 1 shl 4
		jae     bitscan
		mov     ecx, edx                // ecx:ebx <- divisor
		mov     ebx, LOWORD(DVSR)
		shr     ecx, 1
		mov     edx, eax                // edx:eax <- dividend
		rcr     ebx, 1
		mov     eax, LOWORD(DVND)
		shr     edx, 1
		rcr     eax, 1
		or      ecx, ecx
		jz      divide
		shr     ecx, 1
		rcr     ebx, 1
		shr     edx, 1
		rcr     eax, 1
		or      ecx, ecx
		jz      divide
		shr     ecx, 1
		rcr     ebx, 1
		shr     edx, 1
		rcr     eax, 1
		or      ecx, ecx
		jz      divide
		shr     ecx, 1
		rcr     ebx, 1
		shr     edx, 1
		rcr     eax, 1
		jmp     divide

		align   16
	bitscan:
		bsr     ecx, edx
		mov     ebx, LOWORD(DVSR)       // edx:ebx <- divisor
		inc     ecx
		shrd    ebx, edx, cl
		mov     edx, eax                // edx:eax <- dividend
		mov     eax, LOWORD(DVND)
		shrd    eax, edx, cl
		shr     edx, cl
	divide:
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

		cmp     LOWORD(DVND), eax       // compare original and result
		mov     ecx, HIWORD(DVND)       // ecx <- high word of original
		sbb     ecx, edx                // if original < result, do subtract
	L4:
		mov     eax, esi                // edx:eax <- quotient
		sbb     eax, 0                  // subtract carry flag
		xor     edx, edx

		//
		// Just the cleanup left to do.  edx:eax contains the quotient.
		// Restore the saved registers and return.
		//

	L2:

		pop     esi
		pop     ebx

		ret     16

		#undef DVND
		#undef DVSR
	}
#endif
}

#if 0
__declspec(naked) unsigned __int64 __stdcall _ulldiv(unsigned __int64 dividend, unsigned __int64 divisor)
{
	__asm
	{
		jmp     _aulldiv
	}
}
#endif
