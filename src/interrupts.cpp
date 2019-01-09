#include "interrupts.h"

// just functions
void printf(char* str);
int atoi(int num, char* result);


// InterruptHandler

InterruptHandler::InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager) {
  this->interruptManager = interruptManager;
  interruptManager->handlers[interruptNumber] = this;
}

InterruptHandler::~InterruptHandler() {
  if (interruptManager->handlers[interruptNumber] == this) {
    interruptManager->handlers[interruptNumber] = 0;
  }
}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp) {
  return esp;
}


// InterruptManager
InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256]; // IDT

InterruptManager* InterruptManager::ActiveInterruptManager = 0; // static pointer to interrupt manager object

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
    handlers[i] = 0;
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
  SetInterruptDescriptorTableEntry ((uint8_t)0x2C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);

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
  if (ActiveInterruptManager != 0) {
    // if there is an active interrupt manager
    ActiveInterruptManager->Deactivate();
  }
  ActiveInterruptManager = this;
  asm ("sti");
  printf("Active InterruptManager\n");
}

void InterruptManager::Deactivate() {
  if (ActiveInterruptManager == this) {
    ActiveInterruptManager = 0;
    asm ("cli");
  }
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp) {
  
  if (ActiveInterruptManager != 0) {
    return ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);
  }
  
  return esp;
}
      
uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp) {
  if(handlers[interruptNumber] != 0) {
    // printf("interrupt has a handler!\n");
    esp = handlers[interruptNumber]->HandleInterrupt(esp);
  } else if (interruptNumber != 0x20) {
    // if not a clock interrupt
    //
    // todo: print the unhandled interrupt number
    // todo: add function to concatenate to strings
    printf("Unhandled INTERRUPT!");
  }

  if (0x20 <= interruptNumber && interruptNumber <= 0x30) {
    // answer interrupt if it is a hardware iterrupt
    picMasterCommand.write(0x20);
    if  (0x28 <= interruptNumber <= 0x30) {
      // if interrupt is from slave pic then answer to him too
      picSlaveCommand.write(0x20);
    }
  }

  return esp;  
}
