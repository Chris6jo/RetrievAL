.486
.model flat

extrn @bcb6_std_node_alloc_allocate@4:proc
extrn @bcb6_std_vector_dtor@4:proc
extrn _memcpy:proc

public @bcb6_std_vector_BYTE_push_back@8

.code

align 16

;EXTERN_C void __msfastcall bcb6_std_vector_BYTE_push_back(vector<char> *v, BYTE value)
;{
;	v->push_back(value);
;}
@bcb6_std_vector_BYTE_push_back@8 proc near

	push    ebx
	mov     ebx, dword ptr [ecx +  4]
	mov     eax, dword ptr [ecx + 16]
	cmp     ebx, eax
	je      L1
	mov     byte ptr [ebx], dl
	inc     ebx
	mov     dword ptr [ecx + 4], ebx
	jmp     L4
L1:
	mov     eax, dword ptr [ecx]
	push    esi
	sub     ebx, eax
	mov     esi, ecx
	mov     ecx, ebx
	push    edx
	add     ecx, ecx
	jnz     L2
	inc     ecx
L2:
	push    ecx
	call    @bcb6_std_node_alloc_allocate@4
	push    eax
	test    ebx, ebx
	jz      L3
	mov     ecx, dword ptr [esi]
	push    ebx
	push    ecx
	push    eax
	call    _memcpy
	add     esp, 12
L3:
	mov     ecx, esi
	call    @bcb6_std_vector_dtor@4
	pop     eax
	pop     ecx
	pop     edx
	add     ebx, eax
	add     ecx, eax
	mov     byte ptr [ebx], dl
	inc     ebx
	mov     dword ptr [esi     ], eax
	mov     dword ptr [esi +  4], ebx
	mov     dword ptr [esi + 16], ecx
	pop     esi
L4:
	pop     ebx
	ret

@bcb6_std_vector_BYTE_push_back@8 endp

end
