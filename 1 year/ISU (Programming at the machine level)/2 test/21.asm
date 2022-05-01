%include "rw32-2018.inc"

section .data
    pocet dd 5
    array dw 12,122,11,43,56
    hledame dd 122

section .text
task21:
    ;dec ecx
    cmp ecx,0
    jz bad_end
    dec ecx
    cmp [eax + 2*ecx],bx
    jnz task21
    mov eax,1
    jmp task_exit
bad_end:
    mov eax,0
task_exit:
    ret

_main:
    push ebp
    mov ebp, esp
    
    mov eax,array
    mov bx,[hledame]
    mov ecx,[pocet]
    call task21
    
    call WriteInt32
    
    pop ebp
    ret