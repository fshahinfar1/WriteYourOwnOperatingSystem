
#include "mouse.h"


int atoi(int num, char* result);
void printf(char* str);


void flip_videomem(int8_t x, int8_t y) {
    static uint16_t* VIDEOMEM = (uint16_t*)0xb8000;
    uint8_t tmp = VIDEOMEM[80*y+x] >> 8;
    tmp = ((tmp & 0x0f) << 4) | ((tmp >> 4) & 0x0f);
    VIDEOMEM[80*y+x] = (tmp << 8) | (VIDEOMEM[80*y+x] & 0x00FF);
}

MouseDriver::MouseDriver(InterruptManager* interruptManager)
: InterruptHandler(0x2C, interruptManager), 
  dataport(0x60),
  cmdport(0x64) {
 
  offset = 0;
  buttons = 0;

  cmdport.write(0xAB); // activate interrupts (ask keyboard to communicate with interrupts

  cmdport.write(0x20); // get keyboard current state
  uint8_t status = dataport.read() | 2;

  cmdport.write(0x60); // update keyboard state
  dataport.write(status);

  cmdport.write(0xD4);
  dataport.write(0xF4);
  dataport.read();
}

MouseDriver::~MouseDriver() {

}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp) {
  uint8_t status = cmdport.read();
  if(!(status & 0x20)) {
    return esp;
  }

  static int8_t x=0, y=0;

  buffer[offset] = dataport.read();
  offset = (offset + 1) % 3;

  if (offset == 0) {
    flip_videomem(x, y);
    
    x += buffer[1];
    
    if (x < 0) {
      x = 0;
    } else if (x >= 80) {
      x = 79;
    }

    y -= buffer[2];
    
    if (y < 0) {
      y = 0;
    } else if (y >= 25) {
      y = 24;
    }

    flip_videomem(x, y);
  

    for(uint8_t i = 0; i < 3; i++)
    {
      if((buffer[0] & (0x1<<i)) != (buttons & (0x1<<i)))
      {
        /*if(buttons & (0x1<<i))
            handler->OnMouseButtonReleased(i+1);
        else
            handler->OnMouseButtonPressed(i+1);
            */
      }
    } 
    buttons = buffer[0]; 
  }

  return esp;
}
