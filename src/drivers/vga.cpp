
#include <drivers/vga.h>

using namespace common;
using namespace drivers;


VideoGraphicsArray::VideoGraphicsArray()
: 				miscPort(0x3C2),
				crtcIndexPort(0x3D4),
				crtcDataPort(0x3D5),
				sequenceIndexPort(0x3C4),
				sequenceDataPort(0x3C5),
				graphicsControllerIndexPort(0x3CE),
				graphicsControllerDataPort(0x3CF),
				attributeControllerIndexPort(0x3C0),
				attributeControllerReadPort(0x3C0),
				attributeControllerWritePort(0x3C1),
				attributeControllerResetPort(0x3DA),
  
{
	
}

VideoGraphicsArray::~VideoGraphicsArray()
{}

void VideoGraphicsArray::WriteRegisters(uint8_t* registers)
{
	//misc 
	miscPort.Write(*(registers++));
	
	//seuence
	for (uint8_t i=0; i<5; i++){
		sequenceIndexPort.Write(i);
		sequenceDataPort.Write(*(registers++));
	}
	
	//cathode tube controller
	
	crtcIndexPort.Write(0x03);
	crtcDataPort.Write(crtcDataPort.Read() | 0x80);   // set the first bit to one
	crtcIndexPort.Write(0x11); 
	crtcDataPort.Write(crtcDataPort.Read() & ~0x80);  // set the first bit zero
	
	registers[0x03] = registers[0x03] | 0x80;         // set the first bit to one
		
	for (uint8_t i=0; i<25; i++){
		crtcIndexPort.Write(i);
		crtcDataPort.Write(*(registers++));
	}
	
	//graphics controller
	for (uint8_t i=0; i<9; i++){
		graphicsControllerIndexPort.Write(i);
		graphicsControllerDataPort.Write(*(registers++));
	}
	
	//attribute controller
	for (uint8_t i=0; i<21; i++){
		attributeControllerResetPort.Read();
		attributeControllerIndexPort.Write(i);
		attributeControllerWritePort.Write(*(registers++));
	}		
}

uint8_t* VideoGraphicsArray::GetFrameBuffer()
{
	
}


void VideoGraphicsArray::PutPixel(uint32_t x, uint32_t y, uint8_t colorIndex)
{
	
}


uint8_t VideoGraphicsArray::GetColorIndex(uint8_t r, uint8_t g, uint8_t b)
{
	
}

				
bool VideoGraphicsArray::SupportsMode(uint32_t width, uint32_t height,  colordepth) // 320x200 8bit color
{
	return width == 320 && height == 200 && colordepth == 8;
}

bool VideoGraphicsArray::SetMode(uint32_t width, uint32_t height, uint32_t colordepth)
{
	if (!SupportsMode(width, height, colordepth))
		return false;
		
	// http://files.osdev.org/mirrors/geezer/osd/graphics/modes.c
	unsigned char g_320x200x256[] =
		{
		/* MISC */
			0x63,
		/* SEQ */
			0x03, 0x01, 0x0F, 0x00, 0x0E,
		/* CRTC */
			0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
			0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
			0xFF,
		/* GC */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
			0xFF,
		/* AC */
			0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
			0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
			0x41, 0x00, 0x0F, 0x00,	0x00
		};
	
	WriteRegisters(g_320x200x256);
	return true;
}
				
void VideoGraphicsArray::PutPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
{
	
}
