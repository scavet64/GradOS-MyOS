#include "stddef.h" /* size_t */
#include "../include/string.h"

/**
 * Fills the first n bytes of a memory location with a particular value
 * Input - bufptr: The pointer to the address we will start filling in information
 * Input - value: The value we will use to fill in the buffer.
 * Input - size: The number of bytes in the buffer we will write
 * Output - None.
 */
void *memset(void *bufptr, int value, size_t size)
{
    unsigned char *p = bufptr;
    while (size > 0)
    {
        *p = value;
        *p++;
        size--;
    }
    return;
}