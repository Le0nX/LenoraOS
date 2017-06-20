
CC=g++
AS=as

CFLAGS=-m32 -Wall -fno-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -c
ASFLAGS=--32
LDFLAGS=-melf_i386

objects = loader.o kernel.o

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@ 
	
%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<
	
LenoraKernel: linker.ld $(objects)
	ld $(LDFLAGS) -T $< -o $@ $(objects)
	
install: LenoraKernel.bin
	sudo cp /boot/LenoraKernel.bin
	
	
