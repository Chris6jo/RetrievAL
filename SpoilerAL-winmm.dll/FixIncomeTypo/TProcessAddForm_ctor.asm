.486
.model flat, c

includelib user32.lib

extrn _imp__SetWindowTextA@8:dword
extrn _TWinControl_GetHandle:dword

public TProcessAddForm_ctor

.const

data1 label byte
	byte    '�ғ��v���Z�X�̍Ď擾', 0

.code

align 16

TProcessAddForm_ctor proc near

	inc     dword ptr [ebx + 1CH]
	inc     dword ptr [ebx + 1CH]
	push    offset data1
	mov     eax, dword ptr [ebp - 4H]
	mov     eax, dword ptr [eax + 30CH]
	call    dword ptr [_TWinControl_GetHandle]
	push    eax
	push    00486C8FH
	jmp     dword ptr [_imp__SetWindowTextA@8]

TProcessAddForm_ctor endp

end
