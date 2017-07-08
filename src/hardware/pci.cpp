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


bool PCIcontroller::DeviceFunctions(common::uint16_t bus, common::uint16_t device)
{
	return Read(bus,device, 0, 0x0E) & (1<<7);
}


void PCIcontroller::SelectDrivers(lenora::drivers::DriverManager* drvManager)
{
	for(int bus=0; bus<8; bus++){
		for(int dev=0; dev<32; dev++){
			int numFunc = DeviceFunctions(bus, dev) ? 8 : 1;
			for(int func=0; func<numFunc; func++){
				PCIdeviceDescriptor device = GetDeviceDescriptor(bus, dev, func);
				
				if (device.vendor_id == 0x0000 || device.vendor_id == 0xFFFF)  // no devices
					continue;
					
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


