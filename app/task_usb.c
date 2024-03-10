#include "UBERTOOTH.h"
#include <string.h>
#include <stdint.h>
#include "ubertooth_usb.h" // TODO test only
#include "cmsis_os2.h"

/* operation mode */
volatile uint8_t mode = MODE_IDLE;
volatile uint8_t requested_mode = MODE_IDLE;

int ubertooth_usb_request_handler(uint8_t request, uint16_t* request_params, uint8_t* data, int* data_len)
{
	uint32_t clock;
	size_t length; // string length
	usb_pkt_rx* p = NULL;
	uint16_t reg_val;
	uint8_t i;
	unsigned data_in_len = request_params[2];

	switch (request) {
	  case UBERTOOTH_RESET:
	  	requested_mode = MODE_RESET;
	  	break;
	  case UBERTOOTH_FLASH:
	  	bootloader_ctrl = DFU_MODE;
	  	requested_mode = MODE_RESET;
	  	break;
	  default:
	  	return 0;
  }
  return (1);
}

extern volatile uint32_t __systick_count;
extern volatile uint32_t clkn;



osThreadAttr_t ubertooth_task_usb_attr = {
  .name = "ubertooth_task_usb",
  .attr_bits = 0, // cmsis_os2.h line 205
  .cb_mem = NULL,
  .cb_size = 0,
  .stack_mem = NULL,
  .stack_size = 0,
  .priority = 0,
  .tz_module = 0,
  0
};

void ubertooth_task_usb(void *arg)
{
  (void)arg;
  while (1) {
    // TODO update clkn (Bluetooth clock = 312.5 us period (312.5MhZ))
    // Need an oscilloscope TODO that
    // debug_printf("\n[clkn=%ld]\n", clkn);
    // USB seems to work with clkn == 0 (clkn is used for timeout)
    handle_usb(clkn);
    if(requested_mode != mode) {
	    switch (requested_mode) {
        case MODE_RESET:
	        asm("SVC #1");
          break;
        default:
          break;
      }
    }
    if (__systick_count & 0x001) {SET_LEDRED();}
    else                      {CLR_LEDRED();}
  }
}


