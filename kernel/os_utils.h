#ifndef OS_UTILS_H
# define OS_UTILS_H

#include <stdint.h>

void wait_ms(uint32_t ms);
uint32_t rbit(uint32_t value);
void wait(uint8_t seconds);
void wait_us(uint32_t us);
extern volatile uint32_t os_wait_us;

#endif
