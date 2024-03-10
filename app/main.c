#include <string.h>
#include <stdint.h>

#include "UBERTOOTH.h"
#include "ubertooth_interface.h"
#include "cmsis_utils.h"
#include "lpc17_uart.h"
#include "cmsis_os2.h"
#include "ubertooth_usb.h" // TODO test only


/* build info */
const char compile_info[] =
	"ubertooth lpc17 cm3 " GIT_REVISION " (" COMPILE_BY "@" COMPILE_HOST ") " TIMESTAMP;

int ubertooth_usb_request_handler(uint8_t request, uint16_t* request_params, uint8_t* data, int* data_len);
void ubertooth_task_usb(void *arg);
extern osThreadAttr_t ubertooth_task_usb_attr;

/* RTOS compiler's environment do the systemInit and start kernel part before
** the call to main
** Here the compiler will just look for a symbole named os_main_thread
** os create the task for it and switch to it after
** the svc 0 call */
void  os_main_thread(void *arg)
{
  osThreadId_t usbid;

  usbid = osThreadNew(ubertooth_task_usb, NULL,
      (const osThreadAttr_t*)&ubertooth_task_usb_attr);
}

int main()
{
  SCB->CCR |= SCB_CCR_STKALIGN_Msk; // Enable double word stack alignment 
  //(recommended in Cortex-M3 r1p1, default in Cortex-M3 r2px and Cortex-M4)
  SystemInit();

  // Setup UART1
	lpc17_uart_init(0);
  // Setup USB
  ubertooth_usb_init(ubertooth_usb_request_handler);

  // Output some system info
  cm3_system_check();
  //DWT_cyccnt_test();
  //SysTick_test();

  debug_printf("PLL0STAT=%08x\n", PLL0STAT);
  // Setup LED output
  CONFIG_OUTPUT_LEDRED();
  CONFIG_OUTPUT_LEDORANGE();
  CONFIG_OUTPUT_LEDYELLOW();
  CONFIG_OUTPUT_0_17();

  debug_printf("Simple context switching test\n");
  osKernelInitialize();
  osKernelStart();

  // Should never reach here
  debug_printf("ERROR main: Should never reach here\n\n");
  while (1)
    ;

  return (1);
}
