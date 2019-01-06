# WriteYourOwnOperatingSystem
Trying to implement operating system described at http://www.wyoos.org


# Issues while following the video     
- in 3rd video after implementing GDT I had to add fno-stack-protector to my compilation flags.     
- in 4th video while implementing the port class I had problem with accessing the <i>portnumber</i> property from <i>Port8BitSlow</i> class. <i>portnumber</i> is defined as protected property of <i>Port</i> abstract class. the problem was that I didn't used public inheritance when inheriting from <i>Port</i> class. [cpp inheritance](https://www.tutorialspoint.com/cplusplus/cpp_inheritance.htm)     

