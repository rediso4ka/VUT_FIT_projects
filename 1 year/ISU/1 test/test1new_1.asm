%include "rw32-2018.inc"

section .data
    a db -128
    b dd 2147483
    c dw 32761
    d dw -5552
    e dw 4333
    q dd 0
    r dd 0

section .text
_main:
    push ebp
    mov ebp, esp
    
    mov eax, 0
    mov ebx, 0
    mov ecx, 0
    mov edx, 0
    ;q=(a*b + c + 24)/(9*d + e + 124)
    ;r=(a*b + c + 24)%(9*d + e + 124)
    
    movsx eax, byte[a]
    mov ebx, [b]
    imul ebx
    
    movsx ebx, word[c]
    add eax, ebx
    adc edx, 0
    
    add eax, 24
    adc edx, 0
    
    push edx
    push eax
    
    xor edx, edx
    xor eax, eax
    mov ax, [d]
    mov bx, 9
    imul bx ;dx:ax =9*d
    rol edx, 16
    add eax, edx ;eax =9*d
    
    movsx ebx, word[e]
    add eax, ebx
    add eax, 124
    
    mov ebx, eax
    pop eax
    pop edx
    
    idiv ebx
    call WriteInt32
    mov [q], eax
    mov eax, edx
    call WriteNewLine
    call WriteInt32
    mov [r], edx

    pop ebp
    ret