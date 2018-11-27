#include <stddef.h> /* for size_t */
#include "../include/string.h"

/**
 * Concatenate two strings together such that strcat(d,s) will give us the string d followed by the string s
 * Input - d: The leftmost string in our concatenated string
 * Input - s: The rightmost string in our concatenated string
 * Input - n: the number of characters to concatenate
 * Output - The address of the two strings concatenated together
 */
char* strncat(char *dest, const char *src, size_t n)
{
    size_t dest_len = strlen(dest);
    size_t i;

    for (i = 0 ; i < n && src[i] != '\0' ; i++) {
        dest[dest_len + i] = src[i];
    }

    dest[dest_len + i] = '\0';
    return dest;
}