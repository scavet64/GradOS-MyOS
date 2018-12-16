
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../libc/include/string.h"
#include "../libc/include/strutils.h"
#include "commands.h"
#include "util.h"
#include "stdint.h"
#include "users.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define LEFTSHIFT 0x2A
#define LEFTSHIFTUP 0xAA
#define SHOW_INPUT 1
#define HIDE_INPUT 0
#define MAX_INPUT_LEN 250
#define SC_MAX 57

//Define some of our static variables that will be used
static char key_buffer[MAX_INPUT_LEN];
static char usernameFromUser[MAX_INPUT_LEN];
static char passwordFromUser[MAX_INPUT_LEN];
static uint8_t lastScanKey = 0;
static int shiftDown = 0;
static int isLoggedIn = 0;

/* Private methods */

void scanForInput();
void handleBackspace(int showInput);
void handleKeystroke(uint8_t scankey, int showInput);
void userInput(char *input);
void parseCommand();
void getUserNameFromUser();
void getPasswordFromUser();
void handleSingleWordCommands(char *command);
void handleTwoWordCommands(char *command, char *parm1);
void handleThreeWordCommands(char *command, char *parm1, char *parm2);
void logout();
void showHelp();
int getInformationWork(char *ptrToUpdate, int showInput);

const char *sc_name[] = {"ERROR", "Esc", "1", "2", "3", "4", "5", "6",
                         "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
                         "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
                         "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
                         "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
                         "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char up_ascii[] = {'\0', '\0', '!', '@', '#', '$', '%', '^',
                         '&', '*', '(', ')', '_', '+', '\0', '\0', 'Q', 'W', 'E', 'R', 'T', 'Y',
                         'U', 'I', 'O', 'P', '{', '}', '\0', '\0', 'A', 'S', 'D', 'F', 'G',
                         'H', 'J', 'K', 'L', ':', '\"', '~', '\0', '|', 'Z', 'X', 'C', 'V',
                         'B', 'N', 'M', '<', '>', '?', '\0', '\0', '\0', ' '};
const char sc_ascii[] = {'\0', '\0', '1', '2', '3', '4', '5', '6',
                         '7', '8', '9', '0', '-', '=', '\0', '\0', 'q', 'w', 'e', 'r', 't', 'y',
                         'u', 'i', 'o', 'p', '[', ']', '\0', '\0', 'a', 's', 'd', 'f', 'g',
                         'h', 'j', 'k', 'l', ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v',
                         'b', 'n', 'm', ',', '.', '/', '\0', '\0', '\0', ' '};

/**
 * Main loop of the entire shell. This will run forever.
 * While a user is not logged on, it will prompt for a username and password
 * 
 * If the username and password match what is known to the system, the user is logged in and can start
 * executing commands. 
 */
uint8_t runShell()
{
    printLn("Welcome to my OS :^)");

    //TEST USER CODE:
    addUserData("Edward", "fullmetal");
    addUserData("ZeroTwo", "franxx");
    addUserData("LeLouch", "geass");

    while (1)
    {
        while (!isLoggedIn)
        {
            print("\nPlease enter your username:\n> ");
            while (usernameFromUser[0] == '\0')
            {
                getUserNameFromUser();
            }

            print("Please enter your password:\n> ");
            while (passwordFromUser[0] == '\0')
            {
                getPasswordFromUser();
            }

            //Check the collected information
            isLoggedIn = checkUserData(usernameFromUser, passwordFromUser);

            if (!isLoggedIn)
            {
                printLn("Incorrect username or password:");
                passwordFromUser[0] = '\0';
                usernameFromUser[0] = '\0';
            }
            else
            {
                print("\n> ");
            }
        }

        while (isLoggedIn)
        {
            // running shell
            scanForInput();
        }
    }
}

/**
 * Loops until the user enters a username and hits enter
 */
void getUserNameFromUser()
{
    int gotUsername = 0;
    while (!gotUsername)
    {
        gotUsername = getInformationWork(usernameFromUser, SHOW_INPUT);
    }
}

/**
 * Loops until the user enters a password and hits enter
 */
void getPasswordFromUser()
{
    int gotPassword = 0;
    while (!gotPassword)
    {
        gotPassword = getInformationWork(passwordFromUser, HIDE_INPUT);
    }
}

/**
 * Get the information from the user. This function can either show or hide the keyboard input. TODO Refactor this and scanForInput into one method
 */
int getInformationWork(char *ptrToUpdate, int showInput)
{
    int retval = 0;

    uint8_t scankey = pollKeyboard();

    if (scankey != lastScanKey)
    {
        lastScanKey = scankey;

        //Check for left shift up specifically since its the only one we care about coming back up
        if (scankey == LEFTSHIFTUP)
        {
            shiftDown = 0;
        }

        if (scankey > SC_MAX)
        {
            return retval;
        }

        switch (scankey)
        {
        case LEFTSHIFT:
            shiftDown = 1;
            break;
        case BACKSPACE:
            handleBackspace(showInput);
            break;

        case ENTER:
            print("\n");
            strcpy(ptrToUpdate, key_buffer);
            key_buffer[0] = '\0';
            retval = 1;
            break;

        default:
            handleKeystroke(scankey, showInput);
            break;
        }
    }
    return retval;
}

/**
 * Scan the IO port for keyboard input. This function does not allow for the user to hold down a key.
 */
void scanForInput()
{
    uint8_t scankey = pollKeyboard();

    if (scankey != lastScanKey)
    {
        lastScanKey = scankey;

        //Check for left shift up specifically since its the only one we care about coming back up
        if (scankey == LEFTSHIFTUP)
        {
            shiftDown = 0;
        }

        if (scankey > SC_MAX)
        {
            return;
        }

        switch (scankey)
        {
        case LEFTSHIFT:
            shiftDown = 1;
            break;
        case BACKSPACE:
            handleBackspace(SHOW_INPUT);
            break;

        case ENTER:
            print("\n");
            userInput(key_buffer);
            key_buffer[0] = '\0';
            break;

        default:
            handleKeystroke(scankey, SHOW_INPUT);
            break;
        }
    }
}

/**
 * Handle the user hitting the backspace key
 */
void handleBackspace(int showInput)
{
    // If the keybuffer is already empty dont keep deleting
    if (key_buffer[0] != '\0')
    {
        backspace(key_buffer);
        if (showInput)
        {
            printBackspace();
        }
    }
    else
    {
        //TODO: Figure out dinging noise
    }
}

/**
 * Handle the user's keystroke. If the showInput flag is set to false, the input will not be echoed to the user
 */
void handleKeystroke(uint8_t scankey, int showInput)
{
    if (strlen(key_buffer) < MAX_INPUT_LEN)
    {
        char letter;
        if (shiftDown)
        {
            letter = up_ascii[(int)scankey];
        }
        else
        {
            letter = sc_ascii[(int)scankey];
        }

        if (letter == '\0') {
            //Do nothing
            return;
        }

        /* Remember that print only accepts char[] */
        char str[2] = {letter, '\0'};
        append(key_buffer, letter);

        if (showInput)
        {
            print(str);
        }
    }
    else
    {
        // Buffer protection. Stop user input
        // TODO: Figure out the dinging noise maybe
    }
}

/**
 * Process user input.
 */
void userInput(char *input)
{
    //Check login
    if (isLoggedIn)
    {
        parseCommand(input);
    }
    else
    {
        printLn("You must be logged to use this terminal");
    }

    print("\n> ");
}

/**
 *  Parses the command.
 * 
 *  While the substring is not null
 *      Get the length of the total input
 *      Get the length of the 'sub' string
 *      subtract the two
 *      do strncpy and thats what we want
 *      Add that token to an array
 */
void parseCommand(char *input)
{
    // Hacky local vars for storing the parsed input
    char firstToken[255];
    memset(firstToken, '\0', 255);
    char secondToken[255];
    memset(secondToken, '\0', 255);
    int counter = 0;

    //Trim the user's input
    input = trim(input);

    // Start by getting the first location of a space.
    char subStringData[255];
    char *subString = subStringData;
    memset(subStringData, '\0', 255);
    subString = strstr(input, " ");

    // Copy the input we got as the last total string we got the substring from
    // This is used to calculate the difference so we can tell how many chars to copy in strncpy
    char temp[255];
    char *lastTotal = temp;
    memset(temp, '\0', 255);
    strcpy(lastTotal, input);

    // While there still is a space in the substring
    while (subString != (char *)0)
    {
        // Calculate the lengths of the original string and substring
        int subLen = strlen(subString);
        int inputLen = strlen(lastTotal);
        int diff = inputLen - subLen;
        if (diff <= 0)
        {
            // some kinda problem
            printLn("Error, diff was less than 0");
            return;
        }

        // This is the ultimate hack just to get this working. I was unable to do this the way I wanted due to a memory issue.
        // When I would declare a temp char array inside the while loop and assign its value to the overall array of pointers (string array),
        // it would always overwrite the previous information. After some research, it was suggested to use strdup, but to implement that,
        // I would need to implement a memory manager to malloc the memory.
        switch (counter)
        {
            // Copy from the start of the last total up until the difference.
            // The difference will be the number of chars until we found the space.
        case 0:
            strncpy(firstToken, lastTotal, diff);
            break;
        case 1:
            strncpy(secondToken, lastTotal, diff);
            break;
        default:
            //error?
            break;
        }

        // Increment the substring to remove the space in the front, then copy into the last total so
        // it can be used in the next iteration of the loop
        subString++;
        memset(lastTotal, '\0', 255);
        strcpy(lastTotal, subString);

        // Find the new substring based on the current substring.
        subString = strstr(subString, " ");
        counter++;
    }

    switch (counter)
    {
    case 0:
        handleSingleWordCommands(input);
        break;
    case 1:
        handleTwoWordCommands(firstToken, lastTotal);
        break;
    case 2:
        handleThreeWordCommands(firstToken, secondToken, lastTotal);
        break;
    default:
        printLn("Please enter a valid command!");
        break;
    }
}

/**
 * Handle one word commands.
 */
void handleSingleWordCommands(char *command)
{
    //Check for the commands
    if (strcmp(command, "CLEAR") == 0)
    {
        clearScreen();
    }
    else if (strcmp(command, "LOGOUT") == 0)
    {
        logout();
    }
    else if (strcmp(command, "HELP") == 0)
    {
        showHelp();
    }
    else if (strcmp(command, "END") == 0)
    {
        printLn("Shutting down :^)");
        asm volatile("hlt");
    }
    else
    {
        print("No command found for: ");
        printLn(command);
    }
    return;
}

/**
 * Handle Two word commands. This is for when there is a command and a single parameter
 */
void handleTwoWordCommands(char *command, char *parm1)
{
    if (strcmp(command, "USER") == 0)
    {
        if (strcmp(parm1, "LIST") == 0)
        {
            listAllUsers();
        }
    }
    else
    {
        print("No command with 1 param found for: ");
        printLn(command);
    }
}

/**
 * Handle three word commands. This is for when there is a command and two paramaters
 */
void handleThreeWordCommands(char *command, char *parm1, char *parm2)
{
    //Check for the commands
    if (strcmp(command, "ADD") == 0)
    {
        add(parm1, parm2);
    }
    else if (strcmp(command, "SUB") == 0)
    {
        sub(parm1, parm2);
    }
    else if (strcmp(command, "MULT") == 0)
    {
        mult(parm1, parm2);
    }
    else if (strcmp(command, "DIV") == 0)
    {
        div(parm1, parm2);
    }
    else if (strcmp(command, "USER") == 0)
    {
        if (strcmp(parm1, "REGISTER") == 0)
        {
            printLn("Please enter your password:");
            memset(passwordFromUser, '\0', MAX_INPUT_LEN);
            key_buffer[0] = '\0';
            while (passwordFromUser[0] == '\0')
            {
                getPasswordFromUser();
            }
            if (addUserData(parm2, passwordFromUser))
            {
                printLn("Successfully added new user");
            }
            else
            {
                printLn("Failed to add new user");
            }
        }
        else if (strcmp(parm1, "PASSWORD") == 0)
        {
            printLn("Please enter your password:");
            memset(passwordFromUser, '\0', MAX_INPUT_LEN);
            key_buffer[0] = '\0';
            while (passwordFromUser[0] == '\0')
            {
                getPasswordFromUser();
            }
            if (changeUserPassword(parm2, passwordFromUser))
            {
                printLn("Successfully changed user password");
            }
            else
            {
                printLn("Failed to change user password");
            }
        }
    }
    else
    {
        print("No command with two params found for: ");
        printLn(command);
    }
    return;
}

/**
 * Log the current user out and clear the information
 */
void logout()
{
    print("Good bye ");
    printLn(usernameFromUser);
    memset(usernameFromUser, '\0', MAX_INPUT_LEN);
    memset(passwordFromUser, '\0', MAX_INPUT_LEN);
    isLoggedIn = 0;
}

/**
 * Show the help text to the user.
 */
void showHelp()
{
    printLn("The available commands on this OS are:");
    printLn("\tArithmatic:");
    printLn("\t\tADD param1 param2 ");
    printLn("\t\t\tAdds param 1 and param2 and displays the result");
    printLn("\t\tSUB param1 param2");
    printLn("\t\t\tSubtracts param 2 from param1 and displays the result");
    printLn("\t\tMULT param1 param2");
    printLn("\t\t\tMultiplys param 1 and param2 and displays the result");
    printLn("\t\tDIV param1 param2");
    printLn("\t\t\tInteger Divides param 1 and param2 and displays the result.");

    printLn("\tUsers:");
    printLn("\t\tUSER REGISTER param1");
    printLn("\t\t\tCreates a new user account with the name of param1.");
    printLn("\t\tUSER PASSWORD param1");
    printLn("\t\t\tModifys the password of the account matching param1");
    printLn("\t\tUSER LIST");
    printLn("\t\t\tLists the current users that the system knows about");
    printLn("\t\tLOGOUT");
    printLn("\t\t\tThis will log the user out of their session");

    printLn("\tMisc:");
    printLn("\t\tCLEAR");
    printLn("\t\t\tThis will clear the terminal window");
    printLn("\t\tEND");
    printLn("\t\t\tThis will END the session");
}