#ifndef WAIT_H
#define WAIT_H

#include <stdint.h>

#define WNOHANG 1

uint16_t wait(int* wstatus);
uint16_t waitpid(uint16_t pid, int* wstatus, int options);

#endif
