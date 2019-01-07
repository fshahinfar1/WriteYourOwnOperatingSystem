#include "interrupts.h"


void printf(char* str);
int atoi(int num, char* result);

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];


void InterruptManager::SetInterruptDescriptorTableEntry (
  uint8_t interruptNumber,
  uint16_t codeSegmentSelectortOffset,
  void (*handler)(),
  uint8_t DescriptortPrivilegeLevel,
  uint8_t DescriptorType
) {
  const uint8_t IDT_DESC_PRESENT = 0x80;

  interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
  interruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((uint32_t)handler) >> 16) & 0xFFFF;
  interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectortOffset;
  interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | ((DescriptortPrivilegeLevel & 3) << 5) | DescriptorType; 
  interruptDescriptorTable[interruptNumber].reserved = 0;
}


InterruptManager::InterruptManager(GlobalDescriptorTable* gdt) 
: picMasterCommand(0x20),
  picMasterData(0x21),
  picSlaveCommand(0xA0),
  picSlaveData(0xA1) {

  uint16_t CodeSegment = gdt->CodeSegmentSelector();
  const uint8_t IDT_INTERRUPT_GATE = 0x0E;

  for (uint16_t i = 0; i < 256; i++) {
    SetInterruptDescriptorTableEntry (
          i,
          CodeSegment,
          &IgnoreInterruptRequest,
          0,
          IDT_INTERRUPT_GATE
        );
  }

  SetInterruptDescriptorTableEntry ((uint8_t)0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
  SetInterruptDescriptorTableEntry ((uint8_t)0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
  

  picMasterCommand.write(0x11);
  picSlaveCommand.write(0x11);
  
  picMasterData.write(0x20);
  picSlaveData.write(0x28);


  picMasterData.write(0x04);
  picSlaveData.write(0x02);
  
  
  picMasterData.write(0x01);
  picSlaveData.write(0x01);
  
  picMasterData.write(0x00);
  picSlaveData.write(0x00);
  
  InterruptDescriptorTablePointer idt;
  idt.size = 256 * sizeof(GateDescriptor) - 1;
  idt.base = (uint32_t)interruptDescriptorTable;
  asm volatile("lidt %0": : "m" (idt));
}


InterruptManager::~InterruptManager() {

} 


void InterruptManager::Activate() {
  printf("Active interupts\n");
  asm ("sti");
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp) {
  printf("INTERRUPT");
  
  return esp;
}
      
