#include "os_utils.h"


/* delay seconds while on internal oscillator (4 MHz) */
void wait(uint8_t seconds)
{
	wait_us(seconds * 1000000);
}

/* delay milliseconds while on internal oscillator (4 MHz) */
void wait_ms(uint32_t ms)
{
	wait_us(ms * 1000);
}

/* efficiently reverse the bits of a 32-bit word */
uint32_t rbit(uint32_t value)
{
  uint32_t result = 0;
  asm("rbit %0, %1" : "=r" (result) : "r" (value));
  return result;
}
/* delay a number of microseconds while on internal oscillator (4 MHz) */
/* we only have a resolution of 1000/400, so to the nearest 2.5        */
volatile uint32_t os_wait_us;
void wait_us(uint32_t us)
{
	/* This is binary multiply by ~0.3999, i.e, multiply by
	   0.011011011b. The loop also contains 6 instructions at -Os, so
	   why this factor works is not at all related to the comment
	   above ;-) */
	os_wait_us =
		(us>>2) + (us>>3) + (us>>6) + (us>>7) + (us>>10) + (us>>11);
	while(--os_wait_us);
}

