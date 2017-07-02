#ifndef __LENORA__HARDWARE__PCI_H
#define __LENORA__HARDWARE__PCI_H

#include <common/types.h>
#include <hardware/port.h>
#include <hardware/interrupt.h>

  class PCIcontroller{
  	
		lenora::hardware::Port32bit dataPort;
		lenora::hardware::Port32bit commandPort;
		
  	public:
		PCIcontroller();
		~PCIcontroller();
		
		common::uint32_t Read(common::uint16_t bus, common::uint16_t device, 
							  common::uint16_t function, common::uint32_t offset);
							  
		void Write(common::uint16_t bus, common::uint16_t device, common::uint16_t function, 
										 common::uint32_t offset, common::uint32_t value);

  };
