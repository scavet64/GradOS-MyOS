
#include "../drivers/screen.h"
#include "../drivers/ports.h"
#include "commands.h"
#include "util.h"
#include "stdint.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C

// The key buffer is what the user types.
static char key_buffer[256];
static uint8_t lastScanKey = 0;

/* Private methods */

void scanForInput();
void handleBackspace();
void handleEnter();
void handleKeystroke(uint8_t scankey);

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
    while (1)
    {
        scanForInput();
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
            handleEnter();
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
    kprint("\n");
    user_input(key_buffer); /* kernel-controlled function */
    key_buffer[0] = '\0';
}

void handleKeystroke(uint8_t scankey)
{
    if (strlen(key_buffer) < 256)
    {
        char letter = sc_ascii[(int)scankey];
        /* Remember that kprint only accepts char[] */
        char str[2] = {letter, '\0'};
        append(key_buffer, letter);
        kprint(str);
    } else {
        // Buffer protection. Stop user input
        // TODO: Figure out the dinging noise maybe
    }
}