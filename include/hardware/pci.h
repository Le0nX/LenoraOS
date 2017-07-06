#ifndef __LENORA__HARDWARE__PCI_H
#define __LENORA__HARDWARE__PCI_H

#include <common/types.h>
#include <hardware/port.h>
#include <drivers/driver.h>
#include <hardware/interrupt.h>


  class PCIdeviceDescriptor
  {
	public:
		common::uint32_t portBase;
		common::uint32_t interrupt;
		
		common::uint16_t bus;
	    common::uint16_t device;
	    common::uint16_t function;
	    
	    common::uint16_t vendor_id;
	    common::uint16_t device_id;
	    
	    common::uint8_t class_id;
	    common::uint8_t subclass_id;
	    common::uint8_t interface_id;
	    
	    common::uint8_t version;
	    
	    PCIdeviceDescriptor();
	    ~PCIdeviceDescriptor();
  };

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
		bool DeviceFunctions(common::uint16_t bus, common::uint16_t device);
		
		void SelectDrivers(lenora::drivers::DriverManager* drvManager, );							 

  };
