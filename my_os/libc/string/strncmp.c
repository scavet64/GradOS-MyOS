#include <stddef.h> /* for size_t */
#include "../include/string.h"

/**
 * Same as strcmp except we use a third character to declare how many characters we will compare.
 * Input - s1: The first string to compare
 * Input - s2: The second string to compare
 * Input - n: the number of characters to compare
 * Output - If the return value is less than 0 it means the character in s1 is less than the character in s2.  If it is greater than 0, the character in s1 is greater than the character in s2.  If 0, the strings are the same
 */
int strncmp(const char * s1, const char * s2, size_t n) {
    // While number of bytes is greater than 0, the character is not terminating, and they are still equal
    int i = 0;
    while (i < n && s1[i] != '\0' && (s1[i]==s2[i])) {
        i++;
    }
    
    // Return the difference
    return *(unsigned char*)(s1[i] - 1) - *(unsigned char*)(s2[i] - 1);
}