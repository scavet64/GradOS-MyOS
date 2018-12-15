void reverse(char s[]);

void memory_copy(char *source, char *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

/**
 * K&R implementation
 */
void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}

/* K&R */
void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void append(char s[], char n) {
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}

void backspace(char s[]) {
    int len = strlen(s);
    s[len-1] = '\0';
}


int countArray(char *array[])
{
    kprint("\n");
    kprint("\n");
    int i = 0;
    char *tmp = array[i];
    while (strcmp(array[i], "\0") != 0)
    {
        //DEBUG INFORMATION: TODO DELETE
        tmp = array[i];
        kprint("Index: ");
        //kprintInt(i);

        char *str;
        int_to_ascii(i, str);
        kprint(str);

        kprint(" Value: ");
        kprint(tmp);

        kprint("\n");
        i++;
    }
    return i;
}

void printArray(char *array[])
{
    kprint("\n");
    int size = countArray(array);
    int i = 0;
    for (i = 0; i < size; i++)
    {
        kprint(array[i]);
        kprint("\n");
    }
}