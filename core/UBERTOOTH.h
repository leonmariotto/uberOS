
#ifndef MYUBEROOTH_H
# define MYUBERTOOTH_H

/*
This is the HAL for UBERTOOTH (not for LPC17)
Do not try to implement LPC17 logic
However this add function to manipulate LPC17
*/


/******************************************************************************/
/*            Interrupt Number Definition                                     */
/******************************************************************************/

typedef enum IRQn
{
  /******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn         = -14,    /*!<  2 Cortex-M3 Non Maskable Interrupt                  */
  HardFault_IRQn              = -13,    /*!<  3 Cortex-M3 Hard Fault Interrupt                    */
  MemoryManagement_IRQn       = -12,    /*!<  4 Cortex-M3 Memory Management Interrupt             */
  BusFault_IRQn               = -11,    /*!<  5 Cortex-M3 Bus Fault Interrupt                     */
  UsageFault_IRQn             = -10,    /*!<  6 Cortex-M3 Usage Fault Interrupt                   */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                       */
  DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt                 */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                       */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M3 System Tick Interrupt                   */

  /******  ARMCM3 specific Interrupt Numbers ********************************************************/
  GPIO_IRQn                   = 0       /*!< GPIO Interrupt                                       */
} IRQn_Type;

/******************************************************************************/
/*             Processor and Core Peripheral Section                          */
/******************************************************************************/

/* Configuration of the Cortex-M3 Processor and Core Peripherals */
#define __CM3_REV                 0x0201    /*!< Core Revision r2p1                      */
#define __MPU_PRESENT             1         /*!< MPU present or not                      */
#define __NVIC_PRIO_BITS          3         /*!< Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */

// Convert an address into its bit-band alias addr
#define BITBAND_OFFSET            0x02000000UL
#define BITBAND_SRAM_REF          0x20000000
#define BITBAND_SRAM_BASE         0x22000000
#define BITBAND_SRAM_ADDR(a)      (BITBAND_SRAM_BASE + (a-BITBAND_SRAM_REF)*32)

#define BITBAND_PERI_REF          0x40000000
#define BITBAND_PERI_BASE         0x42000000
#define BITBAND_PERI_ADDR(a)           (BITBAND_PERI_BASE + (a-BITBAND_PERI_REF)*32)

#define BIT_BAND(addr, bitnum)    ((((addr) & 0xF0000000) + 0x02000000 + (((addr) & 0x000FFFFF) << 5) + ((bitnum) << 2)))
#define BIT_BAND_SRAM(a,b) ((BITBAND_SRAM_BASE + (a-BITBAND_SRAM_REF)*32 + (b*4))) // Convert SRAM address

#define MEM_ADDR(addr) ((volatile unsigned long*)(addr))

#include <core_cm3.h>                       /* Cortex-M3 processor and core peripherals */
#include "ARMCM3.h"                         /* Some CM3 structs */


#include "types.h"
#include "lpc17.h"                          /* NXP LPC17 registers definitions */
#include "lpc17_uart.h"                          /* NXP LPC17 registers definitions */
#include "cc2400.h"                         /* cc2400 peripheral */

#include "system_UBERTOOTH.h"                  /* System Header */
#include "coreos.h"     /* coreos */

/* !!!!!!!!!!!!!! ===  Start of section using anonymous unions  == */
#if defined (__CC_ARM)
#pragma push
#pragma anon_unions
#elif defined (__ICCARM__)
#pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc11-extensions"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#elif defined (__GNUC__)
/* anonymous unions are enabled by default */
#elif defined (__TMS470__)
/* anonymous unions are enabled by default */
#elif defined (__TASKING__)
#pragma warning 586
#elif defined (__CSMC__)
/* anonymous unions are enabled by default */
#else
#warning Not supported compiler type
#endif

/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/

    /*
     ** APB0 peripheral  Base address    Peripheral name
     ** 0                0x40000000      Watchdog Timer
     ** 1                0x40004000      Timer 0
     ** 2                0x40008000      Timer 1
     ** 3                0x4000C000      UART0
     ** 4                0x40010000      UART1
     ** 5                0x40014000      reserved
     ** 6                0x40018000      PWM1
     ** 7                0x4001C000      I2C0
     ** 8                0x40020000      SPI
     ** 9                0x40024000      RTC
     ** 10               0x40028000      GPIO interrupts
     ** 11               0x4002C000      Pin Connect Block
     ** 12               0x40030000      SSP1
     ** 13               0x40034000      ADC
     ** 14               0x40038000      CAN Acceptance Filter RAM
     ** 15               0x4003C000      CAN Acceptance Filter Registers
     ** 16               0x40040000      CAN Common Registers
     ** 17               0x40044000      CAN Controller 1
     ** 18               0x40048000      CAN Controller 2
     ** 19 to 22         0x4004C000 to 0x40058000 reserved
     ** 23               0x4005C000      I2C1
     */

#define GPIO_BASE               0x2009C000UL
#define UART0_BASE              0x4000C000UL
#define PINCTRL_BASE            0x4002C000UL
#define GPIOINT_BASE            0x4002C000UL
#define WatchDog_BASE			      0x40000000UL      // Watchdog Timer
#define TIMER0_BASE			        0x40004000UL      // Timer 0
#define TIMER1_BASE			        0x40008000UL      // Timer 1
#define UART0_BASE			        0x4000C000UL      // UART0
#define UART1_BASE			        0x40010000UL      // UART1
#define PWM1_BASE			          0x40018000UL      // PWM1
#define I2C0_BASE			          0x4001C000UL      // I2C0
#define SPI_BASE			          0x40020000UL      // SPI
#define RTC_BASE			          0x40024000UL      // RTC
#define GPIOTIN_BASE			      0x40028000UL      // GPIO interrupts
#define SSP1_BASE			          0x40030000UL      // SSP1
#define ADC_BASE			          0x40034000UL      // ADC
#define I2C_BASE			          0x4005C000UL      // I2C1
#define PCONP_BASE              0x400FC0C4UL      // PCONP register bit-banding
#define PCLK0_BASE              0x400FC1A8UL      // PCLK register bit-banding
#define PCLK1_BASE              0x400FC1ACUL      // PCLK register bit-banding

/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/

#if 0
// TODO this is CM3 specific but constructor can add registers
typedef volatile struct {
  // 0xE000E008 ACTLR RW Privileged 0x00000000 Table 655
  // 0xE000ED10 SCR RW Privileged 0x00000000 Table 661
  // 0xE000ED14 CCR RW Privileged 0x00000200 Table 662
  // 0xE000ED18 SHPR1 RW Privileged 0x00000000 Table 664
  // 0xE000ED1C SHPR2 RW Privileged 0x00000000 Table 665
  // 0xE000ED20 SHPR3 RW Privileged 0x00000000 Table 666
  // 0xE000ED24 SHCRS RW Privileged 0x00000000 Table 667
  // 0xE000ED28 CFSR RW Privileged 0x00000000 Section 34.4.3.11
  // 0xE000ED28 MMSR [2] RW Privileged 0x00 Table 668
  // 0xE000ED29 BFSR [2] RW Privileged 0x00 Table 669
  // 0xE000ED2A UFSR [2] RW Privileged 0x0000 Table 670
  // 0xE000ED2C HFSR RW Privileged 0x00000000 Table 671
  // 0xE000ED34 MMFAR RW Privileged Undefined Table 672
  // 0xE000ED38 BFAR RW Privileged Undefined Table 673
  int MasterCtrl;
  int IntCtrlType;
  int zReserved008_00c[2];
  /* Reserved space */
  struct {
    int Ctrl;
    int Reload;
    int Value;
    int Calibration;
  } SysTick;
  int zReserved020_0fc[(0x100*0x20)/4];
  /* Reserved space */
  /* Offset 0x0100 Addr 0xE00E100*/
  struct { // 34.4.2    0xE000E100 
    int Enable[32];
    int Disable[32];
    int Set[32];
    int Clear[32];
    int Active[64];
    int Priority[64];
  } NVIC;
  int zReserved0x500_0xcfc[(0xd00*0x500)/4];
  /* Reserved space */
  // UM10360 Section 34.4.3
  /* Offset 0x0d00  Addr 0xE000ED00 */
  // 0xE000ED00 CPUID RO Privileged 0x412FC230 Table 656
  int CPUID;
  // 0xE000ED04 ICSR RW [1] Privileged 0x00000000 Table 657
  int IRQcontrolState; // ICSR
  // 0xE000ED08 VTOR RW Privileged 0x00000000 Table 658
  int ExceptionTableOffset; // VTOR + ACTLR
  // 0xE000ED0C AIRCR RW [1] Privileged 0xFA050000 Table 659
  int AIRC;
  int SysCtrl;
  int ConfigCtrl;
  int SystemPriority[3];
  int SystemHandlerCtrlAndState;
  int ConfigurableFaultStatus;
  int HardFaultStatus;
  int DebugFaultStatus;
  int MemManageAddress;
  int BusFaultAddress;
  int AuxFaultStatus;
  int zReserved0xd40_0xd90[(0xd90-0xd40)/4];
  /* Reserved space */
  /* Offset 0x0d90 */
  struct {
    int Type;
    int Ctrl;
    int RegionNumber;
    int RegionBaseAddr;
    int RegionAttrSize;
  } MPU;
} SCS_t;

#endif

/*
 ** UART Control Register (USARTx_CR1, USARTx_CR2, USARTx_CR3):
 ** Configures various aspects of UART operation (e.g., enabling,
 ** word length, stop bits, parity, etc.).
 ** UART Baud Rate Register (USARTx_BRR): Sets the baud rate for communication.
 ** UART Data Register (USARTx_DR): Holds the data to be transmitted or received.
 ** UART Status Register (USARTx_SR): Provides status information about ongoing
 ** operations (e.g., transmit complete, receive data ready).
 ** Before this i need to enable the peripheric with PCONP (power) and PCLKSEL0/1 (clock) 
 ** I also need to set the gpio TX + RX + flow control (CTS, RTS) + CLK
 ** set P0.15 as TXD1, with pullup
 ** set P0.16 as RXD1, with pullup
 ** set P0.17 as CTS1, no pullup/down
 ** set P0.22 as RTS1, no pullup/down
 */
typedef struct UART_s {
  union {
    __IOM   uint32_t ReceiverBuffer;
    __IOM   uint32_t TransmitHolding;
    __IOM   uint32_t DivisorLatchLSB;
  };
  union {
    __IOM   uint32_t DivisorLatchMSB;
    __IOM   uint32_t InterruptEnable;
  };
  union {
    __IOM   uint32_t InterruptId;
    __IOM   uint32_t FifoControl;
  };
  __IOM     uint32_t LineControl;
  __IOM     uint32_t ModemControl;
  __IOM     uint32_t LineStatus;
  __IOM     uint32_t ModemStatus;
  __IOM     uint32_t ScratchPad;
  __IOM     uint32_t AutoBaud;
  __IOM     uint32_t RESERVED0[1];
  __IOM     uint32_t FractionalDivider;
  __IOM     uint32_t TransmitEnable;
} UART_TypeDef;

/* PCONP register aliased structure to be accessed in bit-band alias memory for PCONP */
typedef struct PCONP_s {
  __IOM     uint32_t    RESERVED0;
  __IOM     uint32_t    PCONP_TIM0; // Timer/Counter 0 power/clock control bit. 1
  __IOM     uint32_t    PCONP_TIM1; // Timer/Counter 1 power/clock control bit. 1
  __IOM     uint32_t    PCONP_UART0; // UART0 power/clock control bit. 1
  __IOM     uint32_t    PCONP_UART1; // UART1 power/clock control bit. 1
  __IOM     uint32_t    RESERVED1;
  __IOM     uint32_t    PCPWM1; // PWM1 power/clock control bit. 1
  __IOM     uint32_t    PCONP_I2C0; // The I2C0 interface power/clock control bit. 1
  __IOM     uint32_t    PCONP_SPI; // The SPI interface power/clock control bit. 1
  __IOM     uint32_t    PCONP_RTC; // The RTC power/clock control bit. 1
  __IOM     uint32_t    PCONP_SSP1; // The SSP 1 interface power/clock control bit. 1
  __IOM     uint32_t    RESERVED2;
  __IOM     uint32_t    PCONP_ADC; // A/D converter (ADC) power/clock control bit.
  __IOM     uint32_t    PCONP_CAN1; // CAN Controller 1 power/clock control bit. 0
  __IOM     uint32_t    PCONP_CAN2; // CAN Controller 2 power/clock control bit. 0
  __IOM     uint32_t    PCONP_GPIO; // Power/clock ctrl for IOCON, GPIO, and GPIO interrupts. 1
  __IOM     uint32_t    PCONP_RIT; // Repetitive Interrupt Timer power/clock control bit. 0
  __IOM     uint32_t    PCONP_MCPWM; // Motor Control PWM 0
  __IOM     uint32_t    PCONP_QEI; // Quadrature Encoder Interface power/clock control bit. 0
  __IOM     uint32_t    PCONP_I2C1; // The I2C1 interface power/clock control bit. 1
  __IOM     uint32_t    RESERVED3;
  __IOM     uint32_t    PCONP_SSP0; // The SSP0 interface power/clock control bit. 1
  __IOM     uint32_t    PCONP_TIM2; // Timer 2 power/clock control bit. 0
  __IOM     uint32_t    PCONP_TIM3; // Timer 3 power/clock control bit. 0
  __IOM     uint32_t    PCONP_UART2; // UART 2 power/clock control bit. 0
  __IOM     uint32_t    PCONP_UART3; // UART 3 power/clock control bit. 0
  __IOM     uint32_t    PCONP_I2C2; // I2 C interface 2 power/clock control bit. 1
} PCONP_TypeDef;


/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/

// #define UART_PCONP MEM_ADDR(BIT_BAND(DEVICE_REG0, 4))
#define UART0                ((UART_TypeDef *) UART0_BASE)
#define UART1                ((UART_TypeDef *) UART1_BASE)
#define PinCtrl              ((PinCtrl_TypeDef *) MEM_ADDR(PINCTRL_BASE))
#define PCONP_BITBAND        ((PCONP_TypeDef *) MEM_ADDR(BITBAND_PERI_ADDR(PCONP_BASE)))

#define MailBox              ((MailBox_TypeDef *) MAILBOX_BASE)
#define MailBox_BitBand      ((MailBox_BitBand_TypeDef *) MEM_ADDR(BITBAND_SRAM_ADDR(MAILBOX_BASE)))

/* !!!!!!!!!!!!!! ===  End of section using anonymous unions  == */
#if defined (__CC_ARM)
#pragma pop
#elif defined (__ICCARM__)
/* leave anonymous unions enabled */
#elif (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic pop
#elif defined (__GNUC__)
/* anonymous unions are enabled by default */
#elif defined (__TMS470__)
/* anonymous unions are enabled by default */
#elif defined (__TASKING__)
#pragma warning restore
#elif defined (__CSMC__)
/* anonymous unions are enabled by default */
#endif

#endif
