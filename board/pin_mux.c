/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v8.0
processor: MIMXRT1062xxxxA
package_id: MIMXRT1062DVL6A
mcu_data: ksdk2_0
processor_version: 8.0.3
pin_labels:
- {pin_num: K1, pin_signal: GPIO_SD_B0_03, label: LED3, identifier: LED3}
- {pin_num: J4, pin_signal: GPIO_SD_B0_00, label: LED2, identifier: LED2}
- {pin_num: H2, pin_signal: GPIO_SD_B0_04, label: LED1, identifier: LED1}
- {pin_num: B9, pin_signal: GPIO_B0_08, label: ENET2_RST, identifier: ENET2_RST}
- {pin_num: H10, pin_signal: GPIO_AD_B0_01, label: LED1, identifier: LED1}
- {pin_num: M11, pin_signal: GPIO_AD_B0_02, label: LED2, identifier: LED2}
- {pin_num: C14, pin_signal: GPIO_B1_14, label: LED3, identifier: LED3}
- {pin_num: K12, pin_signal: GPIO_AD_B1_05, label: ENET1_RST, identifier: ENET1_RST}
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
  - {pin_num: H10, peripheral: GPIO1, signal: 'gpio_io, 01', pin_signal: GPIO_AD_B0_01, direction: OUTPUT}
  - {pin_num: M11, peripheral: GPIO1, signal: 'gpio_io, 02', pin_signal: GPIO_AD_B0_02, direction: OUTPUT}
  - {pin_num: C14, peripheral: GPIO2, signal: 'gpio_io, 30', pin_signal: GPIO_B1_14, direction: OUTPUT}
  - {pin_num: B13, peripheral: ENET, signal: enet_ref_clk, pin_signal: GPIO_B1_10, software_input_on: Enable, pull_keeper_select: Keeper, pull_keeper_enable: Disable,
    speed: MHZ_50, slew_rate: Fast}
  - {pin_num: A13, peripheral: ENET, signal: enet_tx_en, pin_signal: GPIO_B1_09, pull_up_down_config: Pull_Up_100K_Ohm}
  - {pin_num: B12, peripheral: ENET, signal: 'enet_tx_data, 0', pin_signal: GPIO_B1_07, pull_up_down_config: Pull_Up_100K_Ohm, pull_keeper_select: Pull, speed: MHZ_200,
    drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: A12, peripheral: ENET, signal: 'enet_tx_data, 1', pin_signal: GPIO_B1_08, pull_up_down_config: Pull_Up_100K_Ohm, pull_keeper_select: Pull, speed: MHZ_200,
    drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: C13, peripheral: ENET, signal: enet_rx_er, pin_signal: GPIO_B1_11, pull_up_down_config: Pull_Up_100K_Ohm, pull_keeper_select: Pull, speed: MHZ_200,
    drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: E12, peripheral: ENET, signal: 'enet_rx_data, 0', pin_signal: GPIO_B1_04, pull_up_down_config: Pull_Up_100K_Ohm, pull_keeper_select: Pull, speed: MHZ_200,
    drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: D12, peripheral: ENET, signal: 'enet_rx_data, 1', pin_signal: GPIO_B1_05, pull_up_down_config: Pull_Up_100K_Ohm, pull_keeper_select: Pull, speed: MHZ_200,
    drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: C12, peripheral: ENET, signal: enet_rx_en, pin_signal: GPIO_B1_06, pull_up_down_config: Pull_Up_100K_Ohm, pull_keeper_select: Pull, speed: MHZ_200,
    drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: C7, peripheral: ENET, signal: enet_mdio, pin_signal: GPIO_EMC_41}
  - {pin_num: A7, peripheral: ENET, signal: enet_mdc, pin_signal: GPIO_EMC_40, pull_keeper_select: Keeper, speed: MHZ_100}
  - {pin_num: C11, peripheral: ENET2, signal: 'enet_rx_data, 1', pin_signal: GPIO_B1_02, pull_up_down_config: Pull_Up_100K_Ohm, pull_keeper_select: Pull, speed: MHZ_200,
    drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: B11, peripheral: ENET2, signal: 'enet_rx_data, 0', pin_signal: GPIO_B1_01, pull_up_down_config: Pull_Up_100K_Ohm, pull_keeper_select: Pull, speed: MHZ_200,
    drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: C10, peripheral: ENET2, signal: 'enet_tx_data, 0', pin_signal: GPIO_B0_12, pull_up_down_config: Pull_Up_100K_Ohm, pull_keeper_select: Pull, speed: MHZ_200,
    drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: D10, peripheral: ENET2, signal: 'enet_tx_data, 1', pin_signal: GPIO_B0_13, pull_up_down_config: Pull_Up_100K_Ohm, pull_keeper_select: Pull, speed: MHZ_200,
    drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: D7, peripheral: ENET2, signal: enet_mdc, pin_signal: GPIO_B0_00}
  - {pin_num: E7, peripheral: ENET2, signal: enet_mdio, pin_signal: GPIO_B0_01}
  - {pin_num: E10, peripheral: ENET2, signal: enet_tx_en, pin_signal: GPIO_B0_14, pull_up_down_config: Pull_Up_100K_Ohm, pull_keeper_select: Pull, speed: MHZ_200,
    drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: A11, peripheral: ENET2, signal: enet_rx_er, pin_signal: GPIO_B1_00, pull_up_down_config: Pull_Up_100K_Ohm, pull_keeper_select: Pull, speed: MHZ_200,
    drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: E11, peripheral: ENET2, signal: enet_ref_clk, pin_signal: GPIO_B0_15, software_input_on: Enable, pull_keeper_enable: Disable, speed: MHZ_50, slew_rate: Fast}
  - {pin_num: D11, peripheral: ENET2, signal: enet_rx_en, pin_signal: GPIO_B1_03, pull_up_down_config: Pull_Up_100K_Ohm, pull_keeper_select: Pull, speed: MHZ_200,
    drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: K12, peripheral: GPIO1, signal: 'gpio_io, 21', pin_signal: GPIO_AD_B1_05, direction: OUTPUT}
  - {pin_num: B9, peripheral: GPIO2, signal: 'gpio_io, 08', pin_signal: GPIO_B0_08, direction: OUTPUT}
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

  /* GPIO configuration of LED1 on GPIO_AD_B0_01 (pin H10) */
  gpio_pin_config_t LED1_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_AD_B0_01 (pin H10) */
  GPIO_PinInit(GPIO1, 1U, &LED1_config);

  /* GPIO configuration of LED2 on GPIO_AD_B0_02 (pin M11) */
  gpio_pin_config_t LED2_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_AD_B0_02 (pin M11) */
  GPIO_PinInit(GPIO1, 2U, &LED2_config);

  /* GPIO configuration of ENET1_RST on GPIO_AD_B1_05 (pin K12) */
  gpio_pin_config_t ENET1_RST_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_AD_B1_05 (pin K12) */
  GPIO_PinInit(GPIO1, 21U, &ENET1_RST_config);

  /* GPIO configuration of ENET2_RST on GPIO_B0_08 (pin B9) */
  gpio_pin_config_t ENET2_RST_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_B0_08 (pin B9) */
  GPIO_PinInit(GPIO2, 8U, &ENET2_RST_config);

  /* GPIO configuration of LED3 on GPIO_B1_14 (pin C14) */
  gpio_pin_config_t LED3_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_B1_14 (pin C14) */
  GPIO_PinInit(GPIO2, 30U, &LED3_config);

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_01_GPIO1_IO01,        /* GPIO_AD_B0_01 is configured as GPIO1_IO01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_02_GPIO1_IO02,        /* GPIO_AD_B0_02 is configured as GPIO1_IO02 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_10_ARM_TRACE_SWO,     /* GPIO_AD_B0_10 is configured as ARM_TRACE_SWO */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_05_GPIO1_IO21,        /* GPIO_AD_B1_05 is configured as GPIO1_IO21 */
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
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_B0_15 */
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
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_B1_10 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_11_ENET_RX_ER,           /* GPIO_B1_11 is configured as ENET_RX_ER */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_14_GPIO2_IO30,           /* GPIO_B1_14 is configured as GPIO2_IO30 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_40_ENET_MDC,            /* GPIO_EMC_40 is configured as ENET_MDC */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_41_ENET_MDIO,           /* GPIO_EMC_41 is configured as ENET_MDIO */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_GPR->GPR26 = ((IOMUXC_GPR->GPR26 &
    (~(BOARD_INITPINS_IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL_MASK))) /* Mask bits to zero which are setting */
      | IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL(0x00U) /* GPIO1 and GPIO6 share same IO MUX function, GPIO_MUX1 selects one GPIO function: 0x00U */
    );
  IOMUXC_GPR->GPR27 = ((IOMUXC_GPR->GPR27 &
    (~(BOARD_INITPINS_IOMUXC_GPR_GPR27_GPIO_MUX2_GPIO_SEL_MASK))) /* Mask bits to zero which are setting */
      | IOMUXC_GPR_GPR27_GPIO_MUX2_GPIO_SEL(0x00U) /* GPIO2 and GPIO7 share same IO MUX function, GPIO_MUX2 selects one GPIO function: 0x00U */
    );
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_12_ENET2_TDATA00,        /* GPIO_B0_12 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_13_ENET2_TDATA01,        /* GPIO_B0_13 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_14_ENET2_TX_EN,          /* GPIO_B0_14 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_15_ENET2_REF_CLK2,       /* GPIO_B0_15 PAD functional properties : */
      0x31U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: low(50MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Disabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_00_ENET2_RX_ER,          /* GPIO_B1_00 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_01_ENET2_RDATA00,        /* GPIO_B1_01 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_02_ENET2_RDATA01,        /* GPIO_B1_02 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_03_ENET2_RX_EN,          /* GPIO_B1_03 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_04_ENET_RX_DATA00,       /* GPIO_B1_04 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_05_ENET_RX_DATA01,       /* GPIO_B1_05 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_06_ENET_RX_EN,           /* GPIO_B1_06 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_07_ENET_TX_DATA00,       /* GPIO_B1_07 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_08_ENET_TX_DATA01,       /* GPIO_B1_08 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_09_ENET_TX_EN,           /* GPIO_B1_09 PAD functional properties : */
      0x90B0U);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_10_ENET_REF_CLK,         /* GPIO_B1_10 PAD functional properties : */
      0x31U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: low(50MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Disabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_11_ENET_RX_ER,           /* GPIO_B1_11 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_EMC_40_ENET_MDC,            /* GPIO_EMC_40 PAD functional properties : */
      0x10B0U);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
