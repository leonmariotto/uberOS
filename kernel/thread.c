#include "UBERTOOTH.h"
#include "cmsis_os2.h"

extern uint32_t          __TasksStack[OS_MAX_THREAD * TASKS_STACK_WSIZE];
extern uint32_t          __TasksPSP[TASKS_MAX];
extern uint32_t          __curr_task;
extern uint32_t          __next_task;
uint8_t                  __ThreadStatus[OS_MAX_THREAD] = {0};

#define THREAD_ENABLED 1
#define THREAD_SUSPENDED 2

//  ==== Thread Management Functions ====

 
/// Create a thread and add it to Active Threads.
/// \param[in]     func          thread function.
/// \param[in]     argument      pointer that is passed to the thread function as start argument.
/// \param[in]     attr          thread attributes; NULL: default values.
/// \return thread ID for reference by other functions or NULL in case of error.
osThreadId_t osThreadNew (osThreadFunc_t func, void *argument, const osThreadAttr_t *attr)
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
 
/// Get MPU protected zone of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return MPU protected zone of the specified thread. TODO
uint32_t osThreadGetZone (osThreadId_t thread_id);
 
/// Return the thread ID of the current running thread.
/// \return thread ID for reference by other functions or NULL in case of error.
osThreadId_t osThreadGetId (void)
{
  return (__curent_task);
}
 
/// Get current thread state of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return current thread state of the specified thread.
osThreadState_t osThreadGetState (osThreadId_t thread_id)
{
  return (__ThreadStatus[thread_id]);
}
 
/// Get stack size of a thread. (GET CURRENT STACK SPACE USED)
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return stack size in bytes.
uint32_t osThreadGetStackSize (osThreadId_t thread_id)
{
  return ((__TasksPSP[thread_id] - (uintptr_t)__TasksStack[thread_id]));
}
 
/// Get available stack space of a thread based on stack watermark recording during execution.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return remaining stack space in bytes.
uint32_t osThreadGetStackSpace (osThreadId_t thread_id)
{
  return (TASKS_STACK_SIZE - (__TasksPSP[thread_id] - (uintptr_t)__TasksStack[thread_id]));
}
 
/// Pass control to next thread that is in state \b READY.
// TODO what's scheduling scheme is that ?
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadYield (void)
{
  return (-1);
}
 
/// Suspend execution of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadSuspend (osThreadId_t thread_id)
{
  __ThreadStatus[thread_id] |= THREAD_SUSPENDED;
}
 
/// Resume execution of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadResume (osThreadId_t thread_id);
{
  __ThreadStatus[thread_id] &= ~THREAD_SUSPENDED;
}
 
/// Detach a thread (thread storage can be reclaimed when thread terminates).
// TODO detach / join POSIX ???
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadDetach (osThreadId_t thread_id);
 
/// Wait for specified thread to terminate.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadJoin (osThreadId_t thread_id);
 
/// Terminate execution of current running thread.
__NO_RETURN void osThreadExit (void);
 
/// Terminate execution of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadTerminate (osThreadId_t thread_id);
 
/// Feed watchdog of the current running thread.
/// \param[in]     ticks         \ref kernelTimer "time ticks" value until the thread watchdog expires, or 0 to stop the watchdog
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadFeedWatchdog (uint32_t ticks);
 
/// Protect creation of privileged threads.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadProtectPrivileged (void);
 
/// Suspend execution of threads for specified safety classes.
/// \param[in]     safety_class  safety class.
/// \param[in]     mode          safety mode.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadSuspendClass (uint32_t safety_class, uint32_t mode);
 
/// Resume execution of threads for specified safety classes.
/// \param[in]     safety_class  safety class.
/// \param[in]     mode          safety mode.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadResumeClass (uint32_t safety_class, uint32_t mode);
 
/// Terminate execution of threads assigned to a specified MPU protected zone.
/// \param[in]     zone          MPU protected zone.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadTerminateZone (uint32_t zone);
 
/// Get number of active threads.
/// \return number of active threads.
uint32_t osThreadGetCount (void);
 
/// Enumerate active threads.
/// \param[out]    thread_array  pointer to array for retrieving thread IDs.
/// \param[in]     array_items   maximum number of items in array for retrieving thread IDs.
/// \return number of enumerated threads.
uint32_t osThreadEnumerate (osThreadId_t *thread_array, uint32_t array_items);
 
 
//  ==== Thread Flags Functions ====
 
/// Set the specified Thread Flags of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \param[in]     flags         specifies the flags of the thread that shall be set.
/// \return thread flags after setting or error code if highest bit set.
uint32_t osThreadFlagsSet (osThreadId_t thread_id, uint32_t flags);
 
/// Clear the specified Thread Flags of current running thread.
/// \param[in]     flags         specifies the flags of the thread that shall be cleared.
/// \return thread flags before clearing or error code if highest bit set.
uint32_t osThreadFlagsClear (uint32_t flags);
 
/// Get the current Thread Flags of current running thread.
/// \return current thread flags.
uint32_t osThreadFlagsGet (void);
 
/// Wait for one or more Thread Flags of the current running thread to become signaled.
/// \param[in]     flags         specifies the flags to wait for.
/// \param[in]     options       specifies flags options (osFlagsXxxx).
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return thread flags before clearing or error code if highest bit set.
uint32_t osThreadFlagsWait (uint32_t flags, uint32_t options, uint32_t timeout);

/// Change priority of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \param[in]     priority      new priority value for the thread function.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadSetPriority (osThreadId_t thread_id, osPriority_t priority);
 
/// Get current priority of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return current priority value of the specified thread.
osPriority_t osThreadGetPriority (osThreadId_t thread_id);
 
