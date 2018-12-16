#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

/* Public kernel API */
void clearScreen();
void print_at(char *message, int col, int row);
void print(char *message);
void print_backspace();
void printInt(int num);
void printLn(char *message);
void printIntLn(int num);