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
				lenora::hardware::Port8bit miscPort;
				lenora::hardware::Port8bit crtcIndexPort;
				lenora::hardware::Port8bit crtcDataPort;
				lenora::hardware::Port8bit sequenceIndexPort;
				lenora::hardware::Port8bit sequenceDataPort;
				lenora::hardware::Port8bit graphicsControllerIndexPort;
				lenora::hardware::Port8bit graphicsControllerDataPort;
				lenora::hardware::Port8bit attributeControllerIndexPort;
				lenora::hardware::Port8bit attributeControllerReadPort;
				lenora::hardware::Port8bit attributeControllerWritePort;
				lenora::hardware::Port8bit attributeControllerResetPort;
				
				void WriteRegisters(lenora::common::uint8_t* registers);
				lenora::common::uint8_t* GetFrameBufferSegment();
				
				
				/*virtual void PutPixel(lenora::common::uint32_t x, 
									  lenora::common::uint32_t y, 
									  lenora::common::uint8_t colorIndex);
				*/					  
									  
				virtual lenora::common::uint8_t GetColorIndex(lenora::common::uint8_t r, 
															  lenora::common::uint8_t g, 
															  lenora::common::uint8_t b);
				
			public:
				VideoGraphicsArray();
				~VideoGraphicsArray();
				
				virtual bool SupportsMode(lenora::common::uint32_t width, 
										  lenora::common::uint32_t height, 
										  lenora::common::uint32_t colordepth);
										  
										  
				virtual bool SetMode(lenora::common::uint32_t width, 
								     lenora::common::uint32_t height, 
									 lenora::common::uint32_t colordepth);
				
				// get index in table
				virtual void PutPixel(lenora::common::uint32_t x, 
									  lenora::common::uint32_t y, 
									  lenora::common::uint8_t r, 
									  lenora::common::uint8_t g, 
									  lenora::common::uint8_t b);
									  
				virtual void PutPixel(lenora::common::uint32_t x, 
									  lenora::common::uint32_t y, 
									  lenora::common::uint8_t colorIndex);									   
		};
		
	
		

	}
}
#endif
