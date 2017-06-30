
#ifndef __LENORA__DRIVERS__MOUSE_H
#define __LENORA__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardware/interrupt.h>
#include <drivers/driver.h>
#include <hardware/port.h>

namespace lenora{
	
	namespace drivers{
		
		class MouseEventHandler
		{
			lenora::common::int8_t x, y;
		public:
			MouseEventHandler();
			virtual void OnMouseActivate();
			virtual void OnMouseDown(lenora::common::uint8_t button);
			virtual void OnMouseUp(lenora::common::uint8_t button);
			virtual void OnMouseMove(int x, int y);
		};


		class MouseDriver : public lenora::hardware::InterruptHandler, public Driver
		{
			MouseEventHandler* handler;
			
			lenora::hardware::Port8bit dataport;
			lenora::hardware::Port8bit commandport;
			
			lenora::common::uint8_t buffer[3];
			lenora::common::uint8_t offset;
			lenora::common::uint8_t buttons;
			
		public:
			
			MouseDriver(lenora::hardware::InterruptManager* manager, MouseEventHandler* handler);
			~MouseDriver();
			
			virtual lenora::common::uint32_t HandleInterrupt(lenora::common::uint32_t esp);
			virtual void Activate();

		};
	}
}

#endif
