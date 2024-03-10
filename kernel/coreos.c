#include "UBERTOOTH.h"
#include <stddef.h>
#include "coreos.h"

#include "cmsis_os2.h"


uint32_t          __TasksStack[OS_MAX_THREAD * TASKS_STACK_WSIZE] = {0};
uint32_t          __TasksPSP[OS_MAX_THREAD] = {0};
volatile uint32_t __systick_count = 0;                            // 2khZ systick counter
uint32_t          __svc_exc_return = 0;                           // EXC_RETURN use by SVC
uint32_t          __curr_task = 0;                                // Current task (init = 0) used by PendSV
uint32_t          __next_task = 1;                                // Next task (init = 1) used by PendSV
osKernelState_t   __kernelState = osKernelInactive;                                // Next task (init = 1) used by PendSV
// SysTick interruption frequency in Hz (100 hz)
const uint32_t          __osTickFrequency = 100UL;

void print_os_info(void)
{
 // debug_printf ("tasks functions pointer = 0:%p 1:%p 2:%p 3:%p\n", __TasksFun[0],__TasksFun[1],__TasksFun[2],__TasksFun[3]);
  debug_printf ("curr_task = %d\n", __curr_task);
  debug_printf ("next_task = %d\n", __next_task);
  debug_printf ("PSP #0 = %x\n", __TasksPSP[0]);
  debug_printf ("In-stack task0 function pointer = %p\n", HW32_REG((__TasksPSP[0] + (16<<2))));
  debug_printf ("PSP #1 = %x\n", __TasksPSP[1]);
  debug_printf ("PSP #2 = %x\n", __TasksPSP[2]);
  debug_printf ("PSP #3 = %x\n", __TasksPSP[3]);
}

void SysTick_Handler(void) // 1KHz
{
  // Increment systick counter for LED blinking
  __systick_count++;
  // Simple task round robin scheduler
  switch(__curr_task) {
    case(0): __next_task=1; break;
    case(1): __next_task=2; break;
    case(2): __next_task=0; break;
    //case(3): __next_task=0; break;
    default: __next_task=0;
      debug_printf("ERROR: curr=%x\n", __curr_task);
      break; // Should not be here
    }
  // Context switching needed
  if (__curr_task != __next_task) {
     SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
  }
  return;	
}


void SVC_Handler_C(unsigned int * svc_args)
{
  uint8_t svc_number;	
  svc_number = ((char *) svc_args[6])[-2]; // Memory[(Stacked PC)-2]
  debug_printf("===== SVC arg=%p number=%08x\n", svc_args, svc_number);
  switch(svc_number) {
    case (0): // SVC 0: OS init
      osKernelInitialize();

      // ========= Launch the task 0  ==========
      // Switch to task #0 (Current task)
      __curr_task = 0;
      // Return to thread with PSP
      __svc_exc_return = HW32_REG((__TasksPSP[__curr_task]));
      // Set PSP to @R0 of task 0 exception stack frame
      __set_PSP((__TasksPSP[__curr_task] + 10*4));
      // ========= Configure PendSV and SysTick =============
      // Set PendSV to lowest possible priority
      NVIC_SetPriority(PendSV_IRQn, 0xFF);
      // LPC17xx : 100MhZ core clock, set SysTick interrupt to 100000000 / 1000000 = 100hz
      SysTick_Config(100000000 / __osTickFrequency);
      // ========= Switch to use Process Stack, unprivileged state =========
      __set_CONTROL(0x3);                 
      // Execute ISB after changing CONTROL (architectural recommendation)
      __ISB();
      break;
    case (1): // SVC 1: system reset
      //wait(1);
      lpc17_reset();
      break;
    default:
      debug_printf ("ERROR: Unknown SVC service number");
      debug_printf("- SVC number 0x%x\n", svc_number);
      while (1);
      break;
  } // end switch
}	
