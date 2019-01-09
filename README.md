# WriteYourOwnOperatingSystem
Trying to implement operating system described at http://www.wyoos.org


# Issues while following the video     
- in 3rd video after implementing GDT I had to add fno-stack-protector to my compilation flags.     
- in 4th video while implementing the port class I had problem with accessing the <i>portnumber</i> property from <i>Port8BitSlow</i> class. <i>portnumber</i> is defined as protected property of <i>Port</i> abstract class. the problem was that I didn't used public inheritance when inheriting from <i>Port</i> class. [cpp inheritance](https://www.tutorialspoint.com/cplusplus/cpp_inheritance.htm)     
- in 5th video I had an overflow issue when setting 256 enteries of <b>IDT</b>. I used `uint8_t` for counting until 256 but it would go back to zero when calculating 255 + 1. as a result my kernel would stuck in <b>Interrupt Manager</b>'s constructor.     
- in 5th video I had an issue with <b>GDT</b>. I had put gdt address in incorrect order when trying to use `LGDT` assembly instruction.     
- in 5th video I had an issue with a structure which hold a pointer data to <b>IDT</b>. the bits order for sending data using `LIDT` instruction were wrong.
- in 5th video I had to implement something like atio function (converting interger to string) for debugging.    
- in 6th video I had a problem when using InterruptManager in InterruptHandler. InterruptManager should have been pre-declared.
- in 6th video I had an operator precedence issue `~1<<5` was not what I wanted `~(1<<5)`. In the video `~0x10` was used.
