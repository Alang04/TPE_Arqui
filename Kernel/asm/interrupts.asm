EXTERN exceptionDispatcher
EXTERN irqDispatcher
EXTERN int80Dispatcher

; --- MACRO para guardar todos los registros ---
%macro PUSHAQ 0
    push    rax
    push    rbx
    push    rcx
    push    rdx
    push    rsi
    push    rdi
    push    r8
    push    r9
    push    r10
    push    r11
    push    r12
    push    r13
    push    r14
    push    r15
%endmacro

; --- MACRO para restaurar todos los registros ---
%macro POPAQ 0
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     rdi
    pop     rsi
    pop     rdx
    pop     rcx
    pop     rbx
    pop     rax
%endmacro

; Macro para crear un handler de excepción SIN código de error
%macro ISR_NO_ERROR_CODE 1
    GLOBAL _exception%1Handler
    _exception%1Handler:
        cli
    .halt_loop_%1:
        hlt
        jmp .halt_loop_%1
%endmacro

; Macro para crear un handler de excepción CON código de error
%macro ISR_ERROR_CODE 1
    GLOBAL _exception%1Handler
    _exception%1Handler:
        cli
    .halt_loop_err_%1:
        hlt
        jmp .halt_loop_err_%1
%endmacro

; Macro para crear un handler de IRQ
%macro IRQ 2
    GLOBAL _irq%1Handler
    _irq%1Handler:
        cli
    .halt_loop_irq_%1:
        hlt
        jmp .halt_loop_irq_%1
%endmacro

; --- Declaración de Handlers de Excepciones (CORREGIDO) ---
ISR_NO_ERROR_CODE 0     ; 0: Division por cero
ISR_NO_ERROR_CODE 1     ; 1: Debug
ISR_NO_ERROR_CODE 2     ; 2: NMI
ISR_NO_ERROR_CODE 3     ; 3: Breakpoint
ISR_NO_ERROR_CODE 4     ; 4: Overflow
ISR_NO_ERROR_CODE 5     ; 5: Bound Range Exceeded
ISR_NO_ERROR_CODE 6     ; 6: Invalid Opcode
ISR_NO_ERROR_CODE 7     ; 7: Device Not Available
ISR_ERROR_CODE    8     ; 8: Double Fault
ISR_NO_ERROR_CODE 9     ; 9: Coprocessor Segment Overrun
ISR_ERROR_CODE    10    ; 10: Invalid TSS
ISR_ERROR_CODE    11    ; 11: Segment Not Present
ISR_ERROR_CODE    12    ; 12: Stack-Segment Fault
ISR_ERROR_CODE    13    ; 13: General Protection Fault
ISR_ERROR_CODE    14    ; 14: Page Fault
ISR_NO_ERROR_CODE 15    ; 15: (Reservado)
ISR_NO_ERROR_CODE 16    ; 16: x87 FPU Error
ISR_ERROR_CODE    17    ; 17: Alignment Check
ISR_NO_ERROR_CODE 18    ; 18: Machine Check
ISR_NO_ERROR_CODE 19    ; 19: SIMD Floating-Point
ISR_NO_ERROR_CODE 20    ; 20: Virtualization
ISR_ERROR_CODE    21    ; 21: Control Protection
ISR_NO_ERROR_CODE 22
ISR_NO_ERROR_CODE 23
ISR_NO_ERROR_CODE 24
ISR_NO_ERROR_CODE 25
ISR_NO_ERROR_CODE 26
ISR_NO_ERROR_CODE 27
ISR_NO_ERROR_CODE 28
ISR_ERROR_CODE    29
ISR_ERROR_CODE    30
ISR_NO_ERROR_CODE 31

; --- Declaración de IRQs ---
IRQ 0, 0                ; IRQ 0: Timer Tick
IRQ 1, 1                ; IRQ 1: Teclado
IRQ 2, 2
IRQ 3, 3
IRQ 4, 4
IRQ 5, 5
IRQ 6, 6
IRQ 7, 7
IRQ 8, 8
IRQ 9, 9
IRQ 10, 10
IRQ 11, 11
IRQ 12, 12
IRQ 13, 13
IRQ 14, 14
IRQ 15, 15

; --- Declaración de Syscall (Int 0x80) ---
GLOBAL _int80Handler
_int80Handler:
    push    rbp
    mov     rbp, rsp
    PUSHAQ
    sub     rsp, 8      ; Alineamos la pila

    ; Preparar argumentos para int80Dispatcher:
    ; RDI = RAX (nro de syscall)
    ; RSI = RBX (arg1)
    ; RDX = RCX (arg2)
    ; RCX = RDX (arg3)  -> usamos R8 como temporal
    mov     r8, rdx
    mov     rdi, rax
    mov     rsi, rbx
    mov     rdx, rcx
    mov     rcx, r8

    call    int80Dispatcher

    ; Resultado devuelto en RAX (ABI). No escribimos en la pila del caller.

    add     rsp, 8      ; Restauramos la pila
    POPAQ
    pop     rbp
    iretq