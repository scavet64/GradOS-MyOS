#include "ports.h"
#include "stdint.h"

#define KEYBOARD_PORT 0x60

/**
 * Poll the keyboard on port 0x60 and return the value
 */
uint8_t pollKeyboard()
{
    uint8_t scankey = readPortByte(KEYBOARD_PORT);
    return scankey;
}