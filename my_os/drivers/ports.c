#include "stddef.h"
#include "stdint.h"

/**
 * Read a byte from the specified port
 */
unsigned char readPortByte(unsigned short port)
{
    unsigned char result;
    /* Inline assembler syntax
     * !! Notice how the source and destination registers are switched from NASM !!
     *
     * '"=a" (result)'; set '=' the C variable '(result)' to the value of register e'a'x
     * '"d" (port)': map the C variable '(port)' into e'd'x register
     *
     * Inputs and outputs are separated by colons
     */
    __asm__("in %%dx, %%al"
            : "=a"(result)
            : "d"(port));
    return result;
}

/**
 * Set the data at the specified port using the passed in data
 */
void setPortByte(unsigned short port, unsigned char data)
{
    /* Notice how here both registers are mapped to C variables and
     * nothing is returned, thus, no equals '=' in the asm syntax 
     * However we see a comma since there are two variables in the input area
     * and none in the 'return' area
     */
    __asm__("out %%al, %%dx"
            :
            : "a"(data), "d"(port));
}

/**
 * Alternative read byte method
 */
inline uint8_t inportb(unsigned short port)
{
    char ret;
    asm volatile("inb %1, %0"
                 : "=a"(ret)
                 : "Nd"(port));
    return ret;
}
