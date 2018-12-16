#include <stddef.h> /* for size_t */
#include "../include/string.h"

// /**
//  * Looks for a particular string within another string
//  * Input - s1: The string to search
//  * Input - s2: The string to search for
//  * Output - Returns the address of s2 within s1.
//  */
// char *strstr(char *s1, const char *s2) {
//     size_t n = strlen(s2);
//     while(*s1 != '\0')  {
//         if (!memcmp(s1++,s2,n)) {
//             return s1-1;
//         }
//     }
//     return 0;
// }

char *strstr(
    const char *cs, /* String to search		*/
    const char *ct  /* Substring to locate		*/
)
{
    char *cq;
    char *cr;

    for (; *cs != '\0'; cs++)
    {
        if (*cs == *ct)
        {
            cq = (char *)cs;
            cr = (char *)ct;
            while ((*cq != '\0') && (*cr != '\0'))
            {
                if (*cq != *cr)
                {
                    break;
                }
                cq++;
                cr++;
            }
            if ('\0' == *cr)
            {
                return (char *)cs;
            }
        }
    }
    return 0;
}
