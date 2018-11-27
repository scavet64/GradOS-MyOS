#include <stddef.h> /* for size_t */
#include "../include/string.h"

/**
 * Looks for a particular string within another string
 * Input - s1: The string to search
 * Input - s2: The string to search for
 * Output - Returns the address of s2 within s1.
 */
char *strstr(char *s1, const char *s2) {
    size_t n = strlen(s2);
    while(*s1 != '\0')  {
        if (!memcmp(s1++,s2,n)) {
            return s1-1;
        }
    }
    return 0;
}