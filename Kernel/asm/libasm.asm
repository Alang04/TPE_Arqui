GLOBAL cpuVendor

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

GLOBAL inb
inb:
    push rbp
    mov rbp, rsp
    mov rax, 0
    mov rdx, rdi  ; RDI tiene el número de puerto (ej: 0x60)
    in al, dx     ; Leer 1 byte del puerto en AL
    pop rbp
    ret

GLOBAL outb
outb:
    push rbp
    mov rbp, rsp
    mov rdx, rdi  ; RDI tiene el número de puerto (ej: 0x20)
    mov rax, rsi  ; RSI tiene el valor a escribir
    out dx, al    ; Escribir 1 byte en el puerto
    pop rbp
    ret

GLOBAL _sti
_sti:
    sti
    ret