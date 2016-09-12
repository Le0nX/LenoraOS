.set MAGICNUM, 0x1badb002 #создали MAGICNUM со значением магического числа, чтобы GRUB распознал наше ядро.
.set ALIGN, 1<<0 		  #выравнивание по границам
.set MEMINFO, 1<<1 		  #карта памяти 
.set CHECKSUM, -(MAGICNUM + FLAGS) #контрольная сумма, чтобы понять, что мы в multiboot

.section .multiboot
	.align 4
	.long MAGIC
	.long FLAGS
	.long CHECKSUM

.section .text      #секция кода программы
.extern kernelMain  #объявляем функцию из другого файла(kernel.cpp)
.global loader      #глобальная метка. Точка входа в наш загрузчик

loader:
    mov $kernel_stack, %esp #stack pointer указывающий на стек ядра.
    push %eax
    push %ebx
    call kernelMain			#вызов функции
    
 _stop:				#новая метка для inf loop, сраховка после inf loop в kernelMain
 	cli				#запрещаем прерывания от всех внешних устройств. Чистим регистр флагов
 	hlt				#стопит камень
 	jmp _stop

    
    
.section .bss       #Доп. секция
.space 2*1024*1024 	#выделяем 2мб

kernel_stack: