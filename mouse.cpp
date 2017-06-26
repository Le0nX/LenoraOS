
#include "mouse.h"

void printf(const char*);

MouseDriver::MouseDriver(InterruptManager* manager)
: InterruptHandler(0x2C, manager),  //number of mouse interrupt
  dataport(0x60),
  commandport(0x64)
{
	offset = 0;
	buttons = 0;
	
	uint16_t* VideoMem = (uint16_t*)0xb8000;
	VideoMem[80*12+40] = ((VideoMem[80*12+40] & 0xF000) >> 4) // swap
				     | ((VideoMem[80*12+40] & 0x0F00) << 4) // color
				     | (VideoMem[80*12+40] & 0x00FF);      // bits	
	
	commandport.Write(0xA8);   // активировать прерывания мыши
	commandport.Write(0x20);   // получить текущее состояние
	uint8_t status = dataport.Read() | 2;// set второй бит в 1
	commandport.Write(0x60);   // сменить текущее состояние
	dataport.Write(status);    // записываем state обратно
	
	commandport.Write(0xD4);
	dataport.Write(0xF4);
	dataport.Read();
}

MouseDriver::~MouseDriver()
{}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{
	uint8_t status = commandport.Read();
	if (!(status & 0x20))  // если 6 бит не 1, то все
		return esp;
		
	static int8_t x = 40,y = 12;   // инициализируем мышь по центру	
		
	buffer[offset] = dataport.Read();	
	offset = (offset + 1) % 3;  // смещаем на 1 в массиве по 3 байта

	if (offset == 0){
		
		uint16_t* VideoMem = (uint16_t*)0xb8000;
		VideoMem[80*y+x] = ((VideoMem[80*y+x] & 0xF000) >> 4) // swap
					     | ((VideoMem[80*y+x] & 0x0F00) << 4) // color
					     | (VideoMem[80*y+x] & 0x00FF);      // bits
					     				
		x += buffer[1];
		if (x<0) x = 0;
		if (x>=80) x = 79;
		
		y -= buffer[2];
		if (y<0) y = 0;
		if (y>=25) y = 24;
		
		VideoMem[80*y+x] = ((VideoMem[80*y+x] & 0xF000) >> 4) // swap
					     | ((VideoMem[80*y+x] & 0x0F00) << 4) // color
					     | (VideoMem[80*y+x] & 0x00FF);      // bits
		}
		
	return esp;
}
