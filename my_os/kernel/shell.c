
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
        char clone[50];
        char *tmp = clone;
        //char *clone = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        strcpy(tmp, input);
        parseCommand(tmp);
    }
    else
    {
    }

    kprint("\nYou said: ");
    kprint(input);
    kprint("\n> ");
}

void parseCommand(char *input)
{
    //First trim the user input
    kprint("\nParsing Command: ");
    //kprint(input);
    //input = trim(input);
    char index[255];
    char *tmp = index;
    tmp = strstr(input, " ");

    if(tmp == (char *)0){
        kprint("\nnot found ");
        return;
    }

    kprint("\nstrstr result: ");
    kprint(tmp);
    kprint("\n");

    int subLen = strlen(tmp);
    char str1[123];
    int_to_ascii(subLen, str1);
    kprint(str1);
    kprint("\n");

    int inputLen = strlen(input);
    int_to_ascii(inputLen, str1);
    kprint(str1);
    kprint("\n");

    

    int diff = inputLen - subLen;
    if (diff <= 0)
    {
        // Did not have it
        kprint("subLen was 0\n");
        return;
    }

    int_to_ascii(diff, str1);
    kprint(str1);
    kprint("\n");

    char token[255];
    strncpy(token, input, diff);

    kprint("\nTOKEN: ");
    kprint(token);
    kprint("\n");

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