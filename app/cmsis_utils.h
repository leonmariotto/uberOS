
#ifndef CMSIS_UTILS_H
# define CMSIS_UTILS_H

void CoreDebug_check(void);
void DWT_check(void);
void ITM_check(void);
void SysTick_check(void);
void NVIC_check(void);
void SCB_check(void);
void GPIO_check(void);
void TIMER_check(void);
void cm3_system_check();
void DWT_cyccnt_test();
void SysTick_test();


#endif
