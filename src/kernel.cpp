
#include <common/types.h>
#include <gdt.h>
#include <hardware/interrupt.h>
#include <hardware/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>


using namespace lenora;
using namespace lenora::drivers;
using namespace lenora::common;
using namespace lenora::hardware;

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

void printf(const char* str)
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

void hexPrint(uint8_t key)
 {
     char* x = "00";
     char* hex = "0123456789ABCDEF";
     x[0] = hex[(key >> 4) & 0xF];
     x[1] = hex[key & 0xF];
     printf(x);
 }

/**
 * Вызов конструкторов из секции начинающейся в start_ctors 
 * и заканчивающейся в end_ctors. Создается указатель на    
 * функцию типа void и функция callCoonstructors, которая   
 * вызывает все анонимные функции конструкторов из секции.
 * Это связано с особенностью C++.  
 */
typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
	for (constructor* i = &start_ctors; i != &end_ctors; i++)
		(*i)();
}

class PrintKeyboardEventHandler : public KeyboardEventHandler
{
	public:
		virtual void OnKeyDown(char c)
		{
			char *x = " ";
			x[0] = c;
			printf(x);
		}
};


class ConsoleMouse : public MouseEventHandler
{
		int8_t x, y;
	public:
		ConsoleMouse()
		{}
		
		virtual void OnMouseActivate()
		{
			uint16_t* VideoMem = (uint16_t*)0xb8000;
			x = 40;  // инициализируем мышь по-центру
			y = 12;
			VideoMem[80*y+x] = ((VideoMem[80*y+x] & 0xF000) >> 4) // swap
							 | ((VideoMem[80*y+x] & 0x0F00) << 4) // color
							 |  (VideoMem[80*y+x] & 0x00FF);      // bits	
		}
		
		virtual void OnMouseDown(uint8_t button)
		{
			uint16_t* VideoMem = (uint16_t*)0xb8000;
			VideoMem[80*y+x] = ((VideoMem[80*y+x] & 0xF000) >> 4) // swap
							 | ((VideoMem[80*y+x] & 0x0F00) << 4) // color
							 |  (VideoMem[80*y+x] & 0x00FF);      // bits	
		}
		
		virtual void OnMouseUp(uint8_t button)
		{
			uint16_t* VideoMem = (uint16_t*)0xb8000;
			VideoMem[80*y+x] = ((VideoMem[80*y+x] & 0xF000) >> 4) // swap
							 | ((VideoMem[80*y+x] & 0x0F00) << 4) // color
							 |  (VideoMem[80*y+x] & 0x00FF);      // bits
		}
		
		virtual void OnMouseMove(int x_offset, int y_offset)
		{
			static uint16_t* VideoMem = (uint16_t*)0xb8000;
			VideoMem[80*y+x] = ((VideoMem[80*y+x] & 0xF000) >> 4) // swap
							 | ((VideoMem[80*y+x] & 0x0F00) << 4) // color
							 |  (VideoMem[80*y+x] & 0x00FF);      // bits
											
			x += x_offset;
			if (x<0) x = 0;
			if (x>=80) x = 79;
			
			y += y_offset;
			if (y<0) y = 0;
			if (y>=25) y = 24;
			
			VideoMem[80*y+x] = ((VideoMem[80*y+x] & 0xF000) >> 4) // swap
							 | ((VideoMem[80*y+x] & 0x0F00) << 4) // color
							 |  (VideoMem[80*y+x] & 0x00FF);       // bits
		}
};

extern "C" void kmain(void *multiboot_struct, uint32_t MAGIC)
{

	GlobalDescriptorTable gdt;
	InterruptManager interrupts(&gdt);
	printf("Initializing GDT & Hardaware...\n");
	
	DriverManager drvManager;
	
	PrintKeyboardEventHandler kbhandler;
	KeyboardDriver keyboard(&interrupts, &kbhandler);
	drvManager.AddDriver(&keyboard);
	printf("Keyboard is ready...\n");
	
	ConsoleMouse mshandler;
	MouseDriver mouse(&interrupts, &mshandler);
	drvManager.AddDriver(&mouse);
	printf("Mouse is ready...\n");
	
	PCIcontroller PCIController;
	PCIController.SelectDrivers(&drvManager, &interrupts);
	
	drvManager.ActivateAll();
	printf("Activating all drivers.\n");

	interrupts.Activate();
	printf("Interrupts are activated.\n");
	
	printf("Welcome to Lenora OS...\n");
	printf("Moscow 2017\n");
	
	while(1);
}
