#include "shell.h"

void kernel_early(void)
{
}

int main(void)
{
	runShell();
	return 1;
}