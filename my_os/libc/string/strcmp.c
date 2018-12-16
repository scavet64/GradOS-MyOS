#include <stddef.h> /* for size_t */

/**
 * Compares the characters in a string until we see the strings as equal or we get different characters
 * Input - s1: The first string to compare
 * Input - s2: The second string to compare
 * Output - If the return value is less than 0 it means the character in s1 is less than the character in s2.  If it is greater than 0, the character in s1 is greater than the character in s2.  If 0, the strings are the same
 */
int strcmp(const char *s1, const char *s2)
{
    // While neither string is terminated and the chars are still equal
    while (*s1 && (*s1 == *s2))
    {
        s1++, s2++;
    }

    // Return the difference between the final character.
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}