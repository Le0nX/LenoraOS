#ifndef __LENORA__GDT_H
#define __LENORA__GDT_H

#include <common/types.h>

namespace lenora{
	
	class GlobalDescriptorTable
	{
	
	  public:
		  class SegmentDescriptor
		  {	  
		  /* *
		   * В этом классе хранится содержимое одной записи GDT.
		   * Мы используем атрибут 'packed', который указывает компилятору GCC,
		   * что в этой структуре выравнивание не выполняется.
		   */
			  private:
				  lenora::common::uint16_t LimitLow;        // Младшие 16 битов граничного значения limit.
				  lenora::common::uint16_t BaseLow;         // Младшие 16 битов адресной базы.
				  lenora::common::uint8_t  BaseMiddle;      // Следующие 8 битов адресной базы.
				  lenora::common::uint8_t  Access;          // Флаги доступа, определяющие в каком кольце можно использовать этот сегмент.
				  lenora::common::uint8_t  flags_limit_hi;  // Флаги 4бит и старшие 4бит limit
				  lenora::common::uint8_t  BaseHigh;		// Последние 8 битов адресной базы.
			  public:
				  SegmentDescriptor(lenora::common::uint32_t base, lenora::common::uint32_t limit, lenora::common::uint8_t type);
				  lenora::common::uint32_t Base();
				  lenora::common::uint32_t Limit(); 
				  
		  } __attribute__((packed));        /* отключаем выравнивание */
	  
	  SegmentDescriptor nullSegmentSelector;
	  SegmentDescriptor unusedSegmentSelector;
	  SegmentDescriptor codeSegmentSelector;	  
	  SegmentDescriptor dataSegmentSelector;	  
	  
	  GlobalDescriptorTable();
	  ~GlobalDescriptorTable();
	  
	  lenora::common::uint16_t CodeSegmentSelector(); 		//возвращает смещение дескриптора сегмента кода
	  lenora::common::uint16_t DataSegmentSelector(); 		//возвращает смещение дескриптора сегмента данных  
	    
	};
}
#endif
