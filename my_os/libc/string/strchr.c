/**
 * Returns the index of a particular character (represented by an int) in a string
 * Input - s: The string to search
 * Input - c: The integer representation of a character to search for.
 * Output - The address of the character in the string
 */
char *strchr(const char *s, int c)
{
    while (*s != (char)c)
    {
        if (*s++ == '\0')
        {
            return 0;
        }
    }
    return (char *)s;
}