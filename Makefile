
CC=g++
AS=as

CFLAGS=-m32 -Wall -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -c
ASFLAGS=--32
LDFLAGS=-melf_i386

objects = loader.o gdt.o driver.o port.o interruptstub.o interrupt.o keyboard.o mouse.o kernel.o

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@ 
	
%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<
	
LenoraKernel.bin: linker.ld $(objects)
	ld $(LDFLAGS) -T $< -o $@ $(objects)
	
install: LenoraKernel.bin
	sudo cp $< /boot/LenoraKernel.bin
	
burniso: LenoraKernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	touch iso/boot/grub/grub.cfg
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "Lenora OS" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/LenoraKernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub2-mkrescue --output=Lenora.iso iso
	rm -rf iso
	
run: burniso
	(killall VirtualBox && sleep 1) || true
	virtualbox --startvm "Lenora" &
	
.PHONY: clean
clean:
	rm -f $(objects) LenoraKernel.bin Lenora.iso
