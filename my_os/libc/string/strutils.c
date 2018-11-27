#include "../include/string.h"
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
    while(isSpace(c)) {
        c++;
    }
    return c;
}

char* rightTrim(char* c) {
    size_t len = strlen(c);

    while(isSpace(c[len])) {
        c[len] = '\n';
        len--;
    }
    return c;
}

char* trim(char* c) {
    return leftTrim(rightTrim(c));
}