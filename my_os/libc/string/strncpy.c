#include <stddef.h> /* for size_t */
// #include "../include/string.h"

/**
 * Copy the second string s into the first string d
 * Input - d: The pointer to the address that we will copy a string into
 * Input - s: The pointer to the string that will serve as our source string.
 * Output - The address to the destination string.
 */
char *strncpy(char *dest, const char *src, size_t n) {
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++) {
            dest[i] = src[i];
    }
    while (i < n) {
        dest[i] = '\0';
        i++;
    }
    return dest;
}