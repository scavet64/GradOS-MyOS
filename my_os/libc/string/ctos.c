/**
 * Translates a character into a string.
 * Input - s: A two character array that will hold the character we want to convert to a string followed by the null character
 * Input - c: the character we want to convert into a string
 * Output - A pointer to the array that now holds the character string.
 */
char *ctos(char s[2], const char c)
{
    s[0] = c;
    s[1] = '\0';
}