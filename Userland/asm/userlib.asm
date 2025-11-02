global sys_write
global sys_read
global sys_registers
global sys_time
global sys_date

section .text

sys_registers:
	mov rax, 0
	int 0x80
	ret

sys_time:
	mov rax, 1
	int 0x80
	ret

sys_date:
	mov rax, 2
	int 0x80
	ret

sys_read:
	mov rax, 3
	int 0x80
	ret

sys_write:
	mov rax, 4
	int 0x80
	ret
