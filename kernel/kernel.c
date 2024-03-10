//#include "UBERTOOTH.h"
#include "cmsis_os2.h"

extern osKernelState_t   __kernelState;
extern volatile uint32_t __systick_count;
extern const uint32_t          __osTickFrequency;

//  ==== Kernel Management Exported Functions ====
 
/// Initialize the RTOS Kernel.
/// called by syscall 0
/// \return status code that indicates the execution status of the function.
void      osInitDefaultTasks(void);
osStatus_t osKernelInitialize (void)
{
  osStatus_t r = osOK;
  osInitDefaultTasks();
  // All magic is done by the syscall 0
  return (r);
}
 
/// Get the current RTOS Kernel state.
/// \return current RTOS Kernel state.
osKernelState_t osKernelGetState (void)
{
  return __kernelState;
}
 
/// Start the RTOS Kernel scheduler.
/// This function use SVC0 and switch to task0 without returning
/// \return status code that indicates the execution status of the function.
osStatus_t osKernelStart (void)
{
  osStatus_t r = osOK;
  // Start OS with syscall 0
	asm("SVC #0");
  return (r);
}
 
/* ===== Kernel suspend/resume functions : osKernelResume should be called before osKernelSuspend to schedule
** a wakeup interruption with SysTick, otherwise kernel will never wakeup (if no other interrupt fire)
** Kernel switch to osKernelSuspended
** Run at the right clock frequency : 
** Select the right clock source : we're running with main oscillator (CLKSRCSEL=1, CCLKCFG=1, PLL0 and PLL1 are active) 
** Turn off unused clock signal
** Utilize clock system features
** Using the right I/O port configuration
*/

/// Suspend the RTOS Kernel scheduler.
/// \return time in ticks for how long the system was sleeping or power-down.
uint32_t osKernelSuspend (void)
{
  // TODO Low-Power features : decrease systick ? 
  // SCB configuration, clock speed, LPC17 clocks, GPIO? 
  // Low power USB state : SUSPENDED state
  __kernelState = osKernelSuspended;
  return (0UL);
}
 
/// Resume the RTOS Kernel scheduler.
/// \param[in]     sleep_ticks   time in ticks for how long the system can sleep or power-down mode.
void osKernelResume (uint32_t sleep_ticks)
{
  // TODO
  (void)sleep_ticks;
}
 
/// Protect the RTOS Kernel scheduler access.
/// \param[in]     safety_class  safety class.
/// \return status code that indicates the execution status of the function.
osStatus_t osKernelProtect (uint32_t safety_class)
{
  // TODO MPU configuration 
  (void)safety_class;
  return (osErrorResource);
}
 
/// Get the RTOS kernel tick count.
/// \return RTOS kernel current tick count.
uint32_t osKernelGetTickCount (void)
{
  return __systick_count;
}
 
/// Get the RTOS kernel tick frequency.
/// \return frequency of the kernel tick in hertz, i.e. kernel ticks per second.
uint32_t osKernelGetTickFreq (void)
{
  return (__osTickFrequency);
}
 
/// Get the RTOS kernel system timer count.
/// \return RTOS kernel current system timer count as 32-bit value.
uint32_t osKernelGetSysTimerCount (void)
{
  return __systick_count;
}
 
/// Get the RTOS kernel system timer frequency.
/// \return frequency of the system timer in hertz, i.e. timer ticks per second.
uint32_t osKernelGetSysTimerFreq (void)
{
  return (__osTickFrequency);
}
