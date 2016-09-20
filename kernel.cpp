void printf(char* str){
	unsigned short* VideoMemory = (unsigned short*)0xb8000; //указатель на начальную точку в памяти, где первый байт содержит цвет ,а второй наш символ. 
	for (int i =0; str[i] != '\0'; ++i) //пока не встретим нулевой байт(перевод строки)
		VideoMemory[i] = (VideoMemory [i] & 0xFF00) | str[i]; //побитовое умножение и сложение, чтобы не перетирать 8бит цвета.
}

extern "C" void kernelMain(void* multiboot_structure, unsigned int magicnumber){
    printf("Welcome to UNI OS v.0.1\n Copyright by Nefedov & Serkov. Moscow 2016.");
    
    while(1); //inf loop. Ядро же не может остановиться, лол
    
    }