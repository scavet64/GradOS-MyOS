#include "stddef.h" /* size_t */
#include "../include/string.h"

/**
 * Compares the first n bytes of two memory locations.
 * Input - aptr: The first memory location to compare
 * Input - bptr: The second memory location to compare
 * Input - size: The amount of bytes to compare.
 * Output - Returns 0 if the first n bytes are the same.  If we hit a byte that is not the same in both we return -1 if the value in the a byte is less than the byte value in b, or 1 if the byte value in b is less than the byte value in a.
 */
int memcmp(const void *aptr, const void *bptr, size_t size)
{
    const unsigned char *p1 = aptr, *p2 = bptr;
    int cmp;
    while (size > 0)
    {
        cmp = *p1 - *p2;

        if (cmp == 0)
        {
            p1++;
            p2++;
            size--;
        }
        else if (cmp < 0)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}