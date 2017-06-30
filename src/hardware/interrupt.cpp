
#include <hardware/interrupt.h>
using namespace lenora::common;
using namespace lenora::hardware;


void printf(const char* str);



InterruptHandler::InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager)
{
	this->interruptNumber = interruptNumber;
	this->interruptManager = interruptManager;
	interruptManager->handlers[interruptNumber] = this;   // добавляем в список хендлеров
}
InterruptHandler::~InterruptHandler()
{
	if (interruptManager->handlers[interruptNumber] == this)
		interruptManager->handlers[interruptNumber] = 0;  // удаляем из списка
}
 
uint32_t InterruptHandler::HandleInterrupt(uint32_t esp)
{
	return esp;
}


InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

InterruptManager* InterruptManager::ActiveInterruptManager = 0;

void InterruptManager::SetInterruptDescriptorTableEntry(
			uint8_t interruptNumber,
			uint16_t codeSegmentSellectorOffset,
			void (*handler)(),
			uint8_t DescriptorPrivilegeLevel,
			uint8_t DescriptorType)
{
	const uint8_t IDT_DESC_PRESENT = 0x80;
	
	interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
	interruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((uint32_t)handler) >> 16) & 0xFFFF;
	interruptDescriptorTable[interruptNumber].gdt_codeSegmentSellector = codeSegmentSellectorOffset;
	interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilegeLevel&3) << 5);
	interruptDescriptorTable[interruptNumber].reserved = 0;
	
}

			
InterruptManager::InterruptManager(GlobalDescriptorTable* gdt)
 : picMasterComand(0x20),     // номера портов
   picMasterData(0x21),
   picSlaveComand(0xA0),
   picSlaveData(0xA1)
{
	uint16_t CodeSegment = gdt->CodeSegmentSelector();
	const uint8_t IDT_INTERRUPT_GATE = 0xE;  // wiki.osdev.org/Interrupts
	
	for(uint16_t i=0; i < 256; i++){
		handlers[i] = 0;
		SetInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
	}
		
	SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);  // HandleInterruptRequest0x00
	SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);  // HandleInterruptRequest0x01
	SetInterruptDescriptorTableEntry(0x2C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);  // HandleInterruptRequest0x0C

	picMasterComand.Write(0x11);
	picSlaveComand.Write(0x11);

	picMasterData.Write(0x20);
	picSlaveData.Write(0x28);

	picMasterData.Write(0x04);   // I
	picSlaveData.Write(0x02);    // N
                                 // I
	picMasterData.Write(0x01);   // T
	picSlaveData.Write(0x01);    // I
                                 // A
	picMasterData.Write(0x00);   // L
	picSlaveData.Write(0x00);    // I Z A T I O N
	
			
	InterruptDescriptorTablePointer idt;
	idt.size = 256 * sizeof(GateDescriptor) - 1;
	idt.base = (uint32_t)interruptDescriptorTable;
	asm volatile("lidt %0" : : "m" (idt));    // говорим cpu начать юзать IDT
}


InterruptManager::~InterruptManager()
{}

void InterruptManager::Deactivate()
{	
	if (ActiveInterruptManager == this){
		
		ActiveInterruptManager = 0;
		asm("cli"); 
	
	}
}

void InterruptManager::Activate()
{	
	if (ActiveInterruptManager != 0)
		ActiveInterruptManager->Deactivate();
	ActiveInterruptManager = this;
	asm("sti");      // start use interrupts!
}


uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
	if (ActiveInterruptManager != 0)
		ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);	
	return esp;
}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
	if (handlers[interruptNumber] != 0){
	
		esp = handlers[interruptNumber]->HandleInterrupt(esp);
		
	} else if (interruptNumber != 0x20){
		
		char text[] = "UNHANDLED INTERRUPT 0x00";
		char hex[] = "0123456789ABCDEF";
		text[22] = hex[(interruptNumber >> 4) & 0x0F];
		text[23] = hex[interruptNumber & 0x0F];
		
		printf(text);
	
	}
	if (0x20 <= interruptNumber && interruptNumber < 0x30){
	
		picMasterComand.Write(0x20);
		if (0x28 <=interruptNumber && interruptNumber < 0x30)
			picSlaveComand.Write(0x20);
			
	}
			
	return esp;
}
