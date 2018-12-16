/**
 * Returns total size of an array
 */
int countArray(char *array[])
{
    int i = 0;
    while (strcmp(array[i], "\0") != 0){
        //We only want to increment i if conditional was successful
        i++;
    }
    return i;
}

/**
 * Prints the contents of the array to the screen
 */
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