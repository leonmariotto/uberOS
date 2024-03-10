#include "UBERTOOTH.h"
#include <string.h>
#include "os_utils.h"
#include "system_UBERTOOTH.h"

uint32_t  SystemCoreClock = 1000000;

/* =========== GPIO pin configuration */
/* This should be called very early by every firmware
** in order to ensure safe
 * operating conditions for the CC2400.
** Initial configuration pin, relevant pin are :
** PIN_USRLED (1 << 1 ) P1.1 
** PIN_RXLED  (1 << 4 ) P1.4 
** PIN_TXLED  (1 << 8 ) P1.8 
** PIN_CC1V8  (1 << 9 ) P1.9 
** PIN_CC3V3  (1 << 14) P1.14
** PIN_VBUS   (1 << 30) P1.30
** PIN_RX     (1 << 15) P1.15
** PIN_TX     (1 << 29) P4.29
** PIN_CSN    (1 << 5 ) P2.5 
** PIN_SCLK   (1 << 4 ) P2.4 
** PIN_MOSI   (1 << 0 ) P2.0 
** PIN_MISO   (1 << 1 ) P2.1 
** PIN_GIO6   (1 << 2 ) P2.2 
** PIN_BTGR   (1 << 10) P1.10
** PIN_SSEL1  (1 << 28) P4.28
** PIN_PAEN   (1 << 7 ) P2.7 
** PIN_HGM    (1 << 8 ) P2.8 
*/
void        ubertooth_gpio_init()
{
	/* 
	 * Set all pins for GPIO.  This shouldn't be necessary after a reset, but
	 * we might get called at other times.
	 */
	ubertooth_gpio_reset();

	/* set certain pins as outputs, all others inputs */
  FIO0DIR = 0;
	FIO1DIR = (PIN_USRLED | PIN_RXLED | PIN_TXLED | PIN_CC3V3 |
			PIN_RX | PIN_CC1V8 | PIN_BTGR);
	FIO2DIR = (PIN_CSN | PIN_SCLK | PIN_MOSI | PIN_PAEN | PIN_HGM);
	FIO3DIR = 0;
	FIO4DIR = (PIN_TX | PIN_SSEL1);

	/* set all outputs low */
	FIO0PIN = 0;
	FIO1PIN = 0;
	FIO2PIN = 0;
	FIO3PIN = 0;
	FIO4PIN = 0;

	/* activate 1V8 supply for CC2400 */
	CC1V8_SET;
	wait_us(50);

	/* CSN (slave select) is active low */
	CSN_SET;

	/* activate 3V3 supply for CC2400 IO */
	CC3V3_SET;

	/* initialise various cc2400 settings - see datasheet pg63 */
  // TODO LMA
	// cc2400_set(MANAND,  0x7fff);

}
void        ubertooth_gpio_reset(void)
{
	/* configure all pins for GPIO */
	PINSEL0 = 0;
	PINSEL1 = 0;
	PINSEL2 = 0;
	PINSEL3 = 0;
	PINSEL4 = 0;
	PINSEL7 = 0;
	PINSEL9 = 0;
	PINSEL10 = 0;

	/* configure all pins as inputs */
	FIO0DIR = 0;
	FIO1DIR = 0;
	FIO2DIR = 0;
	FIO3DIR = 0;
	FIO4DIR = 0;

	/* pull-up on every pin */
	PINMODE0 = 0;
	PINMODE1 = 0;
	PINMODE2 = 0;
	PINMODE3 = 0;
	PINMODE4 = 0;
	PINMODE7 = 0;
	PINMODE9 = 0;

	/* set all outputs low */
	FIO0PIN = 0;
	FIO1PIN = 0;
	FIO2PIN = 0;
	FIO3PIN = 0;
	FIO4PIN = 0;
}


/* =========== CM3 configuration */
static void cm3_enable_dwt_cycnt(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  // enable DWT in ITM
  ITM->TCR |= ITM_TCR_DWTENA_Msk;
  ITM->LAR = 0xC5ACCE55; 
  // enable CYCCNT in DWT
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}
static void cm3_enable_systick(void)
{
  SysTick->CTRL = 0;
  SysTick->LOAD = 0xFFFFFFFF;
  SysTick->VAL = 0;
  SysTick->CTRL = 0x5; // Enable SysTick, use processor clock
}
static void cm3_enable_fault_handler(void)
{
	// enable all fault handlers
  /* SCB->SHCSR = SCB_SHCSR_USGFAULTENA_Msk
    | SCB_SHCSR_BUSFAULTENA_Msk
    | SCB_SHCSR_MEMFAULTENA_Msk; */
}

/* ============ clock configuration
activate the CC2400's 16 MHz oscillator and sync LPC175x to it */
void clock_start()
{
	/* configure flash accelerator for higher clock rate */
	FLASHCFG = (0x03A | (FLASHTIM << 12));

	/* switch to the internal oscillator if necessary and disconnect PLL0
  ** This is the default at boot up, the nominal IRC (internal RC oscillator) is 4MHz 
  ** So it may be unecessary to do it
  */
	CLKSRCSEL = 0;
	/* disconnect PLL0 */
	PLL0CON &= ~PLL0CON_PLLC0;
	PLL0FEED_SEQUENCE;
	while (PLL0STAT & PLL0STAT_PLLC0_STAT);
	/* turn off PLL0 */
	PLL0CON &= ~PLL0CON_PLLE0;
	PLL0FEED_SEQUENCE;
	while (PLL0STAT & PLL0STAT_PLLE0_STAT);
	/* temporarily set CPU clock divider to 1 */
	CCLKCFG = 0;

	/* configure CC2400 oscillator, output carrier sense on GIO6 */
	// cc2400_reset();
	// cc2400_set(IOCFG, (GIO_CARRIER_SENSE_N << 9) | (GIO_CLK_16M << 3));
	// cc2400_strobe(SXOSCON);
	// while (!(cc2400_status() & XOSC16M_STABLE));

	/* activate main oscillator, set OSCEN bit in SCS register (UM10360 3.7.1)
  ** The OSCSTAT bit switch to 1 when main oscillator is ready to use.
  ** Prior to starting the main oscillator, a frequency range must be set in OSCRANGE bit */
	SCS = SCS_OSCRANGE; // set frequency : 15MhZ to 25MhZ
	SCS |= SCS_OSCEN; // enable main oscillator
	while (!(SCS & SCS_OSCSTAT)); // wait for ready

	/* errata sheet says we must select peripheral clock before enabling and connecting PLL0
  ** PCLKSELX registers can be used to set a frequency divider for PCLK (ex: if bit 3:2 == 2, the TIMER0 PCLK = CCLK/2)
  ** See UM10360 4.7.3 */
	PCLKSEL0  = (2 << 2) | (2 << 4);    /* TIMER0 and TIMER1 at cclk/2 (50 MHz) */
	PCLKSEL1  = (2 << 12);              /* TIMER2 at cclk/2 (50 MHz) */

	CLKSRCSEL = 1;  // Select the clock source for PLL0 (and subsequently the CPU), this value is referend as PLLCLKIN:
                  // Use main oscillator (VTXL1 input, connected to C29: 220pF capacitor): can operate at frequency of 1MhZ to 25MhZ
                  // On boot, LPC17 use internal RC oscillator (UM10360: 4.3.1)
                  // Main oscillator is used as clock source of PLL0, the output of PLL0 (PLLCLK) pass through the divider
                  // configurable by CCLKCFG and become the ARM core clock (CCLK)
                  // The frequencies of PLLCLKIN and CCLK are the same value unless the PLL0 is active and connected

	/* 5. configure PLL0: on LPC1756 the maximum CPU clock is 100MhZ
  ** set multiplier value to 24 and divider to 1
  ** PLL0CFG can only be update when PLL0 is inactive
  ** See the procedure in UM10360 4.5.13
#define MSEL0 24
#define NSEL0 1
  ** See 4.5.4 for PLL0CFG multiplier value */
	PLL0CFG = (24 << 0) | (NSEL0 << 16);
  PLL0FEED = 0xAA;  /* PLL0FEED enable loading into PLL0CFG and PLL0CON
                    ** Accidental changes to the PLL0 setup values could result in unexpected
                    ** or fatal behavior of the microcontroller, so its protected by a feed sequence */
  PLL0FEED = 0x55;
	/* 6. enable PLL0, and make it effective with one feed sequence */
	PLL0CON |= PLL0CON_PLLE0;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
	while (!(PLL0STAT & PLL0STAT_PLLE0_STAT));
	/* 7. CPU clock configuration register, select the divide value for creating the CPU clock from the PLL0 output (PLLCLK)
  ** set CPU clock divider to 3, cannot be 0 as PLL0 always output a signal superior of the higher CPU frequency range
  ** See UM10360 4.7.1 */
	CCLKCFG = 3;
	/* 8. Wait for PLL0 to achieve lock */
  while ((PLL0STAT & PLL0STAT_PLOCK0));
	/* 9. connect PLL0 */
	PLL0CON |= PLL0CON_PLLC0;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
	while (!(PLL0STAT & PLL0STAT_PLLC0_STAT));

	/* configure PLL1, receive clock input from the main oscillator only and can only output a fixed 48MhZ clock for USB
  ** subsystem only */
	PLL1CFG = (MSEL1 << 0) | (PSEL1 << 5);
	PLL1FEED_SEQUENCE;
	/* turn on PLL1 */
	PLL1CON |= PLL1CON_PLLE1;
	PLL1FEED_SEQUENCE;
	while (!(PLL1STAT & PLL1STAT_PLLE1_STAT));
	while (!(PLL1STAT & PLL1STAT_PLOCK1));
	/* connect PLL1 */
	PLL1CON |= PLL1CON_PLLC1;
	PLL1FEED_SEQUENCE;
	while (!(PLL1STAT & PLL1STAT_PLLC1_STAT));
}

/* reset the LPC17xx, the cc2400 will be handled by the boot code */
void lpc17_reset()
{
	ubertooth_gpio_reset();

	/* Enable the watchdog with reset enabled */
	USRLED_CLR;
	WDMOD |= WDMOD_WDEN | WDMOD_WDRESET;
	WDFEED_SEQUENCE;
	
	/* Set watchdog timeout to 256us (minimum) */
	
	/* sleep for 1s (minimum) */
	wait(1);
}


/* =========== Exported SystemInit function */
void SystemInit (void)
{

  cm3_enable_dwt_cycnt();
  // cm3_enable_systick();
  cm3_enable_fault_handler();
  // LPC17 driver fast gpio 
	ubertooth_gpio_init();
	clock_start();
  // clkn_start();
}

/* in-system programming */
#define IAP_LOCATION 0x1FFF1FF1
const IAP_ENTRY iap_entry = (IAP_ENTRY)IAP_LOCATION;


void iap_get_part_num(uint8_t *buffer, int *len)
{
	u32 command[5];
	u32 result[5];
	command[0] = 54; /* read part number */
	iap_entry(command, result);
	buffer[0] = result[0] & 0xFF; /* status */
	buffer[1] = result[1] & 0xFF;
	buffer[2] = (result[1] >> 8) & 0xFF;
	buffer[3] = (result[1] >> 16) & 0xFF;
	buffer[4] = (result[1] >> 24) & 0xFF;
	*len = 5;
	
}

void iap_get_device_serial(uint8_t *buffer, int *len)
{
	u32 command[5];
	u32 result[5];
	command[0] = 58; /* read device serial number */
	iap_entry(command, result);
	buffer[0] = result[0] & 0xFF; /* status */
	buffer[1] = result[1] & 0xFF;
	buffer[2] = (result[1] >> 8) & 0xFF;
	buffer[3] = (result[1] >> 16) & 0xFF;
	buffer[4] = (result[1] >> 24) & 0xFF;
	buffer[5] = result[2] & 0xFF;
	buffer[6] = (result[2] >> 8) & 0xFF;
	buffer[7] = (result[2] >> 16) & 0xFF;
	buffer[8] = (result[2] >> 24) & 0xFF;
	buffer[9] = result[3] & 0xFF;
	buffer[10] = (result[3] >> 8) & 0xFF;
	buffer[11] = (result[3] >> 16) & 0xFF;
	buffer[12] = (result[3] >> 24) & 0xFF;
	buffer[13] = result[4] & 0xFF;
	buffer[14] = (result[4] >> 8) & 0xFF;
	buffer[15] = (result[4] >> 16) & 0xFF;
	buffer[16] = (result[4] >> 24) & 0xFF;
	*len = 17;
}

void iap_set_isp(void)
{
	u32 command[5];
	u32 result[5];
	command[0] = 57;
	iap_entry(command, result);
}
