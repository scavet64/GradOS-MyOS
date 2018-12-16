#include "../include/strutils.h"

/**
 * Converts a string into an integer. 
 * 
 * If character in the string is not a digit, it is ignored as if it was never there.
 * This does not handle integer overflow
 */
int atoi(char *str)
{
    int result = 0; // Initialize result
    int i;
    int isNeg = 0;

    for (i = 0; str[i] != '\0'; ++i)
    {
        if (str[i] == '-')
        {
            isNeg = 1;
        }
        else if (isDigit(str[i]))
        {
            result = result * 10 + str[i] - '0';
        }
    }

    if (isNeg)
    {
        result = result * -1;
    }

    // return result.
    return result;
}