#include <stddef.h> /* for size_t */
#include "../include/string.h"

/**
 * Same as strcmp except we use a third character to declare how many characters we will compare. **This is from Xinu, Mine was broken :( **
 * Input - s1: The first string to compare
 * Input - s2: The second string to compare
 * Input - n: the number of characters to compare
 * Output - If the return value is less than 0 it means the character in s1 is less than the character in s2.  If it is greater than 0, the character in s1 is greater than the character in s2.  If 0, the strings are the same
 */
int strncmp(const char * s1, const char * s2, size_t n)
{
    while (--n >= 0 && *s1 == *s2++)
    {
        if (*s1++ == '\0')
        {
            return 0;
        }
    }
    return (n < 0 ? 0 : *s1 - *--s2);
}
