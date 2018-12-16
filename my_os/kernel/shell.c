
#include "../drivers/screen.h"
#include "../drivers/ports.h"
#include "../libc/include/string.h"
#include "../libc/include/strutils.h"
#include "commands.h"
#include "util.h"
#include "stdint.h"
#include "users.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C

// The key buffer is what the user types.
static char key_buffer[256];
static char usernameFromUser[256];
static char passwordFromUser[256];
static uint8_t lastScanKey = 0;
static int isLoggedIn = 0;

/* Private methods */

void scanForInput();
void handleBackspace();
void handleEnter();
void handleKeystroke(uint8_t scankey);
void userInput(char *input);
void parseCommand();
void getUserNameFromUser();
void getPasswordFromUser();
void handleSingleWordCommands(char *command);
void handleTwoWordCommands(char *command, char *parm1);
void handleThreeWordCommands(char *command, char *parm1, char *parm2);
void handleFourWordCommands(char *command, char *parm1, char *parm2, char *parm3);
void logout();
void showHelp();

#define SC_MAX 57
const char *sc_name[] = {"ERROR", "Esc", "1", "2", "3", "4", "5", "6",
                         "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
                         "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
                         "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
                         "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
                         "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char sc_ascii[] = {'?', '?', '1', '2', '3', '4', '5', '6',
                         '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
                         'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G',
                         'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
                         'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

uint8_t runShell()
{
    //TEST CODE:
    addUserData("TEST", "TEST");

    while (1)
    {
        while (!isLoggedIn)
        {
            print("Please enter your username:\n");
            while (usernameFromUser[0] == '\0')
            {
                getUserNameFromUser();
            }

            print("Please enter your password:\n");
            while (passwordFromUser[0] == '\0')
            {
                getPasswordFromUser();
            }

            //Check the collected information
            isLoggedIn = checkUserData(usernameFromUser, passwordFromUser);

            if (!isLoggedIn)
            {
                print("Incorrect username or password:\n");
                passwordFromUser[0] = '\0';
                usernameFromUser[0] = '\0';
            }
        }

        print("\n> ");

        while (isLoggedIn)
        {
            // running shell
            scanForInput();
        }
    }
}

void getUserNameFromUser()
{
    while (1)
    {
        uint8_t scankey = port_byte_in(0x60);

        if (scankey != lastScanKey)
        {
            lastScanKey = scankey;
            if (scankey > SC_MAX)
            {
                continue;
            }

            switch (scankey)
            {
            case BACKSPACE:
                handleBackspace();
                break;

            case ENTER:
                print("\n");
                strcpy(usernameFromUser, key_buffer);
                key_buffer[0] = '\0';
                //User pressed enter, get out of this function and move on to getting the password.
                return;

            default:
                handleKeystroke(scankey);
                break;
            }
        }
    }
}

void getPasswordFromUser()
{
    while (1)
    {
        uint8_t scankey = port_byte_in(0x60);

        if (scankey != lastScanKey)
        {
            lastScanKey = scankey;
            if (scankey > SC_MAX)
            {
                continue;
            }

            switch (scankey)
            {
            case BACKSPACE:
                handleBackspace();
                break;

            case ENTER:
                print("\n");
                strcpy(passwordFromUser, key_buffer);
                key_buffer[0] = '\0';
                //User pressed enter, get out of this function and move on to getting the password.
                return;

            default:
                handleKeystroke(scankey);
                break;
            }
        }
    }
}

void scanForInput()
{
    uint8_t scankey = port_byte_in(0x60);

    if (scankey != lastScanKey)
    {
        lastScanKey = scankey;
        if (scankey > SC_MAX)
        {
            return;
        }

        switch (scankey)
        {
        case BACKSPACE:
            handleBackspace();
            break;

        case ENTER:
            print("\n");
            userInput(key_buffer);
            key_buffer[0] = '\0';
            break;

        default:
            handleKeystroke(scankey);
            break;
        }
    }
}

void handleBackspace()
{
    backspace(key_buffer);
    print_backspace();
}

void handleEnter()
{
}

void handleKeystroke(uint8_t scankey)
{
    if (strlen(key_buffer) < 50)
    {
        char letter = sc_ascii[(int)scankey];
        /* Remember that print only accepts char[] */
        char str[2] = {letter, '\0'};
        append(key_buffer, letter);
        print(str);
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
    if (strcmp(input, "END") == 0)
    {
        printLn("Shutting down :^)");
        asm volatile("hlt");
    }

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
 * 
 */
void parseCommand(char *input)
{
    // Hacky local vars for storing the parsed input
    char firstToken[255];
    memset(firstToken, '\0', 255);
    char secondToken[255];
    memset(secondToken, '\0', 255);
    char thirdToken[255];
    memset(thirdToken, '\0', 255);
    char fourthToken[255];
    memset(fourthToken, '\0', 255);
    int counter = 0;

    // printLn(firstToken);
    // printLn(secondToken);
    // printLn(thirdToken);
    // printLn(fourthToken);

    //Trim the user's input
    input = trim(input);

    // Start by getting the first location of a space.
    char *subString;
    memset(subString, '\0', 255);
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
        // For me to declare a temp char array inside the while loop and assign its value to the overall array of pointers (string array),
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
        case 2:
            strncpy(thirdToken, lastTotal, diff);
            break;
        case 3:
            strncpy(fourthToken, lastTotal, diff);
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

    // printLn("");

    // printLn(firstToken);
    // printLn(secondToken);
    // printLn(thirdToken);
    // printLn(fourthToken);
    // printLn(lastTotal);

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
    case 3:
        //strncpy(fourthToken, lastTotal, diff);
        break;
    default:
        //error?
        break;
    }

    // Get the length of the total input
    // Get the length of the 'sub' string
    // subtract the two
    // do strncpy and thats what we want
}

void handleSingleWordCommands(char *command)
{
    //Single word command
    printLn("\nSingle word command detected");
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
    else
    {
        print("No command found for: ");
        printLn(command);
    }
    return;
}

void handleTwoWordCommands(char *command, char *parm1)
{
    if (strcmp(command, "USER") == 0)
    {
        if (strcmp(parm1, "LIST") == 0)
        {
            listAllUsers();
        }
    }
}

void handleFourWordCommands(char *command, char *parm1, char *parm2, char *parm3);

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
            memset(passwordFromUser, '\0', 255);
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
            memset(passwordFromUser, '\0', 255);
            key_buffer[0] = '\0';
            while (passwordFromUser[0] == '\0')
            {
                getPasswordFromUser();
            }
            changeUserPassword(parm2, passwordFromUser);
        }
    }
    else
    {
        print("No command found for: ");
        printLn(command);
    }
    return;
}

void logout()
{
    memset(usernameFromUser, '\0', 255);
    memset(passwordFromUser, '\0', 255);
    isLoggedIn = 0;
}

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
}