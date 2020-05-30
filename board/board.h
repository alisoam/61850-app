/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"

#define BOARD_NAME "COM"

#define LED_ON   (1U)
#define LED_OFF  (0U)

#define BOARD_LED1_GPIO GPIO3
#define BOARD_LED1_GPIO_PIN (16U)

#define BOARD_LED2_GPIO GPIO3
#define BOARD_LED2_GPIO_PIN (12U)

#define BOARD_LED3_GPIO GPIO3
#define BOARD_LED3_GPIO_PIN (15U)

#define BOARD_LED1 BOARD_LED1_GPIO, BOARD_LED1_GPIO_PIN
#define BOARD_LED2 BOARD_LED2_GPIO, BOARD_LED2_GPIO_PIN
#define BOARD_LED3 BOARD_LED3_GPIO, BOARD_LED3_GPIO_PIN
#define BOARD_LED_SET(LED, VAL) GPIO_PinWrite(LED, VAL)

#define BOARD_FLASH_SIZE (0x800000U)

#define BOARD_ENET0_PHY_ADDRESS (0x01U)
#define BOARD_ENET1_PHY_ADDRESS (0x01U)


#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */


void BOARD_ConfigMPU(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
