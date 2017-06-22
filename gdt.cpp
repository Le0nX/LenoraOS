
#include "gdt.h"

GlobalDescriptorTable::GlobalDescriptorTable()
: nullSegmentSelector(0,0,0),
  unusedSegmentSelector(0,0,0),
  codeSegmentSelector(0,64*1024*1024, 0x9A),
  dataSegmentSelector(0,64*1024*1024, 0x92)
{
	uint32_t i[2];
	
	i[1] = (uint32_t)this;        // Грязный хак. передаем в первые 4байта адрес самой таблицы
	i[0] = (sizeof(GlobalDescriptorTable)-1) << 16; // Сдвигаем ,чтобы было 6 байт, а не 8
	
	asm volatile("lgdt (%0)": :"p"(((uint8_t *) i)+2)); //см вики по gcc inline assm
}


GlobalDescriptorTable::~GlobalDescriptorTable()
{}

uint16_t GlobalDescriptorTable::DataSegmentSelector()
{
	return (uint8_t*)&dataSegmentSelector - (uint8_t*)this;
}

uint16_t GlobalDescriptorTable::CodeSegmentSelector()
{
	return (uint8_t*)&codeSegmentSelector - (uint8_t*)this;
}


GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type)
{
	
}





