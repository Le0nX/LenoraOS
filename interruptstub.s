.set IRQ_BASE, 0x20  #зарезервированны CPU для Exceptions

.section .text

.extern _ZN16InterruptManager15handleInterruptEhj
.global _ZN16InterruptManager22IgnoreInterruptRequestEv



.macro HandleException num
.global _ZN16InterruptManager16HandleException\num\()Ev
_ZN16InterruptManager16HandleException\num\()Ev:
	movb $\num, (interruptnumber)
	jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
	movb $\num + IRQ_BASE, (interruptnumber)
	jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x0C




int_bottom:
	
#сохраняем регистры и регистры сегментов
	pusha
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

#вызываем C++ функцию по ее хендлу
	push %esp
	push (interruptnumber)
	call _ZN16InterruptManager15handleInterruptEhj
	# addl $5, %esp
	movl %eax, %esp
	
#восстанавливаем регистры и регистры сегментов	
	popl %gs
	popl %fs
	popl %es
	popl %ds
	popa
	
_ZN16InterruptManager22IgnoreInterruptRequestEv:	
	
	iret

	
.data
	interruptnumber: .byte 0
	
