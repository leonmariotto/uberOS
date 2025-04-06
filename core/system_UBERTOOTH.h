/**************************************************************************//**
 * @file     system_ARMCM3.h
 * @brief    CMSIS Cortex-M3 Device System Header File
 *           for CM3 Device Series
 * @version  V1.05
 * @date     19. July 2011
 *
 * @note
 * Copyright (C) 2010-2011 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M 
 * processor based microcontrollers.  This file can be freely distributed 
 * within development tools that are supporting such ARM based processors. 
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/


#ifndef SYSTEM_UBERTOOTH_H
#define SYSTEM_UBERTOOTH_H

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);

/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemCoreClock with current core Clock 
 *         retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);

/* in-system programming IAP/ISP */
typedef void (*IAP_ENTRY)(u32[], u32[]);
extern const IAP_ENTRY iap_entry;
void set_isp(void);

/* hardware identification number */
#define BOARD_ID_UBERTOOTH_ONE  1
#define BOARD_ID BOARD_ID_UBERTOOTH_ONE


static enum {
  PINMODE_PULLUP      = 0b00,
  PINMODE_REPEATER    = 0b01,
  PINMODE_NOPULL      = 0b10,
  PINMODE_PULLDOWN    = 0b11,
  PINMODE_OPENDRAIN   = 0b100
} PINMODE_enum_e;

#define PINSEL_GPIO_MODE 0b00

/* GPIO mode pins control
Using DECLARE_PIN_FUNCTION it generate for PIN :
CONFIGURE_INPUT_<pinfunction> configure the pin as GPIO with mode
CONFIGURE_OUTPUT_<pinfunction> configure the pin as GPIO with mode
SET_<pinfunction> set the pin
CLR_<pinfunction> clear the pin
GET_<pinfunction> read the pin
  @arg name: name of the pin
  @arg reg: register number of the pin (0 for PINSEL0)
  @arg pinpos: position of pin (17 for P0.17)
  @arg mode: one for enum PINMODE_enum_e 
*/
#define DECLARE_GPIO_PIN_FUNCTION(name, reg, bitpos, mode) \
  static inline void CONFIG_OUTPUT_## name() { \
	  LPC17_REG(0x4002C000 + ((reg + (bitpos>16?1:0)) * 0x04)) |= (PINSEL_GPIO_MODE << ((bitpos>16?bitpos-16:bitpos))); \
	  LPC17_REG(0x4002C040 + ((reg + (bitpos>16?1:0)) * 0x04)) |= (mode << ((bitpos>16?bitpos-16:bitpos))); \
	  LPC17_REG(0x2009C000 + (reg * 0x20)) |= (1 <<  bitpos); \
  }\
  static inline uint32_t GET_## name() \
  { return (LPC17_REG(0x2009C014 + (reg * 0x20)) & (1 << bitpos)); } \
  static inline void SET_## name() { (LPC17_REG(0x2009C018 + (reg * 0x20)) |= (1 << bitpos)); } \
  static inline void CLR_## name() { (LPC17_REG(0x2009C01C + (reg * 0x20)) |= (1 << bitpos)); }

DECLARE_GPIO_PIN_FUNCTION(LEDRED, 1, 1, PINMODE_PULLUP) /* P1.1 User Led */
DECLARE_GPIO_PIN_FUNCTION(LEDORANGE, 1, 4, PINMODE_PULLUP) /* P1.1 User Led */
DECLARE_GPIO_PIN_FUNCTION(LEDYELLOW, 1, 8, PINMODE_PULLUP) /* P1.1 User Led */

//DECLARE_GPIO_PIN_FUNCTION(0_16, 1, 16, PINMODE_PULLUP) /* P1.1 User Led */
DECLARE_GPIO_PIN_FUNCTION(0_17, 1, 2, PINMODE_PULLUP) /* P1.1 User Led */
////DECLARE_GPIO_PIN_FUNCTION(0_18, 1, 18, PINMODE_PULLUP) /* P1.1 User Led */

#define PIN_USRLED (1 << 1 ) /* P1.1  */
#define PIN_RXLED  (1 << 4 ) /* P1.4  */
#define PIN_TXLED  (1 << 8 ) /* P1.8  */
#define PIN_CC1V8  (1 << 9 ) /* P1.9  */
#define PIN_CC3V3  (1 << 14) /* P1.14 */
#define PIN_VBUS   (1 << 30) /* P1.30 */
#define PIN_RX     (1 << 15) /* P1.15 */
#define PIN_TX     (1 << 29) /* P4.29 */
#define PIN_CSN    (1 << 5 ) /* P2.5  */
#define PIN_SCLK   (1 << 4 ) /* P2.4  */
#define PIN_MOSI   (1 << 0 ) /* P2.0  */
#define PIN_MISO   (1 << 1 ) /* P2.1  */
#define PIN_GIO6   (1 << 2 ) /* P2.2  */
#define PIN_BTGR   (1 << 10) /* P1.10 */
#define PIN_SSEL1  (1 << 28) /* P4.28 */
#define PIN_PAEN   (1 << 7 ) /* P2.7 */
#define PIN_HGM    (1 << 8 ) /* P2.8 */
#define PIN_0_15   (1 << 15)
#define PIN_0_16   (1 << 16)
#define PIN_0_17   (1 << 17)
#define PIN_0_18   (1 << 18)
#define PIN_0_22   (1 << 22)

/* indicator LED control */
#define USRLED     (FIO1PIN & PIN_USRLED)
#define USRLED_SET (FIO1SET = PIN_USRLED)
#define USRLED_CLR (FIO1CLR = PIN_USRLED)
#define RXLED      (FIO1PIN & PIN_RXLED)
#define RXLED_SET  (FIO1SET = PIN_RXLED)
#define RXLED_CLR  (FIO1CLR = PIN_RXLED)
#define TXLED      (FIO1PIN & PIN_TXLED)
#define TXLED_SET  (FIO1SET = PIN_TXLED)
#define TXLED_CLR  (FIO1CLR = PIN_TXLED)

/* SSEL (SPI slave select) control for CC2400 DIO (un-buffered) serial */
#define DIO_SSEL_SET  (FIO4SET = PIN_SSEL1)
#define DIO_SSEL_CLR  (FIO4CLR = PIN_SSEL1)

/* 1V8 regulator control */
#define CC1V8      (FIO1PIN & PIN_CC1V8)
#define CC1V8_SET  (FIO1SET = PIN_CC1V8)
#define CC1V8_CLR  (FIO1CLR = PIN_CC1V8)

/* CC2400 control */
#ifdef UBERTOOTH_ONE
#define CC3V3_SET  (FIO1SET = PIN_CC3V3)
#define CC3V3_CLR  (FIO1CLR = PIN_CC3V3)
#define RX_SET     (FIO1SET = PIN_RX)
#define RX_CLR     (FIO1CLR = PIN_RX)
#define TX_SET     (FIO4SET = PIN_TX)
#define TX_CLR     (FIO4CLR = PIN_TX)
#define CSN_SET    (FIO2SET = PIN_CSN)
#define CSN_CLR    (FIO2CLR = PIN_CSN)
#define SCLK_SET   (FIO2SET = PIN_SCLK)
#define SCLK_CLR   (FIO2CLR = PIN_SCLK)
#define MOSI_SET   (FIO2SET = PIN_MOSI)
#define MOSI_CLR   (FIO2CLR = PIN_MOSI)
#define GIO6       (FIO2PIN & PIN_GIO6)
#define GIO6_SET   (FIO2SET = PIN_GIO6)
#define GIO6_CLR   (FIO2CLR = PIN_GIO6)
#define BTGR_SET   (FIO1SET = PIN_BTGR)
#define BTGR_CLR   (FIO1CLR = PIN_BTGR)
#define MISO       (FIO2PIN & PIN_MISO)
#endif

/*
 * DIO_SSP is the SSP assigned to the CC2400's secondary ("un-buffered") serial
 * interface
 */
#if defined UBERTOOTH_ONE || defined TC13BADGE
#define DIO_SSP_CR0   SSP1CR0
#define DIO_SSP_CR1   SSP1CR1
#define DIO_SSP_DR    SSP1DR
#define DIO_SSP_DMACR SSP1DMACR
#define DIO_SSP_SRC   (3 << 1) /* for DMACCxConfig register */
#endif

/* CC2591 control */
#ifdef UBERTOOTH_ONE
#define PAEN     (FIO2PIN & PIN_PAEN)
#define PAEN_SET (FIO2SET = PIN_PAEN)
#define PAEN_CLR (FIO2CLR = PIN_PAEN)
#define HGM      (FIO2PIN & PIN_HGM)
#define HGM_SET  (FIO2SET = PIN_HGM)
#define HGM_CLR  (FIO2CLR = PIN_HGM)
#endif

/* USB VBUS monitoring */
#define VBUS (FIO1PIN & PIN_VBUS)

/*
 * clock configuration
 *
 * main oscillator:  16 MHz (from CC2400)
 * CPU clock (PLL0): 100 MHz
 * USB clock (PLL1): 48 MHz
 *
 * The ToorCon 13 badge is configured with a 30 MHz CPU clock instead of 100
 * MHz to reduce heat at the voltage regulator.  This is a sufficient clock
 * speed for passive Bluetooth monitoring.
 */
#define MSEL0 24
#define NSEL0 1
#define CCLKSEL 3
#define MSEL1 34
#define PSEL1 0

/* flash accelerator configuration */
#define FLASHTIM 0x4 /* up to 100 MHz CPU clock */

/*
 * bootloader_ctrl is a fixed memory location used for passing
 * information from the application to the bootloader across a reset
 */
extern uint32_t bootloader_ctrl;
#define DFU_MODE 0x4305BB21

/* GPIO */
void ubertooth_gpio_init(void);
void ubertooth_gpio_reset(void);

/* ADC */
void adc_init(void);
void clock_start(void);

void r8c_takeover(void); // ???
void lpc17_reset(void);
void iap_get_part_num(uint8_t *buffer, int *len);
void iap_get_device_serial(uint8_t *buffer, int *len);

/* operating modes */
enum operating_modes {
	MODE_IDLE          = 0,
	MODE_RX_SYMBOLS    = 1,
	MODE_TX_SYMBOLS    = 2,
	MODE_TX_TEST       = 3,
	MODE_SPECAN        = 4,
	MODE_RANGE_TEST    = 5,
	MODE_REPEATER      = 6,
	MODE_LED_SPECAN    = 7,
	MODE_BT_FOLLOW     = 8,
	MODE_BT_FOLLOW_LE  = 9,
	MODE_BT_PROMISC_LE = 10,
	MODE_RESET         = 11,
	MODE_BT_SLAVE_LE   = 12,
	MODE_EGO           = 13,
	MODE_AFH           = 14,
	MODE_RX_GENERIC    = 15,
	MODE_TX_GENERIC    = 16,
	MODE_XMAS          = 17,
};

#ifdef __cplusplus
}
#endif


#endif /* SYSTEM_UBERTOOTH_H */

