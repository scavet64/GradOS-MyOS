#ifndef _STRING_H
#define _STRING_H 1
 
#include "stddef.h"

/**
 * Compares the first n bytes of two memory locations.
 * Input - aptr: The first memory location to compare
 * Input - bptr: The second memory location to compare
 * Input - size: The amount of bytes to compare.
 * Output - Returns 0 if the first n bytes are the same.  If we hit a byte that is not the same in both we return -1 if the value in the a byte is less than the byte value in b, or 1 if the byte value in b is less than the byte value in a.
 */
int memcmp(const void* aptr, const void* bptr, size_t size);

/**
 * Fills the first n bytes of a memory location with a particular value
 * Input - bufptr: The pointer to the address we will start filling in information
 * Input - value: The value we will use to fill in the buffer.
 * Input - size: The number of bytes in the buffer we will write
 * Output - None.
 */
void* memset(void* bufptr, int value, size_t size);

/**
 * Returns the length, in characters, of the string pointed to by the parameter
 * Input - char*: The string we want to get the length for
 * Output - The number of characters in the string
 */
size_t strlen(const char* s);

/**
 * Concatenate two strings together such that strcat(d,s) will give us the string d followed by the string s
 * Input - d: The leftmost string in our concatenated string
 * Input - s: The rightmost string in our concatenated string
 * Output - The address of the two strings concatenated together
 */
char* strcat(char* d, const char* s);

/**
 * Copy the second string s into the first string d
 * Input - d: The pointer to the address that we will copy a string into
 * Input - s: The pointer to the string that will serve as our source string.
 * Output - The address to the destination string.
 */
char* strcpy(char* d, const char* s);

/**
 * Compares the characters in a string until we see the strings as equal or we get different characters
 * Input - s1: The first string to compare
 * Input - s2: The second string to compare
 * Output - If the return value is less than 0 it means the character in s1 is less than the character in s2.  If it is greater than 0, the character in s1 is greater than the character in s2.  If 0, the strings are the same
 */
int strcmp(const char * s1, const char * s2);

/**
 * Same as strcat except we only concatenate n characters.
 */
char *strncat(char *dest, const char *src, size_t n);

/**
 * Same as strcpy except we only copy the first n bytes.
 */
char *strncpy(char *dest, const char *src, size_t n);

/**
 * Looks for a particular string within another string
 * Input - s1: The string to search
 * Input - s2: The string to search for
 * Output - Returns the address of s2 within s1.
 */
char *strstr(char *s1, const char *s2);

/**
 * Returns the index of a particular character (represented by an int) in a string
 * Input - s: The string to search
 * Input - c: The integer representation of a character to search for.
 * Output - The address of the character in the string
 */
char *strchr(const char *s, int c);

/**
 * Same as strcmp except we use a third character to declare how many characters we will compare.
 */
int strncmp(const char * s1, const char * s2, size_t n);

/**
 * Translates a character into a string.
 * Input - s: A two character array that will hold the character we want to convert to a string followed by the null character
 * Input - c: the character we want to convert into a string
 * Output - A pointer to the array that now holds the character string.
 */
char *ctos(char s[2], const char c);

#endif