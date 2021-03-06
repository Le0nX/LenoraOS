
#include <drivers/mouse.h>
using namespace lenora::drivers;
using namespace lenora::common;
using namespace lenora::hardware;

void printf(const char*);

MouseEventHandler::MouseEventHandler()
{
	
}

void MouseEventHandler::OnMouseActivate()
{
	
}

void MouseEventHandler::OnMouseDown(uint8_t button)
{
	
}

void MouseEventHandler::OnMouseUp(uint8_t button)
{
	
}

void MouseEventHandler::OnMouseMove(int x, int y)
{
	
}




MouseDriver::MouseDriver(InterruptManager* manager, MouseEventHandler* handler)
: InterruptHandler(0x2C, manager),  //number of mouse interrupt
  dataport(0x60),
  commandport(0x64)
{
	this->handler = handler;
}

MouseDriver::~MouseDriver()
{}

void MouseDriver::Activate()
{
	offset = 0;
	buttons = 0;
	
	if (handler != 0)
		handler->OnMouseActivate();
	printf("passed if statement");
	
	commandport.Write(0xA8);   // активировать прерывания мыши
	commandport.Write(0x20);   // получить текущее состояние
	uint8_t status = dataport.Read() | 2;// set второй бит в 1
	commandport.Write(0x60);   // сменить текущее состояние
	dataport.Write(status);    // записываем state обратно
	
	commandport.Write(0xD4);
	dataport.Write(0xF4);
	dataport.Read();	
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{
	uint8_t status = commandport.Read();
	if (!(status & 0x20))  // если 6 бит не 1, то все
		return esp;
				
	buffer[offset] = dataport.Read();	
	
	if (handler == 0)
		return esp;
		
	offset = (offset + 1) % 3;  // смещаем на 1 в массиве по 3 байта

	if (offset == 0){
		
		if (buffer[1] != 0 || buffer[2] != 0)
			handler->OnMouseMove(buffer[1], -buffer[2]);
					     
		for (uint8_t i = 0; i < 3; i++){
		
			if ((buffer[0] & (0x01 << i)) != (buttons & (0x01 << i))){
				
				//PRESSED
				if (buttons & (0x01 << i))
					handler->OnMouseUp(i+1);
				else 
					handler->OnMouseDown(i+1);				
			}			
		}
		
		buttons = buffer[0];
	}
		
	return esp;
}
