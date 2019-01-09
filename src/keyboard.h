#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "types.h"
#include "interrupts.h"
#include "port.h"


class KeyboarDriver: public InterruptHandler {
  Port8Bit dataport;
  Port8Bit cmdport;

  public:
    KeyboarDriver(InterruptManager* interruptManager);
    ~KeyboarDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif
