
enum vga_color{
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

void printf(char *str)
{
	unsigned short* VideoMem = (unsigned short*)0xb8000;
	
	for (int i=0; str[i] != '\0'; ++i){
		VideoMem[i] = (VideoMem[i] & 0xFF00) | str[i];
	}
}


typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
	for (constructor* i = &start_ctors; i != &end_ctors; i++)
		(*i)();
}

extern "C" void kmain(void *multiboot_struct, unsigned int MAGIC)
{
	printf("Welcome to Lenora OS...");
	
	while(1);
}
