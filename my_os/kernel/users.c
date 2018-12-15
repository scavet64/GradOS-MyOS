#include "../libc/include/string.h"
//#include "../drivers/screen.c"
#include "util.h"
#include <stdbool.h>

static char *defaultUsername = "admin";
static char *defaultPassword = "password";

static char *usernamesList[10] = {"ADMIN", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0"};
static char *passwordsList[10] = {"PASSWORD", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0"};

void printArray();
int countArray(char *array[]);

int checkUserData(char *username, char *password)
{
    int i;
    for (i = 0; i < 10; i++)
    {
        kprint("checked");
        if (strcmp((char *)usernamesList[i], username) == 0)
        {
            if (strcmp((char *)passwordsList[i], password) == 0)
            {
                return true;
            }
        }
    }
    return false;
}

int addUserData(char *username, char *password)
{
    int numUsers = countArray(usernamesList);
    int numPass = countArray(passwordsList);
    if (numUsers < 10 && numPass < 10)
    {
        usernamesList[1] = (char *)username;
        passwordsList[1] = (char *)password;
        countArray(usernamesList);
    }
    else
    {
    }
}