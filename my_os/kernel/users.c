#include "../libc/include/string.h"
#include "util.h"
#include <stdbool.h>
#define MAX_USERS 10

static char *usernamesList[MAX_USERS] = {"ADMIN", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0"};
static char *passwordsList[MAX_USERS] = {"PASSWORD", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0"};

void printArray();
int countArray(char *array[]);

/**
 * Checks the passed in user data to see if it matches what the system knows about
 */
int checkUserData(char *username, char *password)
{
    int i;
    for (i = 0; i < MAX_USERS; i++)
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
    for (i = 0; i < MAX_USERS; i++)
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

/**
 * Prints the array of usernames to the screen
 */
void listAllUsers()
{
    printArray(usernamesList);
}

/**
 * Changes the password of the passed in username
 */
int changeUserPassword(char *username, char *password)
{
    int i;
    for (i = 0; i < MAX_USERS; i++)
    {
        if (strcmp((char *)usernamesList[i], username) == 0)
        {
            passwordsList[i] = (char *)password;
            return true;
        }
    }
    return false;
}