/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Peripherals v7.0
processor: MIMXRT1062xxxxA
package_id: MIMXRT1062DVL6A
mcu_data: ksdk2_0
processor_version: 7.0.1
functionalGroups:
- name: BOARD_InitPeripherals
  UUID: d6e84094-ad11-42b4-8ad2-e6e1cd911694
  called_from_default_init: true
  selectedCore: core0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'system'
- type_id: 'system_54b53072540eeeb8f8e9343e71f28176'
- global_system_definitions: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'msg'
- type_id: 'msg_6e2baaf3b97dbeef01c0043275f9a0e7'
- global_messages: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "peripherals.h"

/***********************************************************************************************************************
 * BOARD_InitPeripherals functional group
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * TEMPMON initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'TEMPMON'
- type: 'tempmon'
- mode: 'general'
- custom_name_enabled: 'false'
- type_id: 'tempmon_c966e400757748d37785414258f2801c'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'TEMPMON'
- config_sets:
  - fsl_tempmon:
    - tempmon_config:
      - singleMeasurement: 'false'
      - clock_config:
        - clockSource: 'LowFreqClock'
        - clockSourceFreq: 'BOARD_BootClockRUN'
      - frequencySet: '10922 Hz'
      - lowAlarmTemp: '39'
      - highAlarmTemp: '44'
      - panicAlarmTemp: '90'
    - startMeasure: 'true'
    - interruptsCfg:
      - isInterruptEnabledLowHigh: 'false'
      - interruptLowHigh:
        - IRQn: 'TEMP_LOW_HIGH_IRQn'
        - enable_priority: 'true'
        - priority: '0'
        - enable_custom_name: 'false'
      - isInterruptEnabledPanic: 'false'
      - interruptPanic:
        - IRQn: 'TEMP_PANIC_IRQn'
        - enable_priority: 'true'
        - priority: '1'
        - enable_custom_name: 'false'
    - quick_selection: 'QuickSelection1'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const tempmon_config_t TEMPMON_config = {
  .frequency = 3,
  .lowAlarmTemp = 39,
  .highAlarmTemp = 44,
  .panicAlarmTemp = 90
};

void TEMPMON_init(void) {
  /* TEMPMON initialization */
  TEMPMON_Init(TEMPMON_PERIPHERAL, &TEMPMON_config);
  /* Start measure */
  TEMPMON_StartMeasure(TEMPMON_PERIPHERAL);
}

/***********************************************************************************************************************
 * TRNG initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'TRNG'
- type: 'TRNG'
- mode: 'General'
- custom_name_enabled: 'false'
- type_id: 'trng_dd5b2a6f14d45ca091cf6c9a38560d5e'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'TRNG'
- config_sets:
  - fsl_trng:
    - settings_trng_config_t:
      - clockMode: 'kTRNG_ClockModeRingOscillator'
      - struct_ring_oscillator:
        - ringOscDiv: 'kTRNG_RingOscDiv0'
        - sampleMode: 'kTRNG_SampleModeRaw'
      - entropyDelay: '3200'
      - sampleSize: '2500'
      - sparseBitLimit: '63'
      - retryCount: '1'
      - lock: 'false'
      - struct_statistical_checks:
        - longRunMaxLimit: '34'
        - monobitLimit:
          - maximum: '1384'
          - minimum: '1116'
        - runBit1Limit:
          - maximum: '405'
          - minimum: '227'
        - runBit2Limit:
          - maximum: '220'
          - minimum: '98'
        - runBit3Limit:
          - maximum: '125'
          - minimum: '37'
        - runBit4Limit:
          - maximum: '75'
          - minimum: '11'
        - runBit5Limit:
          - maximum: '47'
          - minimum: '1'
        - runBit6PlusLimit:
          - maximum: '47'
          - minimum: '1'
        - pokerLimit:
          - maximum: '26912'
          - minimum: '24445'
        - frequencyCountLimit:
          - maximum: '25600'
          - minimum: '1600'
    - quick_selection: 'Default'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const trng_config_t TRNG_config = {
  .lock = false,
  .clockMode = kTRNG_ClockModeRingOscillator,
  .ringOscDiv = kTRNG_RingOscDiv0,
  .sampleMode = kTRNG_SampleModeRaw,
  .entropyDelay = 3200,
  .sampleSize = 2500,
  .sparseBitLimit = 63,
  .retryCount = 1,
  .longRunMaxLimit = 34,
  .monobitLimit = {
    .maximum = 1384,
    .minimum = 1116
  },
  .runBit1Limit = {
    .maximum = 405,
    .minimum = 227
  },
  .runBit2Limit = {
    .maximum = 220,
    .minimum = 98
  },
  .runBit3Limit = {
    .maximum = 125,
    .minimum = 37
  },
  .runBit4Limit = {
    .maximum = 75,
    .minimum = 11
  },
  .runBit5Limit = {
    .maximum = 47,
    .minimum = 1
  },
  .runBit6PlusLimit = {
    .maximum = 47,
    .minimum = 1
  },
  .pokerLimit = {
    .maximum = 26912,
    .minimum = 24445
  },
  .frequencyCountLimit = {
    .maximum = 25600,
    .minimum = 1600
  }
};

void TRNG_init(void) {
  /* Initialize TRNG. */
  TRNG_Init(TRNG_PERIPHERAL, &TRNG_config);
}

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/
void BOARD_InitPeripherals(void)
{
  /* Initialize components */
  TEMPMON_init();
  TRNG_init();
}

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void)
{
  BOARD_InitPeripherals();
}
