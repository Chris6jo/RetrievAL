.486
.model flat, c
assume fs:nothing

extrn bcb6_std_string_ctor:dword
extrn bcb6_global_operator_new:dword
extrn F005D54CC:dword

public new_bcb6_std_string

.const

data1 label byte
	dword   0040261CH
	dword   00002007H, -4
	dword   00000000H

data2 label byte
	dword   00000000H, -40
	dword   00050000H, 00000000H
	dword   00000000H, 00050008H
	dword   00000000H
	dword   data1

.code

align 16

;EXTERN_C string * __cdecl new_bcb6_std_string()
;{
;	return new string();
;}
new_bcb6_std_string proc near

	push    ebp
	mov     eax, offset data2
	mov     ebp, esp
	sub     esp, 40
	call    dword ptr [F005D54CC]
	push    24
	call    dword ptr [bcb6_global_operator_new]
	pop     ecx
	test    eax, eax
	jz      L1
	mov     dword ptr [ebp - 4H], eax
	push    eax
	call    dword ptr [bcb6_std_string_ctor]
	pop     ecx
L1:
	mov     ecx, dword ptr [ebp - 40]
	mov     dword ptr fs:[0], ecx
	mov     esp, ebp
	pop     ebp
	ret

new_bcb6_std_string endp

end
