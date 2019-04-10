#include <stdint.h>
#include <stdbool.h>

#include <arch/x86_64/io/ports.h>

#define PIC_PORT_MASTER_COMMAND 0x0020
#define PIC_PORT_MASTER_DATA 0X0021

#define PIC_PORT_SLAVE_COMMAND 0x00A0
#define PIC_PORT_SLAVE_DATA 0x00A1

#define PIC_COMMAND_EOI 0x20

#define PIC_ICW1_FLAG_INIT_MODE 0x10
#define PIC_ICW1_FLAG_ICW4 0x01

#define PIC_ICW4_FLAG_8086 0x01

#define PIC_SLAVE_INTERRUPT 2

void pic_send_eoi(bool send_slave)
{
    if (send_slave == true) outb(PIC_PORT_SLAVE_COMMAND, PIC_COMMAND_EOI);
    outb(PIC_PORT_MASTER_COMMAND, PIC_COMMAND_EOI);
}

void pic_initialize(uint8_t master_idt_base, uint8_t slave_idt_base)
{
    outb(PIC_PORT_MASTER_COMMAND, PIC_ICW1_FLAG_INIT_MODE | PIC_ICW1_FLAG_ICW4);
    outb(PIC_PORT_MASTER_DATA, master_idt_base);
    outb(PIC_PORT_MASTER_DATA, 0b1 << PIC_SLAVE_INTERRUPT);
    outb(PIC_PORT_MASTER_DATA, PIC_ICW4_FLAG_8086);

    outb(PIC_PORT_SLAVE_COMMAND, PIC_ICW1_FLAG_INIT_MODE | PIC_ICW1_FLAG_ICW4);
    outb(PIC_PORT_SLAVE_DATA, slave_idt_base);
    outb(PIC_PORT_SLAVE_DATA, PIC_SLAVE_INTERRUPT);
    outb(PIC_PORT_SLAVE_DATA, PIC_ICW4_FLAG_8086);
}