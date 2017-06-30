
#include <gdt.h>
using namespace lenora;
using namespace lenora::common;

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
	uint8_t* target = (uint8_t*)this;
	
	if(limit <=  65536){
		target[6] = 0x40;               // 16bit entry
	} else {
		if ((limit & 0xFFF) != 0xFFF)   // равны ли первые 12бит единицам
			limit = (limit >> 12)-1;
		else 
			limit = limit >> 12;
			
	  target[6] = 0xC0;
	}
	
	target[0]  =  limit & 0xFF;         // первый байт лимита
	target[1]  = (limit >> 8) & 0xFF;   // следующий байт лимита 
	target[6] |= (limit >> 16) & 0xF;   // последние 4бита лимита
	
	target[2] =  base & 0xFF;           // первый байт базы
	target[3] = (base >> 8) & 0xFF;     // второй байт базы
	target[4] = (base >> 16) & 0xFF;    // тертий байт базы
	target[7] = (base >> 24) & 0xFF;    // четвертый байт базы

	target[5] = type;

}


uint32_t GlobalDescriptorTable::SegmentDescriptor::Base()
{
	uint8_t* target = (uint8_t*)this;
	
	uint32_t result = target[7];        // Здесь все изи, если вверху было понятно.
	result = (result << 8) + target[4];
	result = (result << 8) + target[3];
	result = (result << 8) + target[2];
	return result;
}



uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit()
{
	uint8_t* target = (uint8_t*)this;
	
	uint32_t result = target[6] & 0xF;
	result = (result << 8) + target[1];
	result = (result << 8) + target[0];
	
	if ((target[6] & 0xC0) == 0xC0)
		result = (result << 12) | 0xFFF;
	
	return result;
}




