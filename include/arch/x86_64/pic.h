#ifndef PIC_H
#define PIC_H

#include <stdint.h>
#include <stdbool.h>

void pic_send_eoi(bool send_slave);
void pic_initialize(uint8_t master_idt_base, uint8_t slave_idt_base);

#endif