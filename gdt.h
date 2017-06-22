#ifndef __GDT_H
#define __GDT_H

#include "types.h"

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
				  uint16_t LimitLow;        // Младшие 16 битов граничного значения limit.
				  uint16_t BaseLow;         // Младшие 16 битов адресной базы.
				  uint8_t  BaseMiddle;      // Следующие 8 битов адресной базы.
				  uint8_t  Access;          // Флаги доступа, определяющие в каком кольце можно использовать этот сегмент.
				  uint8_t  flags_limit_hi;  // Флаги 4бит и старшие 4бит limit
				  uint8_t  BaseHigh;		// Последние 8 битов адресной базы.
			  public:
				  SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
				  uint32_t Base();
				  uint32_t Limit(); 
				  
		  } __attribute__((packed));        /* отключаем выравнивание */
	  
	  SegmentDescriptor nullSegmentSelector;
	  SegmentDescriptor unusedSegmentSelector;
	  SegmentDescriptor codeSegmentSelector;	  
	  SegmentDescriptor dataSegmentSelector;	  
	  
	  GlobalDescriptorTable();
	  ~GlobalDescriptorTable();
	  
	  uint16_t CodeSegmentSelector(); 		//возвращает смещение дескриптора сегмента кода
	  uint16_t DataSegmentSelector(); 		//возвращает смещение дескриптора сегмента данных  
	    
	};
	

#endif
