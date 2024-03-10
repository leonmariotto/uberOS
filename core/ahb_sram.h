
#ifndef AHBSRAM_H
# define AHBSRAM_H


#include "cmsis_os2.h"

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

// According to ARM we likely have 16k of AHB SRAM at this address
// This memory support BitBand aliases 
// Start : 0x2007C000UL end 0x2007FFFF
// But this would be typically used for peripheral data (IO buffer?) 


#define MAILBOX_BASE            0x2007C000UL      // MailBox

/******************************************************************************/
/*               Software registers structures                                */
/******************************************************************************/
typedef void (*MailBox_Func)(int);

// Reside in MAILBOX_BASE
typedef struct MailBox_s {
  __IOM uint32_t config; // 32bits bitmap data[0] is not a valid data and is always > 1
  __IOM uint32_t flags[32]; // 32bits - 1 flags activable with bitmap
  __IOM MailBox_Func  funcs[32];
} MailBox_TypeDef;

// Reside in MAILBOX_BASE BIT_BAND alias
typedef struct MailBox_BitBand_s {
  __IOM uint32_t config[32];      // Access the config flag in Bit-Band alias
  __IOM uint32_t flags[32][32];  // Each flags can also be accessed with bit-band
} MailBox_BitBand_TypeDef;

/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
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
