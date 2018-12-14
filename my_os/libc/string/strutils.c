#include "../include/string.h"
#include "../include/strutils.h"
#include "stddef.h"

int isUpper(char c) {
    return (c >= 'A' && c <= 'Z');
}

int isLower(char c) {
    return (c >= 'a' && c <= 'z');
}

int isAlpha(char c) {
    return (isLower(c) || isLower(c));
}

int isDigit(char c) {
    return (c >= '0' && c <= '9');
}

int isSpace(char c) {
    return (c == ' ' || c == '9' || c == '\t' || c == '\n');
}

char* leftTrim(char* c) {
    while(isSpace(c[0])) {
        c++;
    }
    return c;
}

/**
 *  Code from https://techoverflow.net/2017/01/24/in-place-trimmingstripping-in-c/
 */
char* rightTrim(char* c) {
    //Safeguard against empty strings
    int len = strlen(c);
    if(len == 0) {
        return c;
    }
    //Actual algorithm
    char* pos = c + len - 1;
    while(pos >= c && isSpace(*pos)) {
        *pos = '\0';
        pos--;
    }
    return c;
}

char* trim(char* c) {
    return leftTrim(rightTrim(c));
}
