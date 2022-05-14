%include "rw32-2018.inc"

section .data
    a db 20
    b dd 350
    c dw 40
    d dw 30
    e dw 50
    q dd 0
    r dd 0

section .text
_main:
    push ebp
    mov ebp, esp
    
    ; a * b
    mov eax,[b]
    movsx ebx,byte [a]
    mul ebx
    
    ; + c
    movzx ecx,word [c]
    add eax,ecx
    adc edx,0
    
    ; + 24
    add eax,24
    adc edx,0
    
    mov ecx,eax
    mov edi,edx
        
    ; 9 * d
    mov ax,[d]
    mov bx,9
    mul bx
    
    ; + e
    add ax,[e]
    adc dx,0
    
    ; + 124
    add ax,124
    adc dx,0
    
    ; perenos dx:ax do ebx
    movzx ebx,dx
    rol ebx,16
    add bx,ax
    
    ; delenie
    mov eax,ecx
    mov edx,edi
    div ebx
    mov [q],eax
    mov [r],edx

    pop ebp
    ret