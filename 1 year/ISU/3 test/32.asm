%include "rw32-2018.inc"

section .data
    ; zde budou vase data

section .text
_main:
    push ebp
    mov ebp, esp
    
    push __float32__(25.650) ;y    [ebp + 12]
    push __float32__(20.650) ;x    [ebp + 8]
    call task32
    add esp,8

    pop ebp
    ret
    
task32:
    push ebp
    mov ebp,esp
    
    fld dword [ebp + 8]
    push __float32__(6.650)
    fld dword [esp]
    add esp,4
    
    faddp
    fsqrt
    
    fld dword [ebp + 8]
    fldpi
    fld dword [ebp + 12]
    
    fmulp
    fsubp
    fsin
    fmulp
    
    
    fld dword [ebp + 8]
    fld dword [ebp + 12]
    push 4
    fild dword [esp]
    add esp,4
    
    fmulp
    fsubp
    fabs
    fdivp
    
    mov esp,ebp
    pop ebp
    ret