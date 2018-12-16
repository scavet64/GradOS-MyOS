#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "stdint.h"

/**
 * Poll the keyboard on port 0x60 and return the value
 */
uint8_t pollKeyboard();

#endif