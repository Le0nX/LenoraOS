
#include "keyboard.h"

void printf(const char*);

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
	
	static bool shift = false;
		
	switch(key)
	{
		//case 0xFA: break;               // init key пропускаем
		//case 0x45: case 0xC5:  break;   // virtual box охуел. numlock активирует. Пропускаем...
		
		//блок для цифр
		case 0x02: if (shift) printf("!"); else printf("1"); break;
		case 0x03: if (shift) printf("@"); else printf("2"); break;
		case 0x04: if (shift) printf("#"); else printf("3"); break;
		case 0x05: if (shift) printf("$"); else printf("4"); break;
		case 0x06: if (shift) printf("%"); else printf("5"); break;
		case 0x07: if (shift) printf("^"); else printf("6"); break;
		case 0x08: if (shift) printf("&"); else printf("7"); break;
		case 0x09: if (shift) printf("*"); else printf("8"); break;
		case 0x0A: if (shift) printf("("); else printf("9"); break;
		case 0x0B: if (shift) printf(")"); else printf("0"); break;
		
		
		//блок для первой строки q-]
		case 0x10: printf("q"); break;
		case 0x11: printf("w"); break;
		
		
		//блок для второй строки a-\n
		case 0x1E: printf("a"); break;
		case 0x1F: printf("s"); break;
		case 0x20: printf("d"); break;
		
		
		//блок для третьей строки z-/
		case 0x2C: printf("z"); break;
		
		//stuff
		case 0x1C: printf("\n"); break;
		case 0x39: printf(" "); break;
		case 0x2A: case 0x36: shift = true; break;
		case 0xAA: case 0xB6: shift = false; break;
		
		
		//игнорим
		case 0x45: break;
		
		default:
			
			if(key < 0x80){  // пропускаем коды отпуска клавиши. Только нажатие.

				char text[] = "KEY PRESSED 0x00";
				char hex[] = "0123456789ABCDEF";
				text[13] = hex[(key >> 4) & 0x0F];
				text[14] = hex[key & 0x0F];  
				printf(text);
				printf("\n");
				break;	
			}
	}	
	
	return esp;
}
