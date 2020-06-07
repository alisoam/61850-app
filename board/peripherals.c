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
 * SEMC initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'SEMC'
- type: 'semc'
- mode: 'general'
- custom_name_enabled: 'false'
- type_id: 'semc_8caeb64ecb2dd34cd7fd365f593107fd'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'SEMC'
- config_sets:
  - fsl_semc:
    - enableDCD: 'false'
    - clockConfig:
      - clockSource: 'kSEMC_ClkSrcPeri'
      - clockSourceFreq: 'BOARD_BootClockRUN'
    - semc_config_t:
      - dqsMode: 'kSEMC_Loopbackinternal'
      - cmdTimeoutCycles: '0'
      - busTimeoutCycles: '0x1F'
      - queueWeight:
        - queueaWeight:
          - queueaConfig:
            - qos: '0'
            - aging: '0'
            - slaveHitSwith: '0'
            - slaveHitNoswitch: '0'
          - queueaValue: '0'
        - queuebWeight:
          - queuebConfig:
            - qos: '0'
            - aging: '0'
            - slaveHitSwith: '0'
            - weightPagehit: '0'
            - bankRotation: '0'
          - queuebValue: '0'
    - semc_sdram_config_t:
      - csxPinMux: 'kSEMC_MUXCSX0'
      - semcSdramCs: 'kSEMC_SDRAM_CS0'
      - address: '0x80000000'
      - memsize_kbytes: '32768'
      - portSize: 'kSEMC_PortSize16Bit'
      - burstLen: 'kSEMC_Sdram_BurstLen8'
      - columnAddrBitNum: 'kSEMC_SdramColunm_9bit'
      - casLatency: 'kSEMC_LatencyThree'
      - tPrecharge2Act_Ns: '18'
      - tAct2ReadWrite_Ns: '18'
      - tRefreshRecovery_Ns: '127'
      - tWriteRecovery_Ns: '12'
      - tCkeOff_Ns: '42'
      - tAct2Prechage_Ns: '42'
      - tSelfRefRecovery_Ns: '67'
      - tRefresh2Refresh_Ns: '60'
      - tAct2Act_Ns: '60'
      - tPrescalePeriod_Ns: '160'
      - tIdleTimeout_Ns: '0'
      - refreshPeriod_nsPerRow: '64'
      - refreshUrgThreshold: '64'
      - refreshBurstLen: '1'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
semc_config_t SEMC_config = {
  .dqsMode = kSEMC_Loopbackinternal,
  .cmdTimeoutCycles = 0,
  .busTimeoutCycles = 0x1F,
  .queueWeight = {
    .queueaWeight = {
      .queueaConfig = {
        .qos = 0,
        .aging = 0,
        .slaveHitSwith = 0,
        .slaveHitNoswitch = 0
      },
      .queueaValue = 0
    },
    .queuebWeight = {
      .queuebConfig = {
        .qos = 0,
        .aging = 0,
        .slaveHitSwith = 0,
        .weightPagehit = 0,
        .bankRotation = 0
      },
      .queuebValue = 0
    }
  }
};
semc_sdram_config_t SEMC_sdram_struct = {
  .csxPinMux = kSEMC_MUXCSX0,
  .address = 0x80000000,
  .memsize_kbytes = 32768,
  .portSize = kSEMC_PortSize16Bit,
  .burstLen = kSEMC_Sdram_BurstLen8,
  .columnAddrBitNum = kSEMC_SdramColunm_9bit,
  .casLatency = kSEMC_LatencyThree,
  .tPrecharge2Act_Ns = 18,
  .tAct2ReadWrite_Ns = 18,
  .tRefreshRecovery_Ns = 127,
  .tWriteRecovery_Ns = 12,
  .tCkeOff_Ns = 42,
  .tAct2Prechage_Ns = 42,
  .tSelfRefRecovery_Ns = 67,
  .tRefresh2Refresh_Ns = 60,
  .tAct2Act_Ns = 60,
  .tPrescalePeriod_Ns = 160,
  .tIdleTimeout_Ns = 0,
  .refreshPeriod_nsPerRow = 64,
  .refreshUrgThreshold = 64,
  .refreshBurstLen = 1
};

void SEMC_init(void) {
  /* Initialize SEMC peripheral. */
  SEMC_Init(SEMC_PERIPHERAL, &SEMC_config);
  /* Initialize SEMC SDRAM. */
  SEMC_ConfigureSDRAM(SEMC_PERIPHERAL, kSEMC_SDRAM_CS0, &SEMC_sdram_struct, 160000000);
}

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
      - frequencySet: '1s'
      - lowAlarmTemp: '40'
      - highAlarmTemp: '70'
      - panicAlarmTemp: '90'
    - startMeasure: 'true'
    - interruptsCfg:
      - isInterruptEnabledLowHigh: 'true'
      - interruptLowHigh:
        - IRQn: 'TEMP_LOW_HIGH_IRQn'
        - enable_priority: 'true'
        - priority: '0'
        - enable_custom_name: 'false'
      - isInterruptEnabledPanic: 'true'
      - interruptPanic:
        - IRQn: 'TEMP_PANIC_IRQn'
        - enable_priority: 'true'
        - priority: '1'
        - enable_custom_name: 'false'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const tempmon_config_t TEMPMON_config = {
  .frequency = 32768,
  .lowAlarmTemp = 40,
  .highAlarmTemp = 70,
  .panicAlarmTemp = 90
};

void TEMPMON_init(void) {
  /* TEMPMON initialization */
  TEMPMON_Init(TEMPMON_PERIPHERAL, &TEMPMON_config);
  /* Interrupt vector TEMP_LOW_HIGH_IRQn priority settings in the NVIC */
  NVIC_SetPriority(TEMP_LOW_HIGH_IRQn, TEMPMON_LOW_HIGH_IRQ_PRIORITY);
  /* Interrupt vector TEMP_PANIC_IRQn priority settings in the NVIC */
  NVIC_SetPriority(TEMP_PANIC_IRQn, TEMPMON_PANIC_IRQ_PRIORITY);
  /* Enable interrupt TEMP_LOW_HIGH_IRQn request in the NVIC */
  EnableIRQ(TEMP_LOW_HIGH_IRQn);
  /* Enable interrupt TEMP_PANIC_IRQn request in the NVIC */
  EnableIRQ(TEMP_PANIC_IRQn);
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
  SEMC_init();
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
