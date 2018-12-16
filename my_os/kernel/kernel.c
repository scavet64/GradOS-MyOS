#include "../libc/include/string.h"
#include "../libc/include/conversion.h"
// #include "libc/include/conversion.h"
// #include "libc/include/strutils.h"
// #include "libc/include/video.h"
// #include "libc/include/typedefs.h"
#include <stdint.h>
//#include "libc/include/keyboard.h"
#include "../drivers/keyboard.h"

#include "../drivers/screen.h"
#include "util.h"
#include "shell.h"

static const int VGA_WIDTH = 80;
static const int VGA_HEIGHT = 25;

void kernel_early(void)
{
}

void testScreenScroll()
{
	clearScreen();

	/* Fill up the screen */
	int i = 0;
	for (i = 0; i < 24; i++)
	{
		char str[255];
		intToAscii(i, str);
		print_at(str, 0, i);
	}

	print_at("This text forces the kernel to scroll. Row 0 will disappear. ", 60, 24);
	print("And with this text, the kernel will scroll again, and row 1 will disappear too!");
}

int main(void)
{
	printLn(" Welcome to my OS :^)");

	// // char *clone;
	// // char *input = "THIS IS SOMETHING";
	// // strcpy(clone, input);

	// print(clone);
	// print("\n");
	// print(input);

	// Show Login Prompt.
	// user will be stuck here until they login

	//print("\n> ");
	runShell();
	// Run the shell loop.

	return 1;
}