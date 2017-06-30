#ifndef __LENORA__HARDWARE__INTERRUPT_H
#define __LENORA__HARDWARE__INTERRUPT_H

#include <common/types.h>
#include <hardware/port.h>
#include <gdt.h>

namespace lenora{
	
	namespace hardware{
		
	  class InterruptManager;

	  class InterruptHandler
	  {
		  protected:
				lenora::common::uint8_t interruptNumber;
				InterruptManager* interruptManager;
				
				InterruptHandler(lenora::common::uint8_t interruptNumber, InterruptManager* interruptManager);
				~InterruptHandler();
		  public: 
				virtual lenora::common::uint32_t HandleInterrupt(lenora::common::uint32_t esp);
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
					lenora::common::uint16_t    handlerAddressLowBits;    // Младшие 16 битов адреса, куда происходи переход в случае возникновения прерывания.
					lenora::common::uint16_t    gdt_codeSegmentSellector;  // Переключатель сегмента ядра.
					lenora::common::uint8_t     reserved;                 // Это значение всегда должно быть нулевым.
					lenora::common::uint8_t     access;                   // Флаги
					lenora::common::uint16_t    handlerAddressHighBits;   // Старшие 16 битов адреса, куда происходи переход.
					
				} __attribute__((packed));
				
				static GateDescriptor interruptDescriptorTable[256];
				
				
				struct InterruptDescriptorTablePointer
				{
					lenora::common::uint16_t size;
					lenora::common::uint32_t base;
				} __attribute__((packed));
				
				
				static void SetInterruptDescriptorTableEntry(
							lenora::common::uint8_t interruptNumber,
							lenora::common::uint16_t codeSegmentSellectorOffset,
							void (*handler)(),
							lenora::common::uint8_t DescriptorPrivilegeLevel,
							lenora::common::uint8_t DescriptorType
				);
				
				
				lenora::hardware::Port8bitSlow picMasterComand;
				lenora::hardware::Port8bitSlow picMasterData;
				
				lenora::hardware::Port8bitSlow picSlaveComand;
				lenora::hardware::Port8bitSlow picSlaveData;
				
				
				
		  public:
				InterruptManager(GlobalDescriptorTable* gdt);
				~InterruptManager();
				
				void Activate();
				void Deactivate();
				
				static lenora::common::uint32_t handleInterrupt(lenora::common::uint8_t interruptNumber, lenora::common::uint32_t esp);
				lenora::common::uint32_t DoHandleInterrupt(lenora::common::uint8_t interruptNumber, lenora::common::uint32_t esp);
				// Эти внешние директивы позволят нам получить доступ к адресам наших ассемблерных обработчиков прерываний ISR.			
				static void 	IgnoreInterruptRequest();
				static void     HandleInterruptRequest0x00();
				static void     HandleInterruptRequest0x01();
				static void     HandleInterruptRequest0x0C();			

	  };
	}
}
#endif
