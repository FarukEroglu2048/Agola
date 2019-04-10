#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>

extern void outb(uint16_t port, uint8_t data);
extern void outw(uint16_t port, uint16_t data);
extern void outl(uint16_t port, uint32_t data);

#endif