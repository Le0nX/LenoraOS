
#include "keyboard.h"

void printf(char*);

KeyboardDriver::KeyboardDriver(InterruptManager* manager)
: InterruptHandler(0x21, manager),
  dataport(0x60),
  commandport(0x64)
{
	while(commandport.Read() & 0x1)   // если зажата клавиша
		dataport.Read();
	commandport.Write(0xAE);   // активировать прерывания клавиатуры
	commandport.Write(0x20);   // получить текущее состояние
	uint8_t status = (dataport.Read() | 1) & ~0x10; // set правый бит в 1 и clear 5й бит
	commandport.Write(0x60);   // сменить текущее состояние
	dataport.Write(status);    // записываем state обратно
	
	dataport.Write(0xF4);
}

KeyboardDriver::~KeyboardDriver()
{}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
	uint8_t key = dataport.Read();	
	
    char* text = "KEY PRESSED 0x00";
    char* hex = "0123456789ABCDEF";
    text[13] = hex[(key >> 4) & 0x0F];
    text[14] = hex[key & 0x0F];  
    printf(text);	
		
	return esp;
}
