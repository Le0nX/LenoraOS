ENTRY(loader) /*Здесь мы говорим линкеру, что метка loader в загрузчике точка входа в наш финальный бинарник ядра.*/
OUTPUT_FORMAT(elf32-i386) /*выходной формат*/
OUTPUT_ARCH(i386:i386)	/*это не Арч ,а архитектура,лол*/

/*Далее мы выделяем различные секции из разных .о файлов и комбинируем их*/

SECTIONS
{
	. = 0x0100000; /*Лол, это в общем специальный скип на 1мб в памяти, чтобы GRUB начал свои делишки*/

	.text :
	{
		*(.multiboot)
		*(.text)
		*(.rodata) /*думаю, что здесь .bss*/
	}

	.data :
	{
		start_ctors = .; /*global vars*/
		KEEP(*( .init_array ));
		KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
		end_ctors = .;

		*(.data)
	}

	.bss :
	{
		*(.bss)
	}

	/DISCARD/ :
	{
		*(.fini_array*)
		*(.comment)
	}
}
