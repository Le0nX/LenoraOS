#ifndef __LENORA__HARDWARE__PCI_H
#define __LENORA__HARDWARE__PCI_H

#include <common/types.h>
#include <hardware/port.h>
#include <drivers/driver.h>
#include <hardware/interrupt.h>

namespace lenora{
	
	namespace hardware{
		

  class PCIdeviceDescriptor
  {
	public:
		lenora::common::uint32_t portBase;
		lenora::common::uint32_t interrupt;
		
		lenora::common::uint16_t bus;
	    lenora::common::uint16_t device;
	    lenora::common::uint16_t function;
	    
	    lenora::common::uint16_t vendor_id;
	    lenora::common::uint16_t device_id;
	    
	    lenora::common::uint8_t class_id;
	    lenora::common::uint8_t subclass_id;
	    lenora::common::uint8_t interface_id;
	    
	    lenora::common::uint8_t revision;
	    
	    PCIdeviceDescriptor();
	    ~PCIdeviceDescriptor();
  };

  class PCIcontroller
  {
  	
		lenora::hardware::Port32bit dataPort;
		lenora::hardware::Port32bit commandPort;
		
  	public:
		PCIcontroller();
		~PCIcontroller();
		
		lenora::common::uint32_t Read(lenora::common::uint16_t bus, lenora::common::uint16_t device, 
							  lenora::common::uint16_t function, lenora::common::uint32_t offset);
							  
		void Write(lenora::common::uint16_t bus, lenora::common::uint16_t device, lenora::common::uint16_t function, 
										 lenora::common::uint32_t offset, lenora::common::uint32_t value);
		bool DeviceFunctions(lenora::common::uint16_t bus, lenora::common::uint16_t device);
		
		void SelectDrivers(lenora::drivers::DriverManager* drvManager);							 
		PCIdeviceDescriptor GetDeviceDescriptor(lenora::common::uint16_t bus, lenora::common::uint16_t device,
																	  lenora::common::uint16_t function);
  };
 }
}
#endif
