#include <stddef.h> /* for size_t */
#include "../include/string.h"

/**
 * Copy the second string s into the first string d
 * Input - d: The pointer to the address that we will copy a string into
 * Input - s: The pointer to the string that will serve as our source string.
 * Output - The address to the destination string.
 */
char* strcpy(char* d, const char *s) {
    size_t i;

    for (i = 0; s[i] != '\0'; i++) {
            d[i] = s[i];
    }

    d[i] = '\0';
    return d;
}