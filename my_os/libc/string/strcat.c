#include "../include/string.h"

/**
 * Concatenate two strings together such that strcat(d,s) will give us the string d followed by the string s
 * Input - d: The leftmost string in our concatenated string
 * Input - s: The rightmost string in our concatenated string
 * Output - The address of the two strings concatenated together
 */
char *strcat(char *d, const char *s)
{
    size_t firstLen = strlen(d);
    size_t i;

    for (i = 0; s[i] != '\0'; i++)
    {
        d[firstLen + i] = s[i];
    }

    d[firstLen + i] = '\0';
    return d;
}