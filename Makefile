COMPPARAM = -m32
ASMPARAM = --32

%.o: %.cpp
	g++ $(COMPPARAM) -o $@ -c $< #компилим все срр файлы с ключем -с для генерации именно обджект файла
%.o: %.s
	as $(ASMPARAM) -o $@ $< #компилим на асме все файлы ($< означает сгенерить название %.s файла в формате .о)