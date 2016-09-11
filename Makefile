COMPPARAM = -m32
NASMPARAM = -f elf

%.o: %.cpp
	g++ $(COMPPARAM) -o $@ -c $< #компилим все срр файлы с ключем -с для генерации именно обджект файла
%.o: %.s
	nasm $(NASMPARAM) -o $@ 