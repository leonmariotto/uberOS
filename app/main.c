#include <string.h>
#include <stdint.h>

#include "UBERTOOTH.h"
#include "ubertooth_interface.h"
#include "cmsis_utils.h"
#include "lpc17_uart.h"
#include "cmsis_os2.h"
#include "ubertooth_usb.h"
#include "tasks.h"


/* build info */
const char compile_info[] =
	"ubertooth lpc17 cm3 " GIT_REVISION " (" COMPILE_BY "@" COMPILE_HOST ") " TIMESTAMP;

int ubertooth_usb_request_handler(uint8_t request, uint16_t* request_params, uint8_t* data, int* data_len);
void ubertooth_task_usb(void *arg);

extern volatile uint32_t __systick_count;

/* Some tasks definitions */
void task0(void *arg) // Toggle LED #0 every 10ms (100hz)
{
  while (1) {
  if (__systick_count & 0x001) {SET_LEDRED();}
  else                      {CLR_LEDRED();}
  };
}
void task1(void *arg) // Toggle LED #1 every 100ms (10hz)
{
  while (1) {
  // debug_printf("[%s]\n", __func__);
  if ((__systick_count / 10) & 0x01) {SET_LEDORANGE();}
  else                      {CLR_LEDORANGE();}
  };
}
void task2(void *arg) // Toggle LED #2 every second (1hz)
{
  while (1) {
  if ((__systick_count / 100) & 0x0001) {SET_LEDYELLOW(); SET_0_17();}
  else                      {CLR_LEDYELLOW();CLR_0_17();}
  };
}
void task3(void *arg)
{
  while (1) {
    ;
  }
}	

uint32_t argusb = 42;

void      appInitDefaultTasks(void)
{
  int id = osTaskNew(ubertooth_task_usb, &argusb, NULL);
  debug_printf("Task %d launched\n", (int)id);
  id = osTaskNew(task1, NULL, NULL);
  debug_printf("Task %d launched\n", (int)id);
  id = osTaskNew(task2, NULL, NULL);
  debug_printf("Task %d launched\n", (int)id);
}

/* Note some RTOS compiler's environment do the systemInit and start kernel part before
** the call to main
** Not the case here. */
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

  // Setup LED output
  CONFIG_OUTPUT_LEDRED();
  CONFIG_OUTPUT_LEDORANGE();
  CONFIG_OUTPUT_LEDYELLOW();
  CONFIG_OUTPUT_0_17();

  debug_printf("Simple context switching test\n");
  osKernelInitialize();
  appInitDefaultTasks();
  osKernelStart(); // call SVC 0 and switch to task

  // Should never reach here
  debug_printf("ERROR main: Should never reach here\n\n");
  while (1)
    ;

  return (1);
}
