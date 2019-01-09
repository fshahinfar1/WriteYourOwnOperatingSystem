
#include "keyboard.h"


int atoi(int num, char* result);
void printf(char* str);

KeyboarDriver::KeyboarDriver(InterruptManager* interruptManager)
: InterruptHandler(0x21, interruptManager), 
  dataport(0x60),
  cmdport(0x64) {
  
  while (cmdport.read() & 0x01) {
    printf("HANGED\n");
    dataport.read();
  }
  cmdport.write(0xAE); // activate interrupts (ask keyboard to communicate with interrupts

  cmdport.write(0x20); // get keyboard current state
  uint8_t status = (dataport.read() | 1) & ~(1<<5);

  cmdport.write(0x60); // update keyboard state
  dataport.write(status);

  dataport.write(0xF4);
  printf("End of keyboard constructor\n");
}

KeyboarDriver::~KeyboarDriver() {

}

uint32_t KeyboarDriver::HandleInterrupt(uint32_t esp) {
  // todo: fix atoi
  // todo: add string concatenation function
  // todo: update printf so can be formated

  printf("handle key board interrupt\n");
  uint8_t key = dataport.read();
  char tmp[8];
  atoi(key, tmp);
  printf("Key: ");
  printf(tmp);
  printf("\n");

  return esp;
}
