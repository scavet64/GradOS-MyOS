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
        if (strcmp((char *)usernamesList[i], username) == 0)
        {
            if (strcmp((char *)passwordsList[i], password) == 0)
            {
                print("Welcome back ");
                printLn(usernamesList[i]);
                return true;
            }
        }
    }
    return false;
}

/**
 * Without malloc, this function will not work as intended. The information that I am storing into the array is being overwritten
 * since the original variable is going out of scope. I could hack something together as I did for the commands if I had more time
 */
int addUserData(char *username, char *password)
{
    int i;
    for (i = 0; i < 10; i++)
    {
        if (strcmp((char *)usernamesList[i], "\0") == 0)
        {
            if (strcmp((char *)passwordsList[i], "\0") == 0)
            {
                usernamesList[i] = (char *)username;
                passwordsList[i] = (char *)password;
                return true;
            }
        }
    }
    return false;
}

void listAllUsers()
{
    printArray(usernamesList);
}

int changeUserPassword(char *username, char *password)
{
    int i;
    for (i = 0; i < 10; i++)
    {
        if (strcmp((char *)usernamesList[i], username) == 0)
        {
            passwordsList[i] = (char *)password;
        }
    }
    return false;
}