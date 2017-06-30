
#ifndef __LENORA__DRIVERS__KEYBOARD_H
#define __LENORA__DRIVERS__KEYBOARD_H



#include <common/types.h>
#include <hardware/interrupt.h>
#include <drivers/driver.h>
#include <hardware/port.h>

namespace lenora{
	
	namespace drivers{
		
		class KeyboardEventHandler
		{
		public:
			KeyboardEventHandler();
			virtual void OnKeyDown(char);
			virtual void OnKeyUp(char);
			
		};

		class KeyboardDriver : public lenora::hardware::InterruptHandler, public Driver
		{
			
			lenora::hardware::Port8bit dataport;
			lenora::hardware::Port8bit commandport;
			
			KeyboardEventHandler* handler;
		public:
			
			KeyboardDriver(lenora::hardware::InterruptManager* manager, KeyboardEventHandler* handler);
			~KeyboardDriver();
			
			virtual lenora::common::uint32_t HandleInterrupt(lenora::common::uint32_t esp);
			virtual void Activate();


		};
	}
}

#endif
