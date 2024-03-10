#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "UBERTOOTH.h"
#include "tinyprintf.h"

int debug_dma_active = 0;
char debug_buffer[512] = {0};

/* size in byte (aligned to 4) , pointer shall be aligned */
void  memcpy_u32_vol(volatile uint32_t *dst, const uint32_t *src, size_t size)
{
  for (size_t i = 0 ; i < size ; i += 4) {
    dst[i] = src[i];
  }
}

void lpc17_uart_init(int flow_control)
{
	// power on UART1 peripheral
  PCONP_BITBAND->PCONP_UART1 = 1;

	// 8N1, enable access to divisor latches
	UART1->LineControl = 0b10000011;

	// divisor: 11, fractional: 3/13. final baud: 115,411
	UART1->DivisorLatchLSB = 11;
	UART1->DivisorLatchMSB = 0;
	UART1->FractionalDivider = (3 << 0) | (13 << 4);

	// block access to divisor latches
	UART1->LineControl &= ~0b10000000;

	// enable auto RTS/CTS
	if (flow_control)
		UART1->ModemControl = 0b11000000;
	else
		UART1->ModemControl = 0;

	// enable FIFO and DMA TODO
	UART1->FifoControl = 0b1001;

	// set P0.15 as TXD1, with pullup
	PINSEL0  = (PINSEL0  & ~(0b11 << 30)) | (0b01 << 30);
	PINMODE0 = (PINMODE0 & ~(0b11 << 30)) | (0b00 << 30);

	// set P0.16 as RXD1, with pullup
	PINSEL1  = (PINSEL1  & ~(0b11 <<  0)) | (0b01 <<  0);
	PINMODE1 = (PINMODE1 & ~(0b11 <<  0)) | (0b00 <<  0);

	if (flow_control) {
		// set P0.17 as CTS1, no pullup/down
		PINSEL1  = (PINSEL1  & ~(0b11 <<  2)) | (0b01 <<  2);
		PINMODE1 = (PINMODE1 & ~(0b11 <<  2)) | (0b10 <<  2);

		// set P0.22 as RTS1, no pullup/down
		PINSEL1  = (PINSEL1  & ~(0b11 << 12)) | (0b01 << 12);
		PINMODE1 = (PINMODE1 & ~(0b11 << 12)) | (0b10 << 12);
	}
}

// synchronously write a string to debug UART
// does not start any DMA
void debug_write(const char *str)
{
	unsigned i;

	for (i = 0; str[i]; ++i) {
		while ((UART1->LineStatus & U1LSR_THRE) == 0)
			;
		UART1->TransmitHolding = str[i];
	}
}

static void debug_send_dma(size_t size)
{
	DMACC7SrcAddr = (uint32_t)debug_buffer;
	DMACC7DestAddr = (uint32_t)&U1THR;
	DMACC7LLI = 0;
	DMACC7Control =
			(size & 0xfff)   | // transfer size
			(0b000 << 12)    | // source burst: 1 byte
			(0b000 << 15)    | // dest burst: 1 byte
			DMACCxControl_SI | // source increment
			DMACCxControl_I  ; // terminal count interrupt enable
	DMACC7Config =
			(10 << 6)         | // UART1 TX
			(0b001 << 11)     | // memory to peripheral
			DMACCxConfig_IE   | // allow error interrupts
			DMACCxConfig_ITC  ; // allow terminal count interrupts

	DMACC7Config |= 1;
}

void debug_printf_dma(char *fmt, ...)
{
	va_list ap;
	void *ret;

	// TODO warn user?
	if (debug_dma_active)
		return;
	debug_dma_active = 1;

	va_start(ap, fmt);
	tfp_vsnprintf(debug_buffer, sizeof(debug_buffer) - 1, fmt, ap);
	va_end(ap);
	debug_buffer[sizeof(debug_buffer) - 1] = 0;

	size_t len = strlen(debug_buffer);
	debug_send_dma(len);
}

void debug_printf(char *fmt, ...)
{
	va_list ap;
	void *ret;

	va_start(ap, fmt);
	tfp_vsnprintf(debug_buffer, sizeof(debug_buffer) - 1, fmt, ap);
	va_end(ap);
	debug_buffer[sizeof(debug_buffer) - 1] = 0;

	size_t len = strlen(debug_buffer);
	debug_write(debug_buffer);
}
