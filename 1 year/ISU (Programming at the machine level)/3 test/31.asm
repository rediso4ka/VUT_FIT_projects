%include "rw32-2018.inc"

section .data
    array dq 1.0,1.5,2.0,100.0,3.0,2.5,3.4,6.0

CEXTERN malloc
CEXTERN free
CEXTERN memcpy
CEXTERN qsort
section .text
_main:
    push ebp
    mov ebp, esp
    
    push dword 8
    push array
    call task31
    add esp,8
    
    call WriteDouble

    pop ebp
    ret

comp:
    push ebp
    mov ebp,esp

    mov eax, dword [ebp + 8]
    fld qword [eax]
    mov eax, dword [ebp + 12]
    fld qword [eax]
    fsubp
    
    sub esp,4
    fst dword [esp]
    mov eax, dword [esp]
    add esp,4
    
    fstp st0
    pop ebp
    ret

task31:
    push ebp
    mov ebp,esp
    push esi
    push edi
    
    ;;;return bad values;;;
    mov ecx, [ebp + 12] ;N
    cmp ecx,0
    jle bad_end
    mov esi,[ebp + 8] ;array
    cmp esi,0
    je bad_end
    shl ecx,3 ;*8 (sizeof double)
    
    ;;;malloc of new array;;;
    mov eax,[ebp + 12] ;N
    shl eax,3 ;*8 (sizeof double)
    push ecx
    push edx
    push eax
    call malloc
    add esp,4
    pop edx
    pop ecx
    mov edi,eax
    
    ;;;memcpy to new array;;;
    push ecx
    push esi
    push edi
    call memcpy
    add esp,8
    pop ecx
    
    ;;;sort new array;;;
    shr ecx,3
    push ecx
    push comp
    push dword 8
    push ecx
    push edi
    call qsort
    add esp,16
    pop ecx
    
    ;;;middle value;;;
    ;push ecx
    finit
    shr ecx,1
    jc lichy
    jmp sudy

lichy:
    fld qword [edi + ecx * 8]
    jmp end
    
sudy:
    fld qword [edi + ecx * 8 - 8]
    fld qword [edi + ecx * 8]
    faddp
    fld1
    fld1
    faddp
    fdivp
    jmp end
    
bad_end:
    push -1
    fld dword [esp]
    add esp,4
    
end:
    pop edi
    pop esi
    mov esp,ebp
    pop ebp
    ret