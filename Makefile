
CC=g++
AS=as

CFLAGS=-m32 -Iinclude -Wall -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -c
ASFLAGS=--32
LDFLAGS=-melf_i386

objects = obj/loader.o \
		  obj/gdt.o \
		  obj/drivers/driver.o \
		  obj/hardware/port.o \
		  obj/hardware/interruptstub.o \
		  obj/hardware/interrupt.o \
		  obj/hardware/pci.o \
		  obj/drivers/keyboard.o \
		  obj/drivers/mouse.o \
		  obj/drivers/vga.o \
		  obj/kernel.o

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@ 
	
obj/%.o: src/%.s
	mkdir -p $(@D)
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
	
mac_send: burniso
	scp Lenora.iso le0nx@deniss-mbp:wrk/OS_dev/iso
	
run: burniso
	(killall VirtualBox && sleep 1) || true
	virtualbox --startvm "Lenora" &
	
.PHONY: clean
clean:
	rm -rf obj LenoraKernel.bin Lenora.iso
