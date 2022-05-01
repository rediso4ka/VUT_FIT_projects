%include "rw32-2018.inc"

section .data


CEXTERN malloc
CEXTERN free
section .text
task23:
    push ebp
    mov ebp,esp
    push esi
    push ebx
    push edx
    xor eax,eax
    cmp ecx,0
    jle task_end
    mov eax,ecx ;pocet cisel
    shl eax,2 ;*4
    
    push ecx
    push edx
    push eax
    call malloc
    add esp,4
    pop edx
    pop ecx
    cmp eax,0
    jz task_end

    
    mov esi,eax

    xor ebx,ebx ;ebx = i
    
first:
    cmp ebx,ecx
    je task_end
    mov [eax + 4*ebx],dword 1
    inc ebx
    
second:
    cmp ebx,ecx
    je task_end
    mov [eax + 4*ebx],dword 1
    inc ebx 
    
third:
    cmp ebx,ecx
    je task_end
    mov [eax + 4*ebx],dword 1
    inc ebx
    
cycle:
    cmp ebx,ecx
    je task_end
    mov [eax + 4*ebx],dword 0
    mov edx,[eax + 4*ebx - 12]
    add [eax + 4*ebx],edx
    mov edx,[eax + 4*ebx - 8]
    adc [eax + 4*ebx],edx
    inc ebx
    jmp cycle
    
task_end:
    pop edx
    pop ebx
    pop esi
    mov esp,ebp
    pop ebp
    ret
    
_main:
    push ebp
    mov ebp, esp
    
    mov ecx,10
    call task23
    
    mov esi,eax
    call WriteArrayInt32
    
    push eax
    call free
    add esp,4
    
    pop ebp
    ret