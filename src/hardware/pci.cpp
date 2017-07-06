#include <hardware/pci.h>

using namespace lenora::common;
using namespace lenora::hardware;
using namespace lenora::drivers;


PCIcontroller::PCIcontroller()
: dataPort(0xCFC),
  commandPort(0xCF8)
{
	
}

PCIcontroller::~PCIcontroller()
{}


uint32_t PCIcontroller::Read(uint16_t bus, uint16_t device, 
							 uint16_t function, uint32_t offset)
{
	uint32_t id = 
	0x1 << 31
		| ((bus & 0xFF) << 16)
		| ((device & 0x1F) << 11)
		| ((function & 0x07) << 8)
		| (offset & 0xFC);           // отрезаем последние 2bit
	
	commandPort.Write(id);
	uint32_t result = dataPort.Read();
	return result >> (8 * (offset % 4));	
}


void PCIcontroller::Write(uint16_t bus, uint16_t device, cuint16_t function, 
					                    uint32_t offset, uint32_t value)
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


void PCIcontroller::SelectDrivers(lenora::drivers::DriverManager* drvManager, )
{
	for(int bus=0; bus<8; bus++){
		for(int dev=0; dev<32; dev++){
			int numFunc = DeviceFunctions(bus, device) ? 8 : 1;
			for(int func=0; func<num; func++){
				
			}
		}
	}
	
}							 

							 

