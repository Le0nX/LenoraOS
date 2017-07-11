#ifndef ____LENORA__DRIVERS__VGA_H
#define ____LENORA__DRIVERS__VGA_H


#include <common/types.h>
#include <hardware/port.h>
#include <drivers/driver.h>


namespace lenora{
	
	namespace drivers{
		
		class VideoGraphicsArray
		{   
			protected:
				hardware::Port8Bit miscPort;
				hardware::Port8Bit crtcIndexPort;
				hardware::Port8Bit crtcDataPort;
				hardware::Port8Bit sequenceIndexPort;
				hardware::Port8Bit sequenceDataPort;
				hardware::Port8Bit graphicsControllerIndexPort;
				hardware::Port8Bit graphicsControllerDataPort;
				hardware::Port8Bit attributeControllerIndexPort;
				hardware::Port8Bit attributeControllerReadPort;
				hardware::Port8Bit attributeControllerWritePort;
				hardware::Port8Bit attributeControllerResetPort;
				
				void WriteRegisters(lenora::common::uint8_t* registers);
				lenora::common::uint8_t* GetFrameBuffer();
				
				// 
				virtual void PutPixel(lenora::common::uint32_t x, lenora::common::uint32_t y, lenora::common::uint8_t colorIndex);
				virtual lenora::common::uint8_t GetColorIndex(lenora::common::uint8_t r, lenora::common::uint8_t g, lenora::common::uint8_t b);
				
			public:
				VideoGraphicsArray();
				virtual ~VideoGraphicsArray();
				
				virtual bool SupportsMode(lenora::common::uint32_t width, lenora::common::uint32_t height, lenora::common::uint32_t colordepth);
				virtual bool SetMode(lenora::common::uint32_t width, lenora::common::uint32_t height, lenora::common::uint32_t colordepth);
				
				// get index in table
				virtual void PutPixel(lenora::common::uint32_t x, lenora::common::uint32_t y, lenora::common::uint8_t r, lenora::common::uint8_t g, lenora::common::uint8_t b); 
		};
		
	
		

	}
}
#endif
