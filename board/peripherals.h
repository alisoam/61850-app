/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "fsl_common.h"
#include "fsl_semc.h"
#include "fsl_tempmon.h"
#include "fsl_trng.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
/* Definitions for BOARD_InitPeripherals functional group */
/* BOARD_InitPeripherals defines for SEMC */
/* Definition of peripheral ID. */
#define SEMC_PERIPHERAL SEMC
/* Definition of peripheral ID */
#define TEMPMON_PERIPHERAL TEMPMON
/* TEMPMON interrupt vector ID (number). */
#define TEMPMON_LOW_HIGH_IRQN TEMP_LOW_HIGH_IRQn
/* TEMPMON interrupt vector priority. */
#define TEMPMON_LOW_HIGH_IRQ_PRIORITY 0
/* TEMPMON interrupt handler identifier. */
#define TEMPMON_LOW_HIGH_IRQHANDLER TEMP_LOW_HIGH_IRQHandler
/* TEMPMON interrupt vector ID (number). */
#define TEMPMON_PANIC_IRQN TEMP_PANIC_IRQn
/* TEMPMON interrupt vector priority. */
#define TEMPMON_PANIC_IRQ_PRIORITY 1
/* TEMPMON interrupt handler identifier. */
#define TEMPMON_PANIC_IRQHANDLER TEMP_PANIC_IRQHandler
/* Definition of peripheral ID */
#define TRNG_PERIPHERAL TRNG

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
extern semc_config_t SEMC_config;
extern semc_sdram_config_t SEMC_sdram_struct;
extern const tempmon_config_t TEMPMON_config;
extern const trng_config_t TRNG_config;

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/
void BOARD_InitPeripherals(void);

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void);

#if defined(__cplusplus)
}
#endif

#endif /* _PERIPHERALS_H_ */
