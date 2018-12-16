#include "screen.h"
#include "ports.h"
#include "../kernel/util.h"
#include "../libc/include/string.h"
#include "../libc/include/conversion.h"

/* Private functions */
int getCursorOffset();
void setCursorOffset(int offset);
int printChar(char c, int col, int row, char attr);
int getOffset(int col, int row);
int getRowOffset(int offset);
int getColOffset(int offset);

/**
 * Print a message on the specified location
 * If col, row, are negative, we will use the current offset
 */
void printAtPoint(char *message, int col, int row)
{
    // If col and row are negative, use the last location
    int offset;
    if (col >= 0 && row >= 0)
        offset = getOffset(col, row);
    else
    {
        offset = getCursorOffset();
        row = getRowOffset(offset);
        col = getColOffset(offset);
    }

    // Loop through message and print it using the printChar function
    int i = 0;
    while (message[i] != 0)
    {
        offset = printChar(message[i++], col, row, WHITE_ON_BLACK);
        /* Compute row/col for next iteration */
        row = getRowOffset(offset);
        col = getColOffset(offset);
    }
}

/**
 * Print a string to the screen at next position
 */
void print(char *message)
{
    printAtPoint(message, -1, -1);
}

/**
 * Print a string to the screen at next position and make a new line at the end
 */
void printLn(char *message)
{
    printAtPoint(message, -1, -1);
    printAtPoint("\n", -1, -1);
}

/**
 * "Prints" a backspace to the screen. This function will go to the previous cursor offset and print a ' ' character
 * to erase whatever was in that position previously.
 */
void printBackspace()
{
    int offset = getCursorOffset() - 2;
    if (offset >= 0)
    {
        int row = getRowOffset(offset);
        int col = getColOffset(offset);
        printChar(' ', col, row, WHITE_ON_BLACK);
        setCursorOffset(offset);
    }
}

/**
 * Print an integer to the screen in the next position
 */
void printInt(int num)
{
    char str[255];
    memset(str, '\0', 255);
    intToAscii(num, str);
    print(str);
}

/**
 * Print an integer to the screen in the next position and make a new line
 */
void printIntLn(int num)
{
    printInt(num);
    printLn("");
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
int printChar(char c, int col, int row, char attr)
{
    unsigned char *vidmem = (unsigned char *)VGA_MEMORY;
    if (!attr)
    {
        attr = WHITE_ON_BLACK;
    }

    int offset;
    if (col >= 0 && row >= 0)
    {
        offset = getOffset(col, row);
    }
    else
    {
        offset = getCursorOffset();
    }

    if (c == '\n')
    {
        row = getRowOffset(offset);
        offset = getOffset(0, row + 1);
    }
    else if (c == '\t')
    {
        col = getColOffset(offset);
        row = getRowOffset(offset);
        offset = getOffset(col + 5, row);
    }
    else
    {
        vidmem[offset] = c;
        vidmem[offset + 1] = attr;
        offset += 2;
    }

    // Scroll the screen if the character will be off the screen
    if (offset >= VGA_HEIGHT * VGA_WIDTH * 2)
    {
        int i;
        for (i = 1; i < VGA_HEIGHT; i++)
        {
            memcpy(getOffset(0, i) + VGA_MEMORY,
                        getOffset(0, i - 1) + VGA_MEMORY,
                        VGA_WIDTH * 2);
        }

        /* Blank last line */
        char *last_line = getOffset(0, VGA_HEIGHT - 1) + VGA_MEMORY;
        for (i = 0; i < VGA_WIDTH * 2; i++)
        {
            last_line[i] = 0;
        }

        offset -= 2 * VGA_WIDTH;
    }

    setCursorOffset(offset);
    return offset;
}

/**
 * Use the VGA ports to get the current cursor position
 * 1. Ask for high byte of the cursor offset (data 14)
 * 2. Ask for low byte (data 15)
 */
int getCursorOffset()
{
    setPortByte(REG_SCREEN_CTRL, 14);
    int offset = readPortByte(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
    setPortByte(REG_SCREEN_CTRL, 15);
    offset += readPortByte(REG_SCREEN_DATA);
    return offset * 2; /* Position * size of character cell */
}

/**
 * Set the cursor offset using the screen ports
 */
void setCursorOffset(int offset)
{
    offset /= 2;
    setPortByte(REG_SCREEN_CTRL, 14);
    setPortByte(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    setPortByte(REG_SCREEN_CTRL, 15);
    setPortByte(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

/**
 * Clear the entire screen of data
 */
void clearScreen()
{
    int screen_size = VGA_WIDTH * VGA_HEIGHT;
    int i;
    char *screen = VGA_MEMORY;

    for (i = 0; i < screen_size; i++)
    {
        screen[i * 2] = ' ';
        screen[i * 2 + 1] = WHITE_ON_BLACK;
    }
    setCursorOffset(getOffset(0, 0));
}

/**
 * Get a cursor offset based on the row and column.
 */
int getOffset(int col, int row)
{
    return 2 * (row * VGA_WIDTH + col);
}

/**
 * Get the row offset
 */
int getRowOffset(int offset)
{
    return offset / (2 * VGA_WIDTH);
}

/**
 * Get the column offset
 */
int getColOffset(int offset)
{
    return (offset - (getRowOffset(offset) * 2 * VGA_WIDTH)) / 2;
}
