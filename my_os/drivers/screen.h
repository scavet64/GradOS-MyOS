#define VGA_HEIGHT 25
#define VGA_WIDTH 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

static char * const VGA_MEMORY = (char *)0xb8000;

void clearScreen();
void printAtPoint(char *message, int col, int row);
void print(char *message);
void printBackspace();
void printInt(int num);
void printLn(char *message);
void printIntLn(int num);