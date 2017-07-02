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
	
}


void PCIcontroller::Write(uint16_t bus, uint16_t device, cuint16_t function, 
					                    uint32_t offset, uint32_t value)
{
	
}
