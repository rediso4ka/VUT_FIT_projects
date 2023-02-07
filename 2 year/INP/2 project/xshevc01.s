; Autor reseni: Aleksandr Shevchenko xshevc01

; Projekt 2 - INP 2022
; Vernamova sifra na architekture MIPS64

; DATA SEGMENT
                .data
login:          .asciiz "xshevc01"  ; sem doplnte vas login
cipher:         .space  17  ; misto pro zapis sifrovaneho loginu

params_sys5:    .space  8   ; misto pro ulozeni adresy pocatku
                            ; retezce pro vypis pomoci syscall 5
                            ; (viz nize "funkce" print_string)

; CODE SEGMENT
                .text

main:
                ; USED REGISTERS:
                ; r11   - storing first letter of cipher (s)
                ; r24   - storing second letter of cipher (h)
                ; r17   - calculations
                ; r26   - result value of letter
                ; r0    - zero
                ; r4    - counter in loop, address of cipher in the end

                DADDI   r11, r0, 19         ; letter s
                DADDI   r24, r0, 8          ; letter h
                DADDI   r4, r0, 0           ; init r4 (counter)


                loop_start:                     ; reading all letters of login
                    LBU     r26, login(r4)      ; r26 = x
                    DADDI   r17, r0, 96         ; r17 = 96
                    DSUB    r17, r17, r26       ; r17 -= r26
                    BEQ     r26, r0, exit       ; end of string
                    BGEZ    r17, exit           ; we found a number

                    ;_________________________________
                    ; 97 + (x - 97 (+ 19 OR - 8)) % 26
                    ;_________________________________

                    DADDI   r17, r0, 97         ; r17 = 97
                    DSUB    r26, r26, r17       ; r26 = x - 97

                    ANDI    r17, r4, 1          ; r4 even -> r17 = 0
                                                ;    odd  -> r17 = 1
                    BEQ     r17, r0, even_part  ; jump to +s

                    odd_part:
                    DSUB    r26, r26, r24       ; r26 = x - 97 - 8 = A
                    B       end_if
                    even_part:
                    DADD    r26, r26, r11       ; r26 = x - 97 + 19 = A
                    end_if:

                    DADDI   r17, r0, 26         ; r17 = 26
                    DSUB    r26, r26, r17       ; r26 = A - 26
                    BGEZ    r26, no_more_add
                    DADDI   r26, r26, 26
                    BGEZ    r26, no_more_add
                    DADDI   r26, r26, 26
                    no_more_add:                ; r26 = A % 26
                    DADDIU  r26, r26, 97        ; r26 = 97 + A % 26
                
                    SB      r26, cipher(r4)     ; store letter to cipher
                    DADDIU  r4, r4, 1           ; cnt++
                    B       loop_start          ; looping

                exit:
                SB      r0, cipher(r4)      ; store end of line
                DADDI   r4, r0, cipher      ; adress of cipher to r4
                JAL     print_string        ; printing the result

                syscall 0   ; halt

print_string:   ; adresa retezce se ocekava v r4
                sw      r4, params_sys5(r0)
                daddi   r14, r0, params_sys5    ; adr pro syscall 5 musi do r14
                syscall 5   ; systemova procedura - vypis retezce na terminal
                jr      r31 ; return - r31 je urcen na return address
