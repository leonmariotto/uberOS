
#include "UBERTOOTH.h"
#include <stddef.h>
#include "cmsis_os2.h"

extern volatile uint32_t __systick_count;
extern uint32_t          __TasksStack[OS_MAX_THREAD * TASKS_STACK_WSIZE];
extern uint32_t          __TasksPSP[OS_MAX_THREAD];
extern uint32_t          __curr_task;
extern uint32_t          __next_task;
uint8_t                  __ThreadStatus[OS_MAX_THREAD] = {0};

#define THREAD_ENABLED    0x1
#define THREAD_SUSPENDED  0x2

static int get_first_thread_available(void)
{
  for (int i = 0 ; i < OS_MAX_THREAD; i++) {
    if (((__ThreadStatus[i] & THREAD_ENABLED)) == 0)
      return (i);
  }
  debug_printf("ERROR: no thread available\n");
  return (-1); // no thread available
}

/* Initialize a thread */
int osTaskNew (osThreadFunc_t func, void *argument, const osThreadAttr_t *attr)
{
  int idx = get_first_thread_available();
  if (idx >= 0)
  {
    __TasksPSP[idx] = ((unsigned int) &__TasksStack[idx * TASKS_STACK_WSIZE]) + TASKS_STACK_SIZE - STACK_OFFSET;
    HW32_REG(__TasksPSP[idx] + REG_PC_OFFSET) = (uint32_t)func;
    HW32_REG(__TasksPSP[idx] + REG_xPSR_OFFSET) = INITIAL_xPSR;
    HW32_REG(__TasksPSP[idx] + REG_EXC_RETURN_OFFSET) = INITIAL_EXEC_RETURN;
    HW32_REG(__TasksPSP[idx] + REG_CONTROL_OFFSET) = INITIAL_CONTROL;
    // argument is set to R0
    HW32_REG(__TasksPSP[idx] + REG_R0_OFFSET) = (uint32_t)argument;
    __ThreadStatus[idx] = THREAD_ENABLED;
  }
  return (idx);
}

