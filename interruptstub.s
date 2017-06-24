.set IRQ_BASE, 0x20

.section .text

.extern _ZN16InterruptManager15handleInterruptEhj



.macro HandleException num
.global _ZN16InterruptManager16handleException\num\()Ev
	movb $\num, (interruptnumber)
	jmp int_bottom
.endm

.macro HandleInterrupRequest num
.global _ZN16InterruptManager26handleInterruptRequest\num\()Ev
	movb $\num + IRQ_BASE, (interruptnumber)
	jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01




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
	
	
	iret

	
.data
	interruptnumber: .byte 0
	
