GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt
GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq128Handler
GLOBAL _exception0Handler
GLOBAL _exception6Handler
GLOBAL syscallIntRoutine
GLOBAL pressed_key
GLOBAL regsArray
EXTERN irqDispatcher
EXTERN int80Dispatcher
EXTERN exceptionDispatcher
EXTERN getStackBase
EXTERN syscalls

SECTION .text

%macro pushState 0

	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro exceptionHandler 1
	pushState
	mov rdi, %1 
	mov rsi, rsp
	call exceptionDispatcher

popState
	call getStackBase	

	mov qword [rsp+8*3], rax				
	mov qword [rsp], userland	
	iretq
%endmacro

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret

_sti:
	sti
	ret

picMasterMask:
	push    rbp
   	mov     rbp, rsp
    	mov     ax, di
    	out	    21h,al
    	pop     rbp
    	retn

picSlaveMask:
	push    rbp
    	mov     rbp, rsp
    	mov     ax, di 
    	out	   0A1h, al
    	pop     rbp
    	retn

;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	push rax
	xor 	rax, rax
	in 	al, 0x60 ; guardo la tecla
	mov 	[pressed_key], rax
	cmp 	rax, SNAPSHOT_KEY
	jne 	.doNotCapture

	pop 	rax
	mov 	rax, 15
	mov 	r8, 10
	mov 	[regsArray + 0 * 8],  rax
	mov 	[regsArray + 1 * 8],  rbx
	mov 	[regsArray + 2 * 8],  rcx
	mov 	[regsArray + 3 * 8],  rdx
	mov 	[regsArray + 4 * 8],  rbp
	mov 	[regsArray + 5 * 8],  rdi
	mov 	[regsArray + 6 * 8],  rsi
	mov 	[regsArray + 7 * 8],  r8
	mov 	[regsArray + 8 * 8],  r9
	mov 	[regsArray + 9 * 8],  r10
	mov 	[regsArray + 10 * 8], r11
	mov 	[regsArray + 11 * 8], r12
	mov 	[regsArray + 12 * 8], r13
	mov 	[regsArray + 13 * 8], r14
	mov 	[regsArray + 14 * 8], r15
	mov 	rax, [rsp + 8 * 0] ; rip
	mov 	[regsArray + 15 * 8], rax
	mov 	rax, [rsp + 8 * 1] ; cs
	mov 	[regsArray + 16 * 8], rax
	mov 	rax, [rsp + 8 * 2] ; rflags
	mov 	[regsArray + 17 * 8], rax
	mov 	rax, [rsp + 8 * 3] ; rsp
	mov 	[regsArray + 18 * 8], rax
	mov 	rax, [rsp + 8 * 4] ; ss
	mov 	[regsArray + 19 * 8], rax
	jmp 	.continue

.doNotCapture:
	pop rax

.continue:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Opcode Exception
_exception6Handler:
	exceptionHandler 6

_irq128Handler:
    pushState
    cmp rax, 23
    jge .syscall_end
    call [syscalls + rax * 8]

.syscall_end:
    mov [aux], rax
    popState
    mov rax, [aux]
    iretq

haltcpu:
	cli
	hlt
	ret

SECTION .bss
	aux resq 1
	; # de registros
	regsArray resq 20 
	pressed_key resq 1
	SNAPSHOT_KEY equ 0x1D

SECTION .data 
	userland equ 0x400000 