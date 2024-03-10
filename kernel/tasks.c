
#include "UBERTOOTH.h"
#include <stddef.h>
#include "cmsis_os2.h"

extern volatile uint32_t __systick_count;
extern uint32_t          __TasksStack[OS_MAX_THREAD * TASKS_STACK_WSIZE];
extern uint32_t          __TasksPSP[TASKS_MAX];
extern uint32_t          __curr_task;
extern uint32_t          __next_task;
uint8_t                  __ThreadStatus[OS_MAX_THREAD] = {0};

#define THREAD_ENABLED 1
#define THREAD_SUSPENDED 2

void task0(void *arg); // Toggle LED #0
void task1(void *arg); // Toggle LED #0
void task2(void *arg); // Toggle LED #0
void task3(void *arg); // Toggle LED #0
//extern volatile uint32_t clkn;
////extern volatile uint8_t mode;
//extern volatile uint8_t requested_mode;
void              (*__TasksFun[TASKS_MAX])(void) = {              // Tasks functions pointers
  task0,
  task1,
  task2,
  task3
};

/*
void      osInitTask(int idx, void (*f)(void))
{
  __TasksPSP[idx] = ((unsigned int) &__TasksStack[idx * TASKS_STACK_WSIZE]) + TASKS_STACK_SIZE - STACK_OFFSET;
  HW32_REG(__TasksPSP[idx] + REG_PC_OFFSET) = (unsigned int)(f == NULL ? __TasksFun[idx] : f);
  HW32_REG(__TasksPSP[idx] + REG_xPSR_OFFSET) = INITIAL_xPSR;
  HW32_REG(__TasksPSP[idx] + REG_EXC_RETURN_OFFSET) = INITIAL_EXEC_RETURN;
  HW32_REG(__TasksPSP[idx] + REG_CONTROL_OFFSET) = INITIAL_CONTROL;
}
*/
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

void ubertooth_task_usb(void *arg);

static int get_first_thread_available(void)
{
  for (int i = 0 ; i < OS_MAX_THREAD; i++) {
    if (((__ThreadStatus[i] | THREAD_ENABLED)) == 0)
      return (i);
  }
  return (-1); // no thread available
}

int osTaskNew (osThreadFunc_t func, void *argument, const osThreadAttr_t *attr)
{
  int idx = get_first_thread_available();
  if (idx >= 0)
  {
    __TasksPSP[idx] = ((unsigned int) &__TasksStack[idx * TASKS_STACK_WSIZE]) + TASKS_STACK_SIZE - STACK_OFFSET;
    HW32_REG(__TasksPSP[idx] + REG_PC_OFFSET) = func;
    HW32_REG(__TasksPSP[idx] + REG_xPSR_OFFSET) = INITIAL_xPSR;
    HW32_REG(__TasksPSP[idx] + REG_EXC_RETURN_OFFSET) = INITIAL_EXEC_RETURN;
    HW32_REG(__TasksPSP[idx] + REG_CONTROL_OFFSET) = INITIAL_CONTROL;
  }
  return (idx);
}

void      osInitDefaultTasks(void)
{
  int id = osTaskNew(ubertooth_task_usb, NULL, NULL);
  debug_printf("Task %d launched\n", (int)id);
  id = osTaskNew(task0, NULL, NULL);
  debug_printf("Task %d launched\n", (int)id);
  id = osTaskNew(task1, NULL, NULL);
  debug_printf("Task %d launched\n", (int)id);
}
