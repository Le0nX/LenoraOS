#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "types.h"
#include "port.h"
#include "gdt.h"

  class InterruptManager
  {
	  protected:
			struct GateDescriptor
			{
				uint16_t    handlerAddressLowBits;
				uint16_t    gtd_codeSegmentSelector;
				uint8_t     reserved;
				uint8_t     access;
				uint16_t    handlerAddressHighBits;
				
			} __atribute__((packed));
			
			static GateDescriptor interruptDescriptorTable[256];
			
			static void SetInterruptDescriptorTableEntry(
						uint8_t interruptNumber,
						uint16_t gdt_codeSegmentSellectorOffset,
						void (*handler)(),
						uint8_t DescriptorPrivilegeLevel,
						uint8_t DescriptorType
			);
			
	  public:
			InterruptManager(GlobalDescriptorTable* gdt);
			~InterruptManager();
			
			static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);
			
			static void 	IgnoreInterruptRequest();
			static void     HandleInterruptRequest0x00();
			static void     HandleInterruptRequest0x01();

  };

#endif
