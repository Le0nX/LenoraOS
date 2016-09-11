.section .text      ;секция кода программы
.extern kernelMain  ;объявляем функцию из другого файла(kernel.cpp)
.global loader      ;глобальная метка. Точка входа в наш загрузчик

loader:
    mov $kernel_stack, %esp ;stack pointer указывающий на стек ядра.
    
    
    
    
.section .bss       ;Доп. секция

kernel_stack: