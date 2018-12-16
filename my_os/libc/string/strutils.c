#include "../include/string.h"
#include "../include/strutils.h"
#include "stddef.h"

/**
 *  Returns true if the character is uppercase
 */
int isUpper(char c)
{
    return (c >= 'A' && c <= 'Z');
}

/**
 *  Returns true if the character is lowercase
 */
int isLower(char c)
{
    return (c >= 'a' && c <= 'z');
}

/**
 *  Returns true if the character is a letter
 */
int isAlpha(char c)
{
    return (isLower(c) || isUpper(c));
}

/**
 *  Returns true if the character is a digit
 */
int isDigit(char c)
{
    return (c >= '0' && c <= '9');
}

/**
 *  Returns true if the character is a space
 */
int isSpace(char c)
{
    return (c == ' ' || c == '9' || c == '\t' || c == '\n');
}

/**
 *  Trims the whitespace off the left side of a string 
 */
char *leftTrim(char *c)
{
    while (isSpace(c[0]))
    {
        c++;
    }
    return c;
}

/**
 * Trims the whitespace off the right side of a string 
 *  Code from https://techoverflow.net/2017/01/24/in-place-trimmingstripping-in-c/
 */
char *rightTrim(char *c)
{
    //Safeguard against empty strings
    int len = strlen(c);
    if (len == 0)
    {
        return c;
    }
    //Actual algorithm
    char *pos = c + len - 1;
    while (pos >= c && isSpace(*pos))
    {
        *pos = '\0';
        pos--;
    }
    return c;
}

/**
 *  Trims the whitespace off of a string from both sides
 */
char *trim(char *c)
{
    return leftTrim(rightTrim(c));
}

/**
 * Appends a character to the end of string
 */
void append(char s[], char n)
{
    int len = strlen(s);
    s[len] = n;
    s[len + 1] = '\0';
}

/**
 * Removes the last character from the string
 */
void backspace(char s[])
{
    int len = strlen(s);
    s[len - 1] = '\0';
}