void memory_copy(char *source, char *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
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
    int i = 0;
    while (strcmp(array[i], "\0") != 0){
        //We only want to increment i if contiditonal was successful
        i++;
    }
    return i;
}

void printArray(char *array[])
{
    print("\n");
    int size = countArray(array);
    int i = 0;
    for (i = 0; i < size; i++)
    {
        print(array[i]);
        print("\n");
    }
}