# dependencies: g++, binutils, libc6-dev-i386

GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -fno-stack-protector
ASPARAMS = --32
LDPARAMS = -melf_i386


objects = loader.o gdt.o kernel.o


run: mykernel.iso
	# virtual box should be installed and a vm
	# with name 'my os' should be setuped
	VBoxManage controlvm "my os" poweroff && sleep 1 || true
	(killall VirtualBox && sleep 1) || true
	VBoxManage startvm 'my os' &


mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/$< 
	echo 'set timeout=0' 					>> iso/boot/grub/grub.cfg
	echo 'set default=0' 					>> iso/boot/grub/grub.cfg
	echo 'menuentry "my os" {' 				>> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin' 	>> iso/boot/grub/grub.cfg
	echo '  boot'		 					>> iso/boot/grub/grub.cfg
	echo '}' 								>> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso
	cp mykernel.iso ~/"VirtualBox VMs"/"my os"/
	

%.o: %.cpp
	gcc $(GCCPARAMS) -c -o $@ $<


%.o: %.s
	as $< $(ASPARAMS) -o $@ 


mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)


# install: mykernel.bin
# 	sudo cp $< /boot/mykernel.bin

clean: 
	rm $(objects) mykernel.bin mykernel.iso
	
	# check if iso folder is left out
	if [-e ./iso]
	then 
		rm -rf ./iso
	fi
    

