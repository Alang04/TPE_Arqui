GLOBAL sys_write
GLOBAL sys_read
GLOBAL sys_registers
GLOBAL sys_time
GLOBAL sys_date
GLOBAL sys_clear
GLOBAL sys_increase_fontsize
GLOBAL sys_decrease_fontsize
GLOBAL sys_ticks
GLOBAL sys_beep

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

sys_increase_fontsize:
    	mov rax, 5
	int 0x80
	ret

sys_decrease_fontsize:
   	mov rax, 6
	int 0x80
	ret

sys_beep:
    	mov rax, 7
	int 0x80
	ret

sys_ticks:
    mov rax, 8
    int 0x80
    ret

sys_clear:
    mov rax, 13
	int 0x80
	ret
