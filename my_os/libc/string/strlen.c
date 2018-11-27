#include <stddef.h> /* for size_t */
#include "../include/string.h" 

/**
 * Returns the length, in characters, of the string pointed to by the parameter
 * Input - char*: The string we want to get the length for
 * Output - The number of characters in the string
 */
size_t strlen(const char* s) {
    size_t characterCount = 0;

    // While the character at the current position is not the termination character, keep counting
    while (s[characterCount] != '\0') {
        characterCount++;
    }
    return characterCount;
}