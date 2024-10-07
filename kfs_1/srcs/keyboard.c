#include <kernel.h>

unsigned char inb(unsigned short _port) {
    unsigned char ret;
    asm volatile("in %%dx, %%al" : "=a"(ret) : "d"(_port) : "memory");
    return ret;
}

void outb(unsigned char _value, unsigned short _port) {
    asm volatile("out %%al, %%dx" : : "a"(_value), "d"(_port) : "memory");
}

unsigned char get_keyboard_input() {
  while (!(inb(0x64) & 0x1))
    ;
  return inb(0x60);
}