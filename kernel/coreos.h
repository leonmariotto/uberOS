#ifndef COREOS_H
# define COREOS_H

#include "cmsis_os2.h"
#define TASKS_MAX 4 // Static number of thread, should take care of not overlap
#define OS_MAX_THREAD 4 // Static number of thread, should take care of not overlap
                        // the memory used for it
#define TASKS_STACK_SIZE 1024  // Static number of thread, should take care of not overlap
                              // the memory used for it
#define TASKS_STACK_WSIZE (TASKS_STACK_SIZE / 4)

#define HW32_REG(ADDRESS)  (*((volatile unsigned long  *)(ADDRESS)))
// Maximum of tasks runnin
// Stack for each task in bytes
#define INITIAL_xPSR        0x01000000UL
#define INITIAL_EXEC_RETURN 0xFFFFFFFDUL
#define INITIAL_CONTROL 0x3UL
#define STACK_OFFSET (18 << 4)
#define REG_PC_OFFSET (16 << 2)
#define REG_xPSR_OFFSET (17 << 2)
#define REG_EXC_RETURN_OFFSET (0)
#define REG_CONTROL_OFFSET (1 << 2)

typedef struct OS {
  __IOM osKernelState_t   __kernelState;
  __I   uint32_t          __osTickFrequency;
} OS_TypeDef;

typedef struct Tasks {
  __IOM uint32_t          PSP[OS_MAX_THREAD];
  __IOM osThreadFunc_t    funcs[OS_MAX_THREAD];
  __IOM uint32_t          __TasksStack[OS_MAX_THREAD * TASKS_STACK_WSIZE];     // Stacks memory pointers
} Tasks_TypeDef;


#endif
