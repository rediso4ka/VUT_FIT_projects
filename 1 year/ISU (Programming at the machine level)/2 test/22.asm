%include "rw32-2018.inc"

section .data
    pA dd 9,6,7,8,9,0,11,21,66,67,9
    N dd 11
    x dd 8

section .text
task22:
    push ebp
    mov ebp,esp
    push ecx
    push esi
    push ebx
    xor eax,eax       ;counter
    cmp [ebp+16],eax
    jz bad_end
    cmp [ebp+12],eax
    jl bad_end
    mov esi,[ebp+16]
    mov ebx,-1       ;i
    mov ecx,[ebp+8] ;to compare with
    jmp cycle
    
cycle_with_plus:
    inc eax 
cycle:
    inc ebx
    cmp ebx,[ebp+12]
    jz task_end
    cmp [esi + 4*ebx],ecx
    jg cycle_with_plus
    jmp cycle

bad_end:
    mov eax,-1
task_end:
    pop ebx
    pop esi
    pop ecx
    mov esp,ebp
    pop ebp
    ret 3*4
    
    
_main:
    push ebp
    mov ebp, esp
    
    push pA
    push dword [N]    
    push dword [x]
    call task22
    
    call WriteInt32
    
    pop ebp
    ret