#include <hardware/pci.h>

using namespace lenora::common;
using namespace lenora::hardware;
using namespace lenora::drivers;

void printf(const char*);
void hexPrint(uint8_t);

PCIcontroller::PCIcontroller()
: dataPort(0xCFC),     // CONFIG_DATA
  commandPort(0xCF8)   // CONFIG_ADDRESS
{
	
}


PCIcontroller::~PCIcontroller()
{}


PCIdeviceDescriptor::PCIdeviceDescriptor()
{}

PCIdeviceDescriptor::~PCIdeviceDescriptor()
{}



uint32_t PCIcontroller::Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset)
{
	uint32_t id = 
		0x1 << 31
		| ((bus & 0xFF) << 16)
		| ((device & 0x1F) << 11)
		| ((function & 0x07) << 8)
		| (offset & 0xFC);           // отрезаем последние 2bit (Register Number)
	
	commandPort.Write(id);
	uint32_t result = dataPort.Read();
	return result >> (8 * (offset % 4));	
}


void PCIcontroller::Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset, uint32_t value)
{
	uint32_t id = 
		0x1 << 31
		| ((bus & 0xFF) << 16)
		| ((device & 0x1F) << 11)
		| ((function & 0x07) << 8)
		| (offset & 0xFC);
	commandPort.Write(id);
	dataPort.Write(value);
}


bool PCIcontroller::DeviceFunctions(uint16_t bus, uint16_t device)
{
	return Read(bus,device, 0, 0x0E) & (1<<7);
}


void PCIcontroller::SelectDrivers(DriverManager* drvManager, InterruptManager* interrupts )
{
	for(int bus=0; bus<8; bus++){
		for(int dev=0; dev<32; dev++){
			int numFunc = DeviceFunctions(bus, dev) ? 8 : 1;
			for(int func=0; func<numFunc; func++){
				PCIdeviceDescriptor device = GetDeviceDescriptor(bus, dev, func);
				
				if (device.vendor_id == 0x0000 || device.vendor_id == 0xFFFF)  // no devices
					continue;
					
				
				for(int barNum = 0; barNum < 6; barNum++){
					
					BaseAddressRegister bar = GetBaseAddressRegister(bus, dev, func, barNum);
					if (bar.address && (bar.type == InputOutput))
						device.portBase = (uint32_t)bar.address;
					
					Driver* driver = GetDriver(device, interrupts);
					if (driver != 0)
						drvManager->AddDriver(driver);
					
					
				} 
					
				
				printf("PCI BUS "); // DEBUG удали меня 
				hexPrint(bus & 0xFF);
				
				printf(" DEVICE ");
				hexPrint(dev & 0xFF);
				
				printf(" FUNCTION ");
				hexPrint(func & 0xFF);
				
				printf(" = VENDOR ");
                hexPrint((device.vendor_id & 0xFF00) >> 8);
                hexPrint(device.vendor_id & 0xFF);
                printf(", DEVICE ");
                hexPrint((device.device_id & 0xFF00) >> 8);
                hexPrint(device.device_id & 0xFF);
                				
				printf("\n");
			}
		}
	}
	
}							 

BaseAddressRegister PCIcontroller::GetBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar)
{
	BaseAddressRegister result;
	
	uint32_t header_type = Read(bus, device, function, 0x0E) & 0x7F;
	int maxBARs = 6 - (4*header_type);
	
	if (bar >= maxBARs)
		return result;
		
	uint32_t bar_val = Read(bus, device, function, 0x10 + 4*bar);
	result.type = (bar_val & 0x1) ? InputOutput : MemoryMapping; // examing the last bit == type of reg in enum
	uint32_t temp; 
	
	if (result.type == MemoryMapping){
		
		switch((bar_val >> 1) & 0x3){ // taking last 2 bits from 3 lst bits o_O PCI is sutch a PCI...
			case 0: //32 bit_mode
			case 1: //20 bit_mode
			case 2: //64 bit_mode
				break;
		}
		
		result.prefetchable = ((bar_val >> 3) & 0x1) == 0x1;
		
	} else { // InmputOutput
		
		result.address = (uint8_t*)(bar_val & ~0x3); // cancel last 2 bits (1111100)
		result.prefetchable = false;
		
	}
	
	return result;
}


Driver* PCIcontroller::GetDriver(PCIdeviceDescriptor dev, InterruptManager* interrupts)
{
	switch(dev.vendor_id){
		
		case 0x1022: //AMD
			switch(dev.device_id){
				case 0x2000: // driver for am79c973
					break;
			}
			break;
			
		case 0x8086: //Intel
			break;
	}

	switch(dev.class_id){
		
		case 0x03: // GPU
			switch(dev.subclass_id){
				case 0x00: // VGA
					break;
			
			}
		break;
	}	
	
	return 0;
}


PCIdeviceDescriptor PCIcontroller::GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function)
{
	PCIdeviceDescriptor descriptor;
	
	descriptor.bus = bus;
	descriptor.device = device;
	descriptor.function = function;
	
	descriptor.vendor_id = Read(bus, device, function, 0x00);
	descriptor.device_id = Read(bus, device, function, 0x02);

	descriptor.class_id = Read(bus, device, function, 0x0b);
	descriptor.subclass_id = Read(bus, device, function, 0x0A);
	descriptor.interface_id = Read(bus, device, function, 0x09);

	descriptor.revision = Read(bus, device, function, 0x08);
	descriptor.interrupt = Read(bus, device, function, 0x3C);
	
	return descriptor;
}


