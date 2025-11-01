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