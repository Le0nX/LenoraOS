#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "types.h"
#include "port.h"
#include "gdt.h"

  class InterruptManager;

  class InterruptHandler
  {
	  protected:
			uint8_t interruptNumber;
			InterruptManager* interruptManager;
			
			InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager);
			~InterruptHandler();
	  public: 
			virtual uint32_t HandleInterrupt(uint32_t esp);
  };

  class InterruptManager
  {	
	  friend class InterruptHandler;  
	  protected:
	  
	  static InterruptManager* ActiveInterruptManager;
	  InterruptHandler* handlers[256];
	  
	  // Структура, описывающая шлюз прерываний.
			struct GateDescriptor
			{
				uint16_t    handlerAddressLowBits;    // Младшие 16 битов адреса, куда происходи переход в случае возникновения прерывания.
				uint16_t    gdt_codeSegmentSellector;  // Переключатель сегмента ядра.
				uint8_t     reserved;                 // Это значение всегда должно быть нулевым.
				uint8_t     access;                   // Флаги
				uint16_t    handlerAddressHighBits;   // Старшие 16 битов адреса, куда происходи переход.
				
			} __attribute__((packed));
			
			static GateDescriptor interruptDescriptorTable[256];
			
			
			struct InterruptDescriptorTablePointer
			{
				uint16_t size;
				uint32_t base;
			} __attribute__((packed));
			
			
			static void SetInterruptDescriptorTableEntry(
						uint8_t interruptNumber,
						uint16_t codeSegmentSellectorOffset,
						void (*handler)(),
						uint8_t DescriptorPrivilegeLevel,
						uint8_t DescriptorType
			);
			
			
			Port8bitSlow picMasterComand;
			Port8bitSlow picMasterData;
			
			Port8bitSlow picSlaveComand;
			Port8bitSlow picSlaveData;
			
			
			
	  public:
			InterruptManager(GlobalDescriptorTable* gdt);
			~InterruptManager();
			
			void Activate();
			void Deactivate();
			
			static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);
			uint32_t DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp);
// Эти внешние директивы позволят нам получить доступ к адресам наших ассемблерных обработчиков прерываний ISR.			
			static void 	IgnoreInterruptRequest();
			static void     HandleInterruptRequest0x00();
			static void     HandleInterruptRequest0x01();

  };

#endif
