#include "port.h"


Port::Port(uint16_t portnumber) {
  this->portnumber = portnumber;
}

Port::~Port() {
  // not implemented
}

/*
 * 8 bit port
 * */

Port8Bit::Port8Bit(uint16_t portnumber)
  : Port(portnumber) {
    // fallback on parent constructor
}

Port8Bit::~Port8Bit() {
  // not implemented
}
    
uint8_t Port8Bit::read() {
  uint8_t result;
  __asm__ volatile ("inb %1, %0" : "=a" (result): "Nd" (portnumber));
  return result;
}

void Port8Bit::write(uint8_t data) {
  __asm__ volatile ("outb %0, %1" : : "a" (data), "Nd" (portnumber));
}

/*
 * 8 bit slow port
 * */


Port8BitSlow::Port8BitSlow(uint16_t portnumber):
  Port8Bit(portnumber) {
    // fallback on parent constructor
}

Port8BitSlow::~Port8BitSlow() {
  // not implemented
}

void Port8BitSlow::write(uint8_t data) {
  __asm__ volatile (
      "outb %0, %1  \n\t"
      "jmp 1f       \n\t"
      "1: jmp 1f    \n\t"
      "1:           \n\t" 
      : : "a" (data), "Nd" (portnumber));
}


/*
 * port 16 bit
 * */

Port16Bit::Port16Bit(uint16_t portnumber)
  :Port(portnumber) {
    // fallback on parent constructor
}

Port16Bit::~Port16Bit() {
  // not implemented
}

uint16_t Port16Bit::read() {
  uint16_t result;
  __asm__ volatile ("inw %1, %0" : "=a" (result): "Nd" (portnumber));
  return result;
}

void Port16Bit::write(uint16_t data) {
  __asm__ volatile ("outw %0, %1" : : "a" (data), "Nd" (portnumber));
}


/*
 * port 32 bit
 * */


Port32Bit::Port32Bit(uint16_t portnumber)
  :Port(portnumber) {
    // fallback on parent constructor
}

Port32Bit::~Port32Bit() {
  // not implemented
}

uint32_t Port32Bit::read() {
  uint32_t result;
  __asm__ volatile ("inl %1, %0" : "=a" (result): "Nd" (portnumber));
  return result;
}

void Port32Bit::write(uint32_t data) {
  __asm__ volatile ("outl %0, %1" : : "a" (data), "Nd" (portnumber));
}

