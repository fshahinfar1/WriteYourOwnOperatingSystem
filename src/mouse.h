#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "types.h"
#include "interrupts.h"
#include "port.h"


class MouseDriver: public InterruptHandler {
  Port8Bit dataport;
  Port8Bit cmdport;

  uint8_t buffer[3];
  uint8_t offset;
  uint8_t buttons;
  public:
    MouseDriver(InterruptManager* interruptManager);
    ~MouseDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif
