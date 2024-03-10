#include "UBERTOOTH.h"
#include <string.h>

void CoreDebug_check(void)
{
  debug_printf("CoreDebug_base ===============\n");
  debug_printf("DHCSR; 0x000 %08x Debug Halting Control and Status Register \n",     CoreDebug->DHCSR);
  debug_printf("DCRSR; 0x004 %08x Debug Core Register Selector Register \n",          CoreDebug->DCRSR);
  debug_printf("DCRDR; 0x008 %08x Debug Core Register Data Register \n",             CoreDebug->DCRDR);
  debug_printf("DEMCR; 0x00C %08x Debug Exception and Monitor Control Register \n",  CoreDebug->DEMCR);
  debug_printf("====================\n");
  /*!< DHCSR; 0x000 (R/W)  Debug Halting Control and Status Register */
  /*!< DCRSR; 0x004 (/W)  Debug Core Register Selector Register */
  /*!< DCRDR; 0x008 (R/W)  Debug Core Register Data Register */
  /*!< DEMCR; 0x00C (R/W)  Debug Exception and Monitor Control Register */
}

void DWT_check(void)
{
  debug_printf("DWT ===============\n");
  debug_printf("CTRL;        0x000 %08x  Control Register\n",                DWT->CTRL);
  debug_printf("CYCCNT;      0x004 %08x  Cycle Count Register\n",            DWT->CYCCNT);
  debug_printf("CPICNT;      0x008 %08x  CPI Count Register\n",              DWT->CPICNT);
  debug_printf("EXCCNT;      0x00C %08x  Exception Overhead Count Register\n", DWT->EXCCNT);
  debug_printf("SLEEPCNT;    0x010 %08x  Sleep Count Register\n",            DWT->SLEEPCNT);
  debug_printf("LSUCNT;      0x014 %08x  LSU Count Register\n",              DWT->LSUCNT);
  debug_printf("FOLDCNT;     0x018 %08x  Folded-instruction Count Register\n", DWT->FOLDCNT);
  debug_printf("PCSR;        0x01C %08x  Program Counter Sample Register\n",   DWT->PCSR);
  debug_printf("COMP0;       0x020 %08x  Comparator Register 0\n",           DWT->COMP0);
  debug_printf("MASK0;       0x024 %08x  Mask Register 0\n",                 DWT->MASK0);
  debug_printf("FUNCTION0;   0x028 %08x  Function Register 0\n",             DWT->FUNCTION0);
  debug_printf("COMP1;       0x030 %08x  Comparator Register 1\n",           DWT->COMP1);
  debug_printf("MASK1;       0x034 %08x  Mask Register 1\n",                 DWT->MASK1);
  debug_printf("FUNCTION1;   0x038 %08x  Function Register 1\n",             DWT->FUNCTION1);
  debug_printf("COMP2;       0x040 %08x  Comparator Register 2\n",           DWT->COMP2);
  debug_printf("MASK2;       0x044 %08x  Mask Register 2\n",                 DWT->MASK2);
  debug_printf("FUNCTION2;   0x048 %08x  Function Register 2\n",             DWT->FUNCTION2);
  debug_printf("COMP3;       0x050 %08x  Comparator Register 3\n",           DWT->COMP3);
  debug_printf("MASK3;       0x054 %08x  Mask Register 3\n",                 DWT->MASK3);
  debug_printf("FUNCTION3;   0x058 %08x  Function Register 3\n",             DWT->FUNCTION3);
  debug_printf("===============\n");
}

void ITM_check(void)
{
  debug_printf("ITM ===============\n");
  debug_printf("TER; 0xE00 %08x  ITM Trace Enable Register\n",    ITM->TER);
  debug_printf("TPR; 0xE40 %08x  ITM Trace Privilege Register\n", ITM->TPR);
  debug_printf("TCR; 0xE80 %08x  ITM Trace Control Register\n",   ITM->TCR);
  debug_printf("LSR; 0xFB4 %08x  ITM Lock Status Register\n",     ITM->LSR);
  debug_printf("===============\n");
  /*!< PORT [32U];    Offset: 0x000 ( /W)  ITM Stimulus Port Registers */
  /*!< TER;     Offset: 0xE00 (R/W)  ITM Trace Enable Register */
  /*!< TPR;     Offset: 0xE40 (R/W)  ITM Trace Privilege Register */
  /*!< TCR;     Offset: 0xE80 (R/W)  ITM Trace Control Register */
  /*!< LAR;     Offset: 0xFB0 ( /W)  ITM Lock Access Register */
  /*!< LSR;     Offset: 0xFB4 (R/ )  ITM Lock Status Register */
  /*!< PID[X];    Offset: 0xFD0 (R/ )  ITM Peripheral Identification Register #X */
  /*!< CID[X];    Offset: 0xFF0 (R/ )  ITM Component  Identification Register #X */
}

void SysTick_check(void)
{
  debug_printf("SysTick ===============\n");
  debug_printf("CTRL;    0x000 %08x  SysTick Control and Status Register\n",  SysTick->CTRL);
  debug_printf("LOAD;    0x004 %08x  SysTick Reload Value Register\n",        SysTick->LOAD);
  debug_printf("VAL;     0x008 %08x  SysTick Current Value Register\n",       SysTick->CALIB);
  debug_printf("CALIB;   0x00C %08x  SysTick Calibration Register\n",         SysTick->VAL);
  debug_printf("====================\n");
  /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
  /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register */
  /*!< Offset: 0x008 (R/W)  SysTick Current Value Register */
  /*!< Offset: 0x00C (R/ )  SysTick Calibration Register */
}

void NVIC_check(void)
{
  debug_printf("NVIC ===============\n");
  debug_printf("ISER[8U]; %08x %08x %08x %08x %08x %08x %08x %08x %08x Offset: 0x000 (R/W)  Interrupt Set Enable Register \n",
      NVIC->ISER[0],
      NVIC->ISER[1],
      NVIC->ISER[2],
      NVIC->ISER[3],
      NVIC->ISER[4],
      NVIC->ISER[5],
      NVIC->ISER[6],
      NVIC->ISER[7]);
  debug_printf("ICER[8U]; %08x %08x %08x %08x %08x %08x %08x %08x %08x Offset: 0x080 (R/W)  Interrupt Clear Enable Register \n",
      NVIC->ICER[0],
      NVIC->ICER[1],
      NVIC->ICER[2],
      NVIC->ICER[3],
      NVIC->ICER[4],
      NVIC->ICER[5],
      NVIC->ICER[6],
      NVIC->ICER[7]);
  debug_printf("ISPR[8U]; %08x %08x %08x %08x %08x %08x %08x %08x %08x Offset: 0x100 (R/W)  Interrupt Set Pending Register \n",
      NVIC->ISPR[0],
      NVIC->ISPR[1],
      NVIC->ISPR[2],
      NVIC->ISPR[3],
      NVIC->ISPR[4],
      NVIC->ISPR[5],
      NVIC->ISPR[6],
      NVIC->ISPR[7]);
  debug_printf("ICPR[8U]; %08x %08x %08x %08x %08x %08x %08x %08x %08x Offset: 0x180 (R/W)  Interrupt Clear Pending Register \n",
      NVIC->ICPR[0],
      NVIC->ICPR[1],
      NVIC->ICPR[2],
      NVIC->ICPR[3],
      NVIC->ICPR[4],
      NVIC->ICPR[5],
      NVIC->ICPR[6],
      NVIC->ICPR[7]);
  debug_printf("IABR[8U]; %08x %08x %08x %08x %08x %08x %08x %08x %08x Offset: 0x200 (R/W)  Interrupt Active bit Register \n",
      NVIC->IABR[0],
      NVIC->IABR[1],
      NVIC->IABR[2],
      NVIC->IABR[3],
      NVIC->IABR[4],
      NVIC->IABR[5],
      NVIC->IABR[6],
      NVIC->IABR[7]);
  debug_printf("===============\n");
}

void SCB_check(void)
{
  debug_printf("SCB ===============\n");
  debug_printf("CPUID;    0x000 %08x (R/ )  CPUID Base Register \n", SCB->CPUID);
  debug_printf("ICSR;     0x004 %08x (R/W)  Interrupt Control and State Register \n", SCB->ICSR);
  debug_printf("VTOR;     0x008 %08x (R/W)  Vector Table Offset Register \n", SCB->VTOR);
  debug_printf("AIRCR;    0x00C %08x (R/W)  Application Interrupt and Reset Control Register \n", SCB->AIRCR);
  debug_printf("SCR;      0x010 %08x (R/W)  System Control Register \n", SCB->SCR);
  debug_printf("CCR;      0x014 %08x (R/W)  Configuration Control Register \n", SCB->CCR);
  debug_printf("SHP[12U]; 0x018 %08x %08x ... (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) \n", SCB->SHP[0],SCB-> SHP[1]);
  debug_printf("SHCSR;    0x024 %08x (R/W)  System Handler Control and State Register \n", SCB->SHCSR);
  debug_printf("CFSR;     0x028 %08x (R/W)  Configurable Fault Status Register \n", SCB->CFSR);
  debug_printf("HFSR;     0x02C %08x (R/W)  HardFault Status Register \n", SCB->HFSR);
  debug_printf("DFSR;     0x030 %08x (R/W)  Debug Fault Status Register \n", SCB->DFSR);
  debug_printf("MMFAR;    0x034 %08x (R/W)  MemManage Fault Address Register \n", SCB->MMFAR);
  debug_printf("BFAR;     0x038 %08x (R/W)  BusFault Address Register \n", SCB->BFAR);
  debug_printf("AFSR;     0x03C %08x (R/W)  Auxiliary Fault Status Register \n", SCB->AFSR);
  debug_printf("PFR[2U];  0x040 %08x %08x (R/ )  Processor Feature Register \n", SCB->PFR[0], SCB->PFR[1]);
  debug_printf("DFR;      0x048 %08x (R/ )  Debug Feature Register \n", SCB->DFR);
  debug_printf("ADR;      0x04C %08x (R/ )  Auxiliary Feature Register \n", SCB->ADR);
  debug_printf("MMFR[4U]; 0x050 %08x %08x ... (R/ )  Memory Model Feature Register \n", SCB->MMFR[0], SCB->MMFR[1]);
  debug_printf("ISAR[5U]; 0x060 %08x %08x ... (R/ )  Instruction Set Attributes Register \n", SCB->ISAR[0], SCB->ISAR[1]);
  debug_printf("CPACR;    0x088 %08x (R/W)  Coprocessor Access Control Register \n", SCB->CPACR);
  debug_printf("===============\n");
}

void cm3_system_check()
{
  CoreDebug_check();
  ITM_check();
  DWT_check();
  //SysTick_check();
  NVIC_check();
  SCB_check();
  // TIMER_check();
}

void DWT_cyccnt_test()
{
  uint32_t t1, t2;
  t1 = DWT->CYCCNT;
  volatile int i = 0;
  while (i < 1000)
    i++;
  t2 = DWT->CYCCNT;
  debug_printf("%s: t1=%d t2=%d diff=%d\n", __func__, t1, t2, t2 - t1);
}

void SysTick_test()
{
  uint32_t t1, t2;
  t1 = SysTick->VAL;
  volatile int i = 0;
  while (i < 1000)
    i++;
  t2 = SysTick->VAL;
  debug_printf("%s: t1=%d t2=%d diff=%d\n", __func__, t1, t2, t1 - t2); // SysTick grows downward
}

//static void GPIOX_check(ARM_GPIO_TypeDef *GPIO, char *name)
//{
//  debug_printf("ARM_GPIO %s IE=%08x DATA :\n", name, GPIO->IE);
//  for (int i = 0 ; i < 256; i+=8) {
//    debug_printf("(%s)[%08x]: %08x %08x %08x %08x %08x %08x\n",
//      name,
//      &((uint32_t*)GPIO->DATA)[i],
//      ((uint32_t*)GPIO->DATA)[i+0],
//      ((uint32_t*)GPIO->DATA)[i+1],
//      ((uint32_t*)GPIO->DATA)[i+2],
//      ((uint32_t*)GPIO->DATA)[i+3],
//      ((uint32_t*)GPIO->DATA)[i+4],
//      ((uint32_t*)GPIO->DATA)[i+5],
//      ((uint32_t*)GPIO->DATA)[i+6],
//      ((uint32_t*)GPIO->DATA)[i+7]);
//  }
//}
//void GPIO_check(void)
//{
//  debug_printf("GPIO ===============\n");
//  GPIOX_check(ARM_GPIO0, "GPIO0");
//  debug_printf("%s:%d\n",__func__, __LINE__);             
//  GPIOX_check(ARM_GPIO1, "GPIO1");
//  debug_printf("%s:%d\n",__func__, __LINE__);             
//  GPIOX_check(ARM_GPIO2, "GPIO2");
//  debug_printf("%s:%d\n",__func__, __LINE__);             
//  debug_printf("===============\n");
//}
//
// Looks like when timer is runnin reading registers cause an bus fault
// I don't find any reference to this behavior in manual (yet)
//void TIMERX_check(TIMER0_Type *TIMER, char *name, uint32_t a, uint32_t b,uint32_t c,uint32_t d)
//{
//  char buf[128] = {0}; // want this buffer on the real stack
//  
//  if (a) // avoid compiler opti
//    memset(buf, 0xF0, 100);
//  debug_printf("%s:%d args:TIMER=%p name=%p\n",__func__, __LINE__, TIMER, name);             
//  debug_printf("%s:%d buf=%p\n",__func__, __LINE__, buf);             
//  debug_printf("TIMER [%s] %p ===============\n", name, TIMER);
//
//  //debug_printf("DEBUG ptr IR=%08x\n",              &TIMER->IR);             
//  // debug_printf("IR=%08x\n",              TIMER->IR);             
//  debug_printf("TCR=%08x\n",             TIMER->TCR);            
//  debug_printf("TC=%08x\n",              TIMER->TC);             
//  debug_printf("PRESCALE=%08x\n",        TIMER->PRESCALE);       
//  debug_printf("PRESCALE_COUNT=%08x\n",  TIMER->PRESCALE_COUNT); 
//  debug_printf("MCR=%08x\n",            TIMER->MCR);           
//  debug_printf("MR0=%08x\n",            TIMER->MR0);           
//  debug_printf("MR1=%08x\n",            TIMER->MR1);           
//  debug_printf("MR2=%08x\n",            TIMER->MR2);           
//  debug_printf("MR3=%08x\n",            TIMER->MR3);           
//  debug_printf("CCR=%08x\n",             TIMER->CCR);            
//  debug_printf("CR0=%08x\n",             TIMER->CR0);            
//  debug_printf("CR1=%08x\n",             TIMER->CR1);            
//  debug_printf("EMR=%08x\n",             TIMER->EMR);            
//  debug_printf("CTCR=%08x\n",            TIMER->CTCR);           
//
//}
//void TIMER_check(void)
//{
//  debug_printf("TIMER ===============\n");
//  TIMERX_check(TIMER0, "TIMER0", 0xA, 0xB, 0xC, 0xD);
//  TIMERX_check(TIMER1, "TIMER0", 0xA, 0xB, 0xC, 0xD);
//  TIMERX_check(TIMER2, "TIMER0", 0xA, 0xB, 0xC, 0xD);
//  TIMERX_check(TIMER3, "TIMER0", 0xA, 0xB, 0xC, 0xD);
//  debug_printf("===============\n");
//}
