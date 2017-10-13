#pragma function(asin)

__declspec(naked) double __cdecl asin(double x)
{
	__asm
	{
		emms
		fld     qword ptr [esp + 4]     ; Load real from stack
		fld     st(0)                   ; Load x
		fld     st(0)                   ; Load x
		fmul                            ; Multiply (x squared)
		fld1                            ; Load 1
		fsubr                           ; 1 - (x squared)
		fsqrt                           ; Square root of (1 - x squared)
		fpatan                          ; This gives the arc sine !
		ret
	}
}
