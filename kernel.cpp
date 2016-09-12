void printf(char* str){
	(unsigned short*)VideoMemory = (unsigned short*)0xb8000;
	for (int i =0; str[i] != '\0'; ++i)
		VideoMemory[i] = str[i];
}

void kernelMain(void* multiboot_structure, unsigned int magicnumber){
    printf("Welcome to UNI OS v.0.1\n Copyright by Nefedov & Serkov. Moscow 2016.");
    
    while(1); //inf loop. Ядро же не может остановиться, лол
    
    }