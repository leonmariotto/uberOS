/*
 * Copyright 2019 Mike Ryan
 *
 * This file is part of Project Ubertooth and is released under the
 * terms of the GPL. Refer to COPYING for more information.
 */

/*
** Hard Fault handler wrapper in assembly in file hardfault.S
** It extracts the location of stack frame and passes it to handler
** in C as a pointer. We also extract the LR value as second 
** parameter.
**asm(
**  ".global HardFault_Handler\n"
**  ".global HardFault_Handler_C\n"
**  "HardFault_Handler:\n"
**  "tst lr, #4 \n"
**  "ite eq \n"
**  "mrseq r0, msp \n"
**  "mrseq r0, psp \n"
**  "mov r1, lr \n"
**  "ldr r2, =HardFault_Handler_C \n"
**);
*/

#include "UBERTOOTH.h"
#include "lpc17_uart.h"

/* Reset_Handler variables defined in linker script */
extern unsigned long _StackTop;

void print_os_info(void);
static void system_error_report(unsigned long * hardfault_args, unsigned int lr_value)
{
  debug_printf ("################## FAULT_EXCEPTION HANDLER ####################\n");
	unsigned long stacked_r0;
  unsigned long stacked_r1;
  unsigned long stacked_r2;
  unsigned long stacked_r3;
  unsigned long stacked_r12;
  unsigned long stacked_lr;
  unsigned long stacked_pc;
  unsigned long stacked_psr;
	unsigned long cfsr;
	unsigned long bus_fault_address;
	unsigned long memmanage_fault_address;

	bus_fault_address       = SCB->BFAR;
	memmanage_fault_address = SCB->MMFAR;
	cfsr                    = SCB->CFSR;
	
  stacked_r0 = ((unsigned long) hardfault_args[0]);
  stacked_r1 = ((unsigned long) hardfault_args[1]);
  stacked_r2 = ((unsigned long) hardfault_args[2]);
  stacked_r3 = ((unsigned long) hardfault_args[3]);
  stacked_r12 = ((unsigned long) hardfault_args[4]);
  stacked_lr  = ((unsigned long) hardfault_args[5]);
  stacked_pc  = ((unsigned long) hardfault_args[6]);
  stacked_psr = ((unsigned long) hardfault_args[7]);

  debug_printf ("\n[[[ Fault Status Register / Fault Address Register ]]]\n");  
  debug_printf (" (configurable fault status) CFSR = %08x\n", cfsr);
  debug_printf ("         ---- (memmanage fault status) MMSFR = %02x\n", (uint8_t)cfsr);
  debug_printf ("         ---- (bus fault status) BFSR = %02x\n", (uint8_t)(cfsr >> 8));
  debug_printf ("         ---- (usage fault status) UFSR = %04x\n", (uint16_t)(cfsr >> 16));
  debug_printf (" (hard fault status) HFSR = %08x\n", SCB->HFSR);
  debug_printf (" (debug fault status) DFSR = %08x\n", SCB->DFSR);
  debug_printf (" (device-specific status) AFSR = %08x\n", SCB->AFSR);
	if (cfsr & 0x0080) debug_printf (" (memmanage fault address) MMFAR = %08x\n", memmanage_fault_address);
	if (cfsr & 0x8000) debug_printf (" (bus fault address BFAR) = %08x\n", bus_fault_address);
	debug_printf (" (Misc??) LR/EXC_RETURN= %08x\n", lr_value);

  debug_printf ("\n[[[ Registers ]]]\n"); 
	debug_printf (" R0  = %08x\n", stacked_r0);
  debug_printf (" R1  = %08x\n", stacked_r1);
  debug_printf (" R2  = %08x\n", stacked_r2);
  debug_printf (" R3  = %08x\n", stacked_r3);
  debug_printf (" R12 = %08x\n", stacked_r12);
  debug_printf (" LR  = %08x\n", stacked_lr);
  debug_printf (" PC  = %08x\n", stacked_pc);
  debug_printf (" PSR = %08x\n", stacked_psr);

  debug_printf( "\n[[[ Stack ]]]\n");
  debug_printf( " StackTop = %08x\n", &_StackTop);
  debug_printf( " Stack size = %d\n", (uintptr_t)&_StackTop - (uintptr_t)&hardfault_args[8]);
  debug_printf( " Stack content =");
  int i = 8;
  while (&hardfault_args[i] < &_StackTop) {
    if (i % 8 == 0)  {
      debug_printf("\n [%p]> ", &hardfault_args[i]);
    }
    debug_printf("%08x ", hardfault_args[i]);
    i++;
  }
  debug_printf("\n");
  debug_printf ("###############################################################\n");
}
 
// delay for about 30 ms (very rough, use a volatile int and a magic number) TODO use CYCCNT ?
static void delay_30ms(void)
{
	volatile int i;
	for (i = 0; i < 3000000; ++i)
		;
}
// flash LEDs a certain number of times
static void flash_leds(int count)
{
	int i;

	for (i = 0; i < count; ++i) {
		USRLED_SET;
		RXLED_SET;
		TXLED_SET;
		delay_30ms();
		USRLED_CLR;
		RXLED_CLR;
		TXLED_CLR;
		delay_30ms();
	}
	delay_30ms();
	delay_30ms();
}
static void led_loop(void)
{
	uint32_t val = SCB_HFSR;

	while (1) {
		flash_leds(2);

		if (val & (1 << 1)) {
			flash_leds(3); // vector table read
		} else if (val & (1 << 30)) {
			flash_leds(4); // forced (should never happen)
		}

		delay_30ms();
		delay_30ms();
		delay_30ms();
	}
}

// Hard Fault handler in C, with stack frame location and LR value
// extracted from the assembly wrapper as input parameters
void HardFault_Handler_C(unsigned long * hardfault_args, unsigned int lr_value)
{
  system_error_report(hardfault_args, lr_value);
  print_os_info();
  led_loop();
}

