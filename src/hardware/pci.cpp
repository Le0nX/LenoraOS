#include <hardware/pci.h>

using namespace lenora::common;
using namespace lenora::hardware;

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
}
