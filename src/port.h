#ifndef __PORT_H__
#define __PORT_H__

#include "types.h"



class Port {
  protected:
    Port(uint16_t portnumber);
    ~Port();
    uint16_t portnumber;

};

class Port8Bit: public Port {
  public:
    Port8Bit(uint16_t portnumber);
    ~Port8Bit();
    
    virtual uint8_t read();
    virtual void write(uint8_t data);
};

class Port8BitSlow: public Port8Bit {
  public:
    Port8BitSlow(uint16_t portnumber);
    ~Port8BitSlow();
    
    virtual void write(uint8_t data);
};


class Port16Bit: public Port {
  public:
    Port16Bit(uint16_t portnumber);
    ~Port16Bit();
    
    virtual uint16_t read();
    virtual void write(uint16_t data);
};


class Port32Bit: public Port {
  public:
    Port32Bit(uint16_t portnumber);
    ~Port32Bit();
    
    virtual uint32_t read();
    virtual void write(uint32_t data);
};

#endif
