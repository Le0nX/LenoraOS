
#include "types.h"
#include "gdt.h"

enum vga_color{
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

void printf(int8_t *str)
{
	uint16_t* VideoMem = (uint16_t*)0xb8000;
	
	static uint8_t x=0, y=0;   // x-символ, y-строка. Всего 25 строк по 80
	
	for (int32_t i=0; str[i] != '\0'; ++i){
		
		switch(str[i])
		{	
			case '\n':
					y++;
					x=0;
					break;
			
			default:
					VideoMem[80*y+x] = (VideoMem[80*y+x] & 0xFF00) | str[i];
					x++;
					break;
		}
		
		if(x >= 80){
			y++;
			x=0;
		}
		
		if(y >= 25){
			
			for(y = 0; y < 25; y++)
				for(x = 0; x < 80; x++)
					VideoMem[80*y+x] = (VideoMem[80*y+x] & 0xFF00) | ' ';
			
			x=0;
			y=0;
		}
	}
}

/**
 * Вызов конструкторов из секции начинающейся в start_ctors 
 * и заканчивающейся в end_ctors. Создается указатель на    
 * функцию типа void и функция callCoonstructors, которая   
 * вызывает все анонимные функции конструкторов из секции.  
 */
typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
	for (constructor* i = &start_ctors; i != &end_ctors; i++)
		(*i)();
}

extern "C" void kmain(void *multiboot_struct, uint32_t MAGIC)
{
	printf("Welcome to Lenora OS...\n");
	printf("Moscow 2017");

	GlobalDescriptorTable gdt;
	
	while(1);
}
