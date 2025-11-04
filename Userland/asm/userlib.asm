; Wrappers de syscalls (int 0x80) - ver índices en Kernel/c/syscallDispatcher.c
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
GLOBAL sys_speaker_start
GLOBAL sys_speaker_off
GLOBAL gen_invalid_opcode

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
    mov rax, 9
	int 0x80
	ret

; No bloqueante: enciende el parlante a 'freq' Hz (0 apaga)
sys_speaker_start:
	mov rax, 10
	int 0x80
	ret

; No bloqueante: apaga el parlante
sys_speaker_off:
	mov rax, 11
	int 0x80
	ret

gen_invalid_opcode:
    ud2
    ret