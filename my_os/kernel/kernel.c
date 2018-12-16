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

int main(void)
{
	runShell();
	return 1;
}