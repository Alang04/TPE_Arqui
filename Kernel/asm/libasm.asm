GLOBAL cpuVendor
GLOBAL getTime
GLOBAL getPressedKey
GLOBAL getSeconds
GLOBAL getMinutes
GLOBAL getHour
GLOBAL getDayOfMonth
GLOBAL getMonth
GLOBAL getYear
GLOBAL inb
GLOBAL outb
GLOBAL printRegisters
EXTERN newLine
EXTERN setTextSize
EXTERN videoPrint
EXTERN intToHexa

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid

	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

getTime:
	push rbp
	mov rbp, rsp 

	call getHour
	mov [rdi], al

	call getMinutes
	mov [rdi+1], al

	call getSeconds
	mov [rdi+2], al 

	leave
	ret

getSeconds:
	mov al, 0
	out 0x70, al
	in al, 0x71
	ret

getMinutes:
	mov al, 2
	out 0x70, al
	in al, 0x71
	ret

getHour:
	mov al, 4
	out 0x70, al
	in al, 0x71
	ret

getDayOfWeek:
	mov al, 6
	out 0x70, al
	in al, 0x71
	ret

getMonth:
	mov al, 8
	out 0x70, al
	in al, 0x71
	ret

getYear:
	mov al, 9
	out 0x70, al
	in al, 0x71
	ret

getDayOfMonth:
	mov al, 7
	out 0x70, al
	in al, 0x71
	ret

getPressedKey:
	xor rax, rax 
.loop:
	in al, 64h
	and al, 0x01
	jz .loop
	in al, 60h
	ret

outb:
	push rbp
	mov rbp, rsp

	mov dx, di
	mov al, sil
	out dx, al

	pop rbp
	ret

inb:
	push rbp	
	mov rbp, rsp

	mov dx, di 
	in al, dx 

	pop rbp
	ret

printRegisters:
    push rbp
    mov rbp, rsp
    xor r10, r10

.loop:
    mov rdi, 1
    call setTextSize

    mov rdi, [registers + r10]
    mov rsi, 0xFFFFFF
    call videoPrint

    add r10, 8
    mov rdi, [rbp + r10 + 8]
    mov rsi, buffRegs
    call intToHexa

    mov rdi, buffRegs
    mov rsi, 0xFFFFFF
    call videoPrint

    call newLine

    cmp r10, length
    jne .loop

    mov rsp, rbp
    pop rbp

    ret

section .bss 
    buffRegs resb 17

section .data
    segmentSS db " SS = ", 0
    rflags db " RFLAGS = ", 0
    segmentCS db " CS = ", 0

    registerRIP db " RIP = ", 0
    registerRAX db " RAX = ", 0
    registerRBX db " RBX = ", 0
    registerRCX db " RCX = ", 0
    registerRDX db " RDX = ", 0
    registerRBP db " RBP = ", 0
    registerRDI db " RDI = ", 0
    registerRSI db " RSI = ", 0
    registerRSP db " RSP = ", 0
    registerR8 db  " R8 = ", 0
    registerR9 db  " R9 = ", 0
    registerR10 db " R10 = ", 0
    registerR11 db " R11 = ", 0
    registerR12 db " R12 = ", 0
    registerR13 db " R13 = ", 0
    registerR14 db " R14 = ", 0
    registerR15 db " R15 = ", 0

    registers dq  registerRAX, registerR15, registerR14, registerR13, registerR12, registerR11, registerR10,registerR9, registerR8, registerRSI, registerRDI, registerRBP, registerRDX, registerRCX, registerRBX, registerRIP, segmentCS, rflags, registerRSP, segmentSS

    length equ $-registers