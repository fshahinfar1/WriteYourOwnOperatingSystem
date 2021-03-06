
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

  uint8_t key = dataport.read();
  
  if(key < 0x80)
    {
        switch(key)
        {
            case 0x02: printf("1"); break;
            case 0x03: printf("2"); break;
            case 0x04: printf("3"); break;
            case 0x05: printf("4"); break;
            case 0x06: printf("5"); break;
            case 0x07: printf("6"); break;
            case 0x08: printf("7"); break;
            case 0x09: printf("8"); break;
            case 0x0A: printf("9"); break;
            case 0x0B: printf("0"); break;

            case 0x10: printf("q"); break;
            case 0x11: printf("w"); break;
            case 0x12: printf("e"); break;
            case 0x13: printf("r"); break;
            case 0x14: printf("t"); break;
            case 0x15: printf("y"); break;
            case 0x16: printf("u"); break;
            case 0x17: printf("i"); break;
            case 0x18: printf("o"); break;
            case 0x19: printf("p"); break;

            case 0x1E: printf("a"); break;
            case 0x1F: printf("s"); break;
            case 0x20: printf("d"); break;
            case 0x21: printf("f"); break;
            case 0x22: printf("g"); break;
            case 0x23: printf("h"); break;
            case 0x24: printf("j"); break;
            case 0x25: printf("k"); break;
            case 0x26: printf("l"); break;

            case 0x2C: printf("z"); break;
            case 0x2D: printf("x"); break;
            case 0x2E: printf("c"); break;
            case 0x2F: printf("v"); break;
            case 0x30: printf("b"); break;
            case 0x31: printf("n"); break;
            case 0x32: printf("m"); break;
            case 0x33: printf(","); break;
            case 0x34: printf("."); break;
            case 0x35: printf("-"); break;

            case 0x1C: printf("\n"); break;
            case 0x39: printf(" "); break;

            default:
            {
                char* foo = "KEYBOARD 0x00 ";
                char* hex = "0123456789ABCDEF";
                foo[11] = hex[(key >> 4) & 0xF];
                foo[12] = hex[key & 0xF];
                printf(foo);
                break;
            }
        }
    }
  
  /* char tmp[8];
  atoi(key, tmp);
  printf("Key: ");
  printf(tmp);
  printf("\n"); */

  return esp;
}
