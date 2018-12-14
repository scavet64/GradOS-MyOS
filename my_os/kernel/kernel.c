// #include "libc/include/string.h"
// #include "libc/include/conversion.h"
// #include "libc/include/strutils.h"
// #include "libc/include/video.h"
// #include "libc/include/typedefs.h"
// #include <stdint.h>
//#include "libc/include/keyboard.h"

#include "../drivers/screen.h"
#include "util.h"

static char *const VGA_MEMORY = (char *)0xb8000;
static const int VGA_WIDTH = 80;
static const int VGA_HEIGHT = 25;

void kernel_early(void)
{
}

// inline char getch(void)
// {
// 	uint16_t inchar;

// 	__asm__ __volatile__("int $0x16\n\t"
// 						 : "=a"(inchar)
// 						 : "0"(0x0));

// 	return ((char)inchar);
// }

void printHelloWorld()
{
	// char *str = "       str          ";
	// char *str2 = trim(str);
	// char *cat = strcat(str2, " cat1       ");
	// cat = trim(cat);
	// cat = strcat(cat, "        cat2");
	// char *trimed = trim(cat);

	// unsigned int i = 0;
	// unsigned int j = 0;

	// size_t len = strlen("aaaaa");
	// char *length;
	// ctos(length, len + '0');

	// while (trimed[i] != '\0')
	// {
	// 	VGA_MEMORY[j] = trimed[i];
	// 	VGA_MEMORY[j + 1] = 0x07;
	// 	i++;
	// 	j = j + 2;
	// }

    clear_screen();

    /* Fill up the screen */
    int i = 0;
    for (i = 0; i < 24; i++) {
        char str[255];
        int_to_ascii(i, str);
        kprint_at(str, 0, i);
    }

    kprint_at("This text forces the kernel to scroll. Row 0 will disappear. ", 60, 24);
    kprint("And with this text, the kernel will scroll again, and row 1 will disappear too!");
}

// void testUserInput()
// {
// 	char fromkey = getch();
// 	char *out;
// 	print(ctos(out, fromkey));
// }

// void testScreenScroll()
// {
// 	clear_screen();

// 	/* Fill up the screen */
// 	int i = 0;
// 	for (i = 0; i < 24; i++)
// 	{
// 		char str[255];
// 		int_to_ascii(i, str);
// 		print_at(str, 0, i);
// 	}

// 	print_at("This text forces the kernel to scroll. Row 0 will disappear. ", 60, 24);
// 	print("And with this text, the kernel will scroll again, and row 1 will disappear too!");
// }

int main(void)
{
	////////// Testing //////////

	//testUserInput();

	printHelloWorld();

	//testScreenScroll();

	/////////////////////////////

	// Show Login Prompt.
	// user will be stuck here until they login

	// Run the shell loop.

	return 1;
}
