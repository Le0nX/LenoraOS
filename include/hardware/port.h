#ifndef __LENORA__HARDWARE__PORT_H
#define __LENORA__HARDWARE__PORT_H

#include <common/types.h>

namespace lenora{
	
	namespace hardware{
		
		  class Port
		  {
		  protected:	  
			 lenora::common::uint16_t portnumber;
			 Port(lenora::common::uint16_t portnumber);
			 ~Port();  
		  };
		  
		  class Port8bit : public Port
		  {
		  public:	  
			 Port8bit(lenora::common::uint16_t portnumber);
			 ~Port8bit();  
			 
			 virtual void Write(lenora::common::uint8_t data);
			 virtual lenora::common::uint8_t Read();
		  };     
		  
		  
		  class Port8bitSlow : public Port8bit
		  {
		  public:	  
			 Port8bitSlow(lenora::common::uint16_t portnumber);
			 ~Port8bitSlow();
			   
			 virtual void Write(lenora::common::uint8_t data);
		  };   

		  class Port16bit : public Port
		  {
		  public:	  
			 Port16bit(lenora::common::uint16_t portnumber);
			 ~Port16bit();
			   
			 virtual void Write(lenora::common::uint16_t data);
			 virtual lenora::common::uint16_t Read();
		  };

		  class Port32bit : public Port
		  {
		  public:	  
			 Port32bit(lenora::common::uint16_t portnumber);
			 ~Port32bit();
			   
			 virtual void Write(lenora::common::uint32_t data);
			 virtual lenora::common::uint32_t Read();
		  };
	}
}

#endif
