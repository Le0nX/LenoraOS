void printf(char *string)
{
	unsigned short* VideoMem = (unsigned short*)0xb8000;
	
	for (int i=0; str[i] != '\0'; ++i){
		VideoMem[i] = (VideoMem[i] & 0xFF00) | str[i];
	}
}

extern "C" void main(void *multiboot_struct, unsigned int MAGIC)
{
	printf("Welcome to Lenora OS...");
	
	while(1);
}
