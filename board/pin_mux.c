/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v7.0
processor: MIMXRT1062xxxxA
package_id: MIMXRT1062DVL6A
mcu_data: ksdk2_0
processor_version: 7.0.1
pin_labels:
- {pin_num: K1, pin_signal: GPIO_SD_B0_03, label: LED1, identifier: AA;LED1}
- {pin_num: J4, pin_signal: GPIO_SD_B0_00, label: LED2, identifier: LED2}
- {pin_num: H2, pin_signal: GPIO_SD_B0_04, label: LED3, identifier: LED3}
- {pin_num: B9, pin_signal: GPIO_B0_08, label: ENET2_RST, identifier: ENET2_RST}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 * 
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 * 
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void) {
    BOARD_InitPins();
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: G13, peripheral: ARM, signal: arm_trace_swo, pin_signal: GPIO_AD_B0_10}
  - {pin_num: H2, peripheral: GPIO3, signal: 'gpio_io, 16', pin_signal: GPIO_SD_B0_04, direction: OUTPUT}
  - {pin_num: J4, peripheral: GPIO3, signal: 'gpio_io, 12', pin_signal: GPIO_SD_B0_00, direction: OUTPUT}
  - {pin_num: K1, peripheral: GPIO3, signal: 'gpio_io, 15', pin_signal: GPIO_SD_B0_03, identifier: LED1, direction: OUTPUT}
  - {pin_num: B13, peripheral: ENET, signal: enet_ref_clk, pin_signal: GPIO_B1_10}
  - {pin_num: A13, peripheral: ENET, signal: enet_tx_en, pin_signal: GPIO_B1_09}
  - {pin_num: B12, peripheral: ENET, signal: 'enet_tx_data, 0', pin_signal: GPIO_B1_07}
  - {pin_num: A12, peripheral: ENET, signal: 'enet_tx_data, 1', pin_signal: GPIO_B1_08}
  - {pin_num: C13, peripheral: ENET, signal: enet_rx_er, pin_signal: GPIO_B1_11}
  - {pin_num: E12, peripheral: ENET, signal: 'enet_rx_data, 0', pin_signal: GPIO_B1_04}
  - {pin_num: D12, peripheral: ENET, signal: 'enet_rx_data, 1', pin_signal: GPIO_B1_05}
  - {pin_num: C12, peripheral: ENET, signal: enet_rx_en, pin_signal: GPIO_B1_06}
  - {pin_num: C7, peripheral: ENET, signal: enet_mdio, pin_signal: GPIO_EMC_41}
  - {pin_num: A7, peripheral: ENET, signal: enet_mdc, pin_signal: GPIO_EMC_40}
  - {pin_num: C11, peripheral: ENET2, signal: 'enet_rx_data, 1', pin_signal: GPIO_B1_02}
  - {pin_num: B11, peripheral: ENET2, signal: 'enet_rx_data, 0', pin_signal: GPIO_B1_01}
  - {pin_num: C10, peripheral: ENET2, signal: 'enet_tx_data, 0', pin_signal: GPIO_B0_12}
  - {pin_num: D10, peripheral: ENET2, signal: 'enet_tx_data, 1', pin_signal: GPIO_B0_13}
  - {pin_num: D7, peripheral: ENET2, signal: enet_mdc, pin_signal: GPIO_B0_00}
  - {pin_num: E7, peripheral: ENET2, signal: enet_mdio, pin_signal: GPIO_B0_01}
  - {pin_num: B9, peripheral: GPIO2, signal: 'gpio_io, 08', pin_signal: GPIO_B0_08}
  - {pin_num: E10, peripheral: ENET2, signal: enet_tx_en, pin_signal: GPIO_B0_14}
  - {pin_num: A11, peripheral: ENET2, signal: enet_rx_er, pin_signal: GPIO_B1_00}
  - {pin_num: E11, peripheral: ENET2, signal: enet_ref_clk, pin_signal: GPIO_B0_15}
  - {pin_num: D11, peripheral: ENET2, signal: enet_rx_en, pin_signal: GPIO_B1_03}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */

  /* GPIO configuration of LED2 on GPIO_SD_B0_00 (pin J4) */
  gpio_pin_config_t LED2_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_SD_B0_00 (pin J4) */
  GPIO_PinInit(GPIO3, 12U, &LED2_config);

  /* GPIO configuration of LED1 on GPIO_SD_B0_03 (pin K1) */
  gpio_pin_config_t LED1_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_SD_B0_03 (pin K1) */
  GPIO_PinInit(GPIO3, 15U, &LED1_config);

  /* GPIO configuration of LED3 on GPIO_SD_B0_04 (pin H2) */
  gpio_pin_config_t LED3_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_SD_B0_04 (pin H2) */
  GPIO_PinInit(GPIO3, 16U, &LED3_config);

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_10_ARM_TRACE_SWO,     /* GPIO_AD_B0_10 is configured as ARM_TRACE_SWO */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_00_ENET2_MDC,            /* GPIO_B0_00 is configured as ENET2_MDC */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_01_ENET2_MDIO,           /* GPIO_B0_01 is configured as ENET2_MDIO */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_08_GPIO2_IO08,           /* GPIO_B0_08 is configured as GPIO2_IO08 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_12_ENET2_TDATA00,        /* GPIO_B0_12 is configured as ENET2_TDATA00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_13_ENET2_TDATA01,        /* GPIO_B0_13 is configured as ENET2_TDATA01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_14_ENET2_TX_EN,          /* GPIO_B0_14 is configured as ENET2_TX_EN */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_15_ENET2_REF_CLK2,       /* GPIO_B0_15 is configured as ENET2_REF_CLK2 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_00_ENET2_RX_ER,          /* GPIO_B1_00 is configured as ENET2_RX_ER */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_01_ENET2_RDATA00,        /* GPIO_B1_01 is configured as ENET2_RDATA00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_02_ENET2_RDATA01,        /* GPIO_B1_02 is configured as ENET2_RDATA01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_03_ENET2_RX_EN,          /* GPIO_B1_03 is configured as ENET2_RX_EN */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_04_ENET_RX_DATA00,       /* GPIO_B1_04 is configured as ENET_RX_DATA00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_05_ENET_RX_DATA01,       /* GPIO_B1_05 is configured as ENET_RX_DATA01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_06_ENET_RX_EN,           /* GPIO_B1_06 is configured as ENET_RX_EN */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_07_ENET_TX_DATA00,       /* GPIO_B1_07 is configured as ENET_TX_DATA00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_08_ENET_TX_DATA01,       /* GPIO_B1_08 is configured as ENET_TX_DATA01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_09_ENET_TX_EN,           /* GPIO_B1_09 is configured as ENET_TX_EN */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_10_ENET_REF_CLK,         /* GPIO_B1_10 is configured as ENET_REF_CLK */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_11_ENET_RX_ER,           /* GPIO_B1_11 is configured as ENET_RX_ER */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_40_ENET_MDC,            /* GPIO_EMC_40 is configured as ENET_MDC */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_41_ENET_MDIO,           /* GPIO_EMC_41 is configured as ENET_MDIO */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B0_00_GPIO3_IO12,        /* GPIO_SD_B0_00 is configured as GPIO3_IO12 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B0_03_GPIO3_IO15,        /* GPIO_SD_B0_03 is configured as GPIO3_IO15 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B0_04_GPIO3_IO16,        /* GPIO_SD_B0_04 is configured as GPIO3_IO16 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_GPR->GPR27 = ((IOMUXC_GPR->GPR27 &
    (~(IOMUXC_GPR_GPR27_GPIO_MUX2_GPIO_SEL_MASK))) /* Mask bits to zero which are setting */
      | IOMUXC_GPR_GPR27_GPIO_MUX2_GPIO_SEL(0x00U) /* GPIO2 and GPIO7 share same IO MUX function, GPIO_MUX2 selects one GPIO function: 0x00U */
    );
  IOMUXC_GPR->GPR28 = ((IOMUXC_GPR->GPR28 &
    (~(IOMUXC_GPR_GPR28_GPIO_MUX3_GPIO_SEL_MASK))) /* Mask bits to zero which are setting */
      | IOMUXC_GPR_GPR28_GPIO_MUX3_GPIO_SEL(0x00U) /* GPIO3 and GPIO8 share same IO MUX function, GPIO_MUX3 selects one GPIO function: 0x00U */
    );
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
