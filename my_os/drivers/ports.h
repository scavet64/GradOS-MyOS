#include "stdint.h"

unsigned char readPortByte (unsigned short port);
void setPortByte (unsigned short port, unsigned char data);
inline uint8_t inportb(unsigned short port);
