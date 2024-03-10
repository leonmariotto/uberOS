/*
 * Copyright 2017 Mike Ryan
 *
 * This file is part of Project Ubertooth and is released under the
 * terms of the GPL. Refer to COPYING for more information.
 */

#ifndef __DEBUG_UART_H__
#define __DEBUG_UART_H__

extern int debug_dma_active;

// initialize debug UART
void lpc17_uart_init(int flow_control);

// write a string to UART synchronously without DMA
// you probably don't want this
void debug_write(const char *str);

// printf to debug UART -- this is the one you want!
void debug_printf_dma(char *fmt, ...);
void debug_printf(char *fmt, ...);

#endif
