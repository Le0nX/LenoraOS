#Определяем константы для multiboot header.
.set MAGIC, 0x1badb002 			#магическое число, чтобы GRUB распознал
.set ALIGN,	1<<0				#Выровнять загруженные модули на границах страниц
.set MEMINFO, 1<<1				#Карта памяти
.set FLAGS, ALIGN | MEMINFO 	#Multiboot 'flag'. "Так нада"
.set CHECKSUM, -(MAGIC + FLAGS) #чек-сумма, которая палит, что мы multiboot

#Объявляем заголовок, который помечает прогу как ядро.
#Сейчас начнется магия, которая нужна по стандартам Multiboot. 
#Загрузчик будет искать эту сигнатуру в первых 8 KiB файла ядра, выровненную
#на 32-bit границу.

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM


#Конец магии multiboot. Объявляем стандартные секции.
.section .text
.extern main					#вызов main() из другого файла
.global loader


loader:
	mov $kernel_stack, %esp 	#c++ требует ,чтобы stack_pointer был установлен
								#вначале стека.
								
#после того, как bootloader понял, что это ядро, то он сохранит в RAM
#bootloader_structure, в которой будет различная информация о системе.
#После создания	структуры он сохранит указатель на нее в %eax, а MAGIC в %ebx

	push %eax					#передаем указатель на структуру в main				
	push %ebx					#передаем MAGIC в main		
	call main
	
_trap:							#infloop для подстраховки ядра
	cli
	hlt
	jmp _trap

.section .bss					#неинициализированные данные
.space 2*1024*1024				#2 MiB для заглушки перед стеком 
kernel_stack:
