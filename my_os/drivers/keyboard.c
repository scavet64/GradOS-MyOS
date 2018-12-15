#include "ports.h"
#include "stdint.h"


uint8_t pollKeyboard()
{
    uint8_t scankey = port_byte_in(0x60);
    return scankey;
}