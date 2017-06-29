
#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "types.h"
#include "interrupt.h"
#include "driver.h"
#include "port.h"


class KeyboardDriver : public InterruptHandler, public Driver
{
	
    Port8bit dataport;
    Port8bit commandport;

public:
    
	KeyboardDriver(InterruptManager* manager);
	~KeyboardDriver();
	
	virtual uint32_t HandleInterrupt(uint32_t esp);
	virtual void Activate();


};


#endif
