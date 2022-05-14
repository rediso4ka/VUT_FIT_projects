%include "rw32-2018.inc"

section .data
    a dd 0xff551100

section .text
_main:
    push ebp
    mov ebp, esp
    
    mov eax,[a]
    ror eax,8
    ror ax,8
    ror eax,8

    pop ebp
    ret