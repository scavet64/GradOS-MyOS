#include "../libc/include/string.h"
#include <stdbool.h>

static char *defaultUsername = "admin";
static char *defaultPassword = "password";

static char usernamesList[10][20] = {"ADMIN"};
static char passwordsList[10][20] = {"PASSWORD"};

int checkUserData(char *username, char *password)
{
    int i;
    for (i = 0; i < 10; i++)
    {
        if (strcmp(usernamesList[i], username) == 0)
        {
            if (strcmp(passwordsList[i], password) == 0)
            {
                return true;
            }
        }
    }
    return false;
}