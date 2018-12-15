
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

    while (!isLoggedIn)
    {
        kprint("Please enter your username:\n");
        while (usernameFromUser[0] == '\0')
        {
            getUserNameFromUser();
        }

        kprint("Please enter your password:\n");
        while (passwordFromUser[0] == '\0')
        {
            getPasswordFromUser();
        }

        //Check the collected information
        isLoggedIn = checkUserData(usernameFromUser, passwordFromUser);

        if (!isLoggedIn)
        {
            kprint("Incorrect username or password:\n");
            passwordFromUser[0] = '\0';
            usernameFromUser[0] = '\0';
        }
    }

    while (1)
    {
        // running shell
        scanForInput();
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
                kprint("\n");
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
                kprint("\n");
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
            kprint("\n");
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
    kprint_backspace();
}

void handleEnter()
{
}

void handleKeystroke(uint8_t scankey)
{
    if (strlen(key_buffer) < 50)
    {
        char letter = sc_ascii[(int)scankey];
        /* Remember that kprint only accepts char[] */
        char str[2] = {letter, '\0'};
        append(key_buffer, letter);
        kprint(str);
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
        kprint("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    }

    //Check login
    if (isLoggedIn)
    {
        // I need to initalize the variable /shrug
        // char *clone;
        // memset(clone, '\0', 254);
        // //char *clone = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        // strcpy(clone, input);
        parseCommand(input);
    }
    else
    {
    }

    // kprint("\nYou said: ");
    // kprint(input);
    // kprint("\n> ");
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
    //Initialize the tokens array with a few elements
    char *tokens[255];
    memset(*tokens, '\0', 255);
    int counter = 0;

    //Trim the user's input
    //kprint("\nParsing Command: ");
    //kprint(input);
    input = trim(input);
    char *subString;
    memset(subString, '\0', 255);
    subString = strstr(input, " ");

    // Was there a space at all in the input. If not return. I probably want to do something else here
    if (subString == (char *)0)
    {
        kprint("\nnot found ");
        return;
    }

    // Copy the input we got as the last total string we got the substring from
    // This is used to calculate the difference so we can tell how many chars to copy in strncpy
    char *lastTotal;
    memset(lastTotal, '\0', 255);
    strcpy(lastTotal, input);

    // While there still is a space in the substring
    while (subString != (char *)0)
    {
        // Calculate the lengths of the original string and substring
        kprint("\nIn Loop\n");
        int subLen = strlen(subString);
        //char str1[123];
        // int_to_ascii(subLen, str1);
        // kprint(str1);
        // kprint("\n");

        int inputLen = strlen(lastTotal);
        // int_to_ascii(inputLen, str1);
        // kprint(str1);
        // kprint("\n");

        // Get the difference. Check the difference for an error
        int diff = inputLen - subLen;
        if (diff <= 0)
        {
            // some kinda problem
            kprint("subLen was 0\n");
            return;
        }

        // int_to_ascii(diff, str1);
        // kprint(str1);
        // kprint("\n");
        // char token[255];
        // memset(token, '\0', 255);

        // Copy from the start of the last total up until the difference. The difference will be the number of chars
        // until we found the space. The result of the copy goes into an array of strings
        strncpy(tokens[counter], lastTotal, diff);

        // kprint("subString b4+: ");
        // kprint(subString);
        // kprint("\n");

        // Increment the substring to remove the space in the front, then copy into the last total so
        // it can be used in the next iteration of the loop
        subString++;
        memset(lastTotal, '\0', 255);
        strcpy(lastTotal, subString);

        kprint("subString a+: ");
        kprint(subString);
        kprint("\n");

        kprint("TOKEN: ");
        kprint(tokens[counter]);
        kprint("\n");

        kprint("lastTotal: ");
        kprint(lastTotal);
        kprint("\n");

        // Find the new substring based on the current substring.
        subString = strstr(subString, " ");
        if (subString == (char *)0)
        {
            kprint("Did not find another space\n");
        }
        else
        {
            kprint("subString after new search: ");
            kprint(subString);
            kprint("\n");
        }

        //finally increment the counter
        counter++;
    }

    //Last total is now the final param
    tokens[counter] = lastTotal;
    counter++;
    //kprint("Out of Loop\n");

    int i = 0;
    for (i = 0; i < counter; i++)
    {
        kprint("Counted: ");
        kprint(tokens[i]);
        kprint("\n");
    }

    if (counter >= 2)
    {
        if (strcmp(tokens[0], "QWE") == 0)
        {
            kprint("Match: QWE\n");
        }
        else
        {
            kprint("Not matched: QWE - ");
            kprint(tokens[0]);
        }

        if (strcmp(tokens[1], "ASD") == 0)
        {
            kprint("Match: ASD\n");
        }
        else
        {
            kprint("Not matched: ASD - ");
            kprint(tokens[1]);
        }

        if (strcmp(tokens[2], "ZXC") == 0)
        {
            kprint("Match: ZXC\n");
        }
        else
        {
            kprint("Not matched: ZXC - ");
            kprint(tokens[2]);
        }
    }

    // Get the length of the total input
    // Get the length of the 'sub' string
    // subtract the two
    // do strncpy and thats what we want

    //

    // if (index == 0) {
    //     //No spaces
    // } else {
    //     // There was a space detected
    //     kprint("Space was detected\n");

    //     //Get the first item before the space
    //     char *dest;
    //     strncpy(*dest, input, index);

    //     kprint("\n");
    //     kprint("First User Word:");
    //     kprint(dest);
    // }
}