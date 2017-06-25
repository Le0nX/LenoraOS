
#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "types.h"
#include "interrupt.h"
#include "port.h"


class KeyboardDriver : public InterruptHandler
{
	
    Port8bit dataport;
    Port8bit commandport;

public:
    
	KeyboardDriver(InterruptManager* manager);
	~KeyboardDriver();
	
	virtual uint32_t HandleInterrupt(uint32_t esp);

};


#endif
