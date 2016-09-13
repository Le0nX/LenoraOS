CPPPARAM = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exeptions -fno-leading-underscore
ASMPARAM = --32
LDPARAM = -m elf_i386
objects = loader.o kernel.o

%.o: %.cpp
	g++ $(CPPPARAM) -o $@ -c $< #компилим все срр файлы с ключем -с для генерации именно обджект файла

%.o: %.s
	as $(ASMPARAM) -o $@ $< #компилим на асме все файлы ("$<" копирует все после "%.о:")
	
UNI_OS_kernel.bin: linker.ld $(objects)
	ld $(LDPARAM) $< -o $@ $(objects)

install: UNI_OS_kernel.bin
	sudo cp $< /boot/UNI_OS_kernel.bin