#include "../libc/include/io.h"
#include "../libc/include/string.h"

/* Declaration of private functions */
static const int SCREEN_COLS = 80;
static const int SCREEN_ROWS = 25;
static char *const VGA_MEMORY = (char *)0xb8000;

#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/

/**
 * Print a message on the specified location
 * If col, row, are negative, we will use the current offset
 */
void print_at(char *message, int col, int row) {
    /* Set cursor if col/row are negative */
    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    /* Loop through message and print it */
    int i = 0;
    while (message[i] != 0) {
        offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
        /* Compute row/col for next iteration */
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void print(char *message) {
    print_at(message, -1, -1);
}

void print_backspace() {
    int offset = get_cursor_offset()-2;
    int row = get_offset_row(offset);
    int col = get_offset_col(offset);
    print_char(0x08, col, row, WHITE_ON_BLACK);
}

void clear_screen()
{
    int screen_size = SCREEN_COLS * SCREEN_ROWS;
    int maxCounter = screen_size * 2;
    int i;

    for (i = 0; i < maxCounter; i = i + 2)
    {
        VGA_MEMORY[i] = ' ';
    }
    set_cursor_offset(get_offset(0, 0));
}

/**********************************************************
 * Private kernel functions                               *
 **********************************************************/

/**
 * Innermost print function for our kernel, directly accesses the video memory 
 *
 * If 'col' and 'row' are negative, we will print at current cursor location
 * If 'attr' is zero it will use 'white on black' as default
 * Returns the offset of the next character
 * Sets the video cursor to the returned offset
 */
int print_char(char c, int col, int row, char attr) {
    unsigned char *vidmem = (unsigned char*) VGA_MEMORY;
    if (!attr) attr = WHITE_ON_BLACK;

    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= SCREEN_COLS || row >= SCREEN_ROWS) {
        vidmem[2*(SCREEN_COLS)*(SCREEN_ROWS)-2] = 'E';
        vidmem[2*(SCREEN_COLS)*(SCREEN_ROWS)-1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    if (c == '\n') {
        row = get_offset_row(offset);
        offset = get_offset(0, row+1);
    } else {
        vidmem[offset] = c;
        vidmem[offset+1] = attr;
        offset += 2;
    }

    /* Check if the offset is over screen size and scroll */
    if (offset >= SCREEN_ROWS * SCREEN_COLS * 2) {
        int i;
        for (i = 1; i < SCREEN_ROWS; i++) 
            memcmp(get_offset(0, i) + VGA_MEMORY,
                        get_offset(0, i-1) + VGA_MEMORY,
                        SCREEN_COLS * 2);

        /* Blank last line */
        char *last_line = get_offset(0, SCREEN_ROWS-1) + VGA_MEMORY;
        for (i = 0; i < SCREEN_COLS * 2; i++) last_line[i] = 0;

        offset -= 2 * SCREEN_COLS;
    }

    set_cursor_offset(offset);
    return offset;
}

int get_cursor_offset() {
    /* Use the VGA ports to get the current cursor position
     * 1. Ask for high byte of the cursor offset (data 14)
     * 2. Ask for low byte (data 15)
     */
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2; /* Position * size of character cell */
}

void set_cursor_offset(int offset) {
    /* Similar to get_cursor_offset, but instead of reading we write data */
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

int get_offset(int col, int row) { return 2 * (row * SCREEN_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * SCREEN_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*SCREEN_COLS))/2; }