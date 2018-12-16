/**
 * Copies the memory from the source into the destination up until the first n bytes
 */
void memcpy(char *source, char *dest, int nbytes)
{
    int i;
    for (i = 0; i < nbytes; i++)
    {
        *(dest + i) = *(source + i);
    }
}