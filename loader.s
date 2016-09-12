.section .text      #секция кода программы
.extern kernelMain  #объявляем функцию из другого файла(kernel.cpp)
.global loader      #глобальная метка. Точка входа в наш загрузчик

loader:
    mov $kernel_stack, %esp #stack pointer указывающий на стек ядра.
    call kernelMain			#вызов функции
    
 _stop:				#новая метка для inf loop, сраховка после inf loop в kernelMain
 	cli				#запрещаем прерывания от всех внешних устройств. Чистим регистр флагов
 	hlt				#стопит камень
 	jmp _stop

    
    
.section .bss       #Доп. секция
.space 2*1024*1024 	#выделяем 2мб

kernel_stack: