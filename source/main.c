#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board.h"
#include "clock_config.h"
#include "fsl_device_registers.h"
#include "fsl_enet.h"
#include "fsl_iomuxc.h"
#include "fsl_phy.h"
#include "pin_mux.h"

static void SWO_Init() {
  uint32_t SWOSpeed = 64000;
  //  uint32_t prescaler = (SystemCoreClock / SWOSpeed) - 1;
  uint32_t prescaler = (BOARD_BOOTCLOCKRUN_TRACE_CLK_ROOT / SWOSpeed) - 1;

  CoreDebug->DEMCR = CoreDebug_DEMCR_TRCENA_Msk;

  if ((CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk) == 0U)
  {
    while (1);
  }

  /* Lock access */
  ITM->LAR = 0xC5ACCE55U;
  /* Disable ITM */
  ITM->TER &= ~(1U << 1);
  ITM->TCR = 0U;
  /* select SWO encoding protocol */
  TPI->SPPR = 2;
  /* select asynchronous clock prescaler */
  TPI->ACPR = prescaler;
  /* allow unprivilege access */
  ITM->TPR = 0;
  /* enable ITM */
  ITM->TCR = ITM_TCR_ITMENA_Msk | ITM_TCR_SYNCENA_Msk
    | ITM_TCR_TraceBusID_Msk
    | ITM_TCR_SWOENA_Msk | ITM_TCR_DWTENA_Msk;
  /* enable the port bits */
  ITM->TER = 1U << 1;

  DWT->CTRL = 0x400003FE; /* DWT_CTRL */
  TPI->FFCR = 0x00000100; /* Formatter and Flush Control Register */
}

__attribute__((constructor(101))) static void setupHardware() {
  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_BootClockRUN();

  CLOCK_EnableClock(kCLOCK_Trace);
  SWO_Init();

  const clock_enet_pll_config_t config = {
    .enableClkOutput = true,
    .loopDivider = 1,
    .enableClkOutput1 = true,
    .loopDivider1 = 1,
    .enableClkOutput25M = false,
    .src = kCLOCK_PllClkSrc24M,
  };
  CLOCK_InitEnetPll(&config);
  IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);
  IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET2TxClkOutputDir, true);
  IOMUXC_EnableMode(IOMUXC_GPR, IOMUXC_GPR_GPR1_ENET1_CLK_SEL_MASK, false);
  IOMUXC_EnableMode(IOMUXC_GPR, IOMUXC_GPR_GPR1_ENET2_CLK_SEL_MASK, false);

  GPIO_PinWrite(GPIO2, 8, 1); //phy 2 rst pin
}

/********************************/
#define ENET_RXBD_NUM (4)
#define ENET_TXBD_NUM (4)
#define ENET_RXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_TXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_DATA_LENGTH (1000)
#define ENET_TRANSMIT_DATA_NUM (20)
#ifndef APP_ENET_BUFF_ALIGNMENT
#define APP_ENET_BUFF_ALIGNMENT ENET_BUFF_ALIGNMENT
#endif

AT_NONCACHEABLE_SECTION_ALIGN(enet_rx_bd_struct_t g_rxBuffDescrip[2][ENET_RXBD_NUM], ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(enet_tx_bd_struct_t g_txBuffDescrip[2][ENET_TXBD_NUM], ENET_BUFF_ALIGNMENT);

SDK_ALIGN(uint8_t g_rxDataBuff[2][ENET_RXBD_NUM][SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)],
    APP_ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t g_txDataBuff[2][ENET_TXBD_NUM][SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)],
    APP_ENET_BUFF_ALIGNMENT);

enet_handle_t g_handle[2];
uint8_t g_frame[2][ENET_DATA_LENGTH];
uint32_t g_testTxNum = 0;

uint8_t g_macAddr[2][6] = {
  {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x60},
  {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x61}
};

void delay(void) {
  vTaskDelay(1);
}

void pDelayMs(uint32_t ms) {
  vTaskDelay(ms / 5);
}

static void ENET_BuildBroadCastFrame(void) {
  uint32_t count  = 0;
  uint32_t length = ENET_DATA_LENGTH - 14;

  for (uint32_t i = 0; i < 2; i++) {
    for (count = 0; count < 6U; count++)
      g_frame[i][count] = 0xFFU;
    memcpy(&g_frame[i][6], &g_macAddr[i][0], 6U);
    g_frame[i][12] = (length >> 8) & 0xFFU;
    g_frame[i][13] = length & 0xFFU;

    for (count = 0; count < length; count++)
      g_frame[i][count + 14] = count % 0xFFU;
  }
}

static void enetTask(void* arg) {
  uint32_t i = (uint32_t) arg;
  ENET_Type* enet = (i == 0)? ENET: ENET2;
  uint32_t phy_addr = (i == 0)? BOARD_ENET0_PHY_ADDRESS: BOARD_ENET1_PHY_ADDRESS;
  enet_data_error_stats_t eErrStatic;


  printf("\nENET%lu example start.\n", i);

  /* prepare the buffer configuration. */
  enet_buffer_config_t buffConfig[] = {{
    ENET_RXBD_NUM,
    ENET_TXBD_NUM,
    SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
    SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
    &g_rxBuffDescrip[i][0],
    &g_txBuffDescrip[i][0],
    &g_rxDataBuff[i][0][0],
    &g_txDataBuff[i][0][0],
  }};

  uint32_t sysClock = CLOCK_GetFreq(kCLOCK_IpgClk);
  struct PhyState phy_state;
  status_t status = PHY_Init(&phy_state, enet, phy_addr, sysClock);
  if (status != kStatus_Success) {
    printf("%lu: failed to intialize the phy\n", i);
    while (true)
      ;
  }

  enet_config_t config;
  ENET_GetDefaultConfig(&config);
  while (1) {
    uint32_t physts = lpcPHYStsPoll(&phy_state);
    if (physts & PHY_LINK_CHANGED) {
      if (physts & PHY_LINK_CONNECTED) {
        printf("%lu: link is up\n", i);
        if (i == 0)
          BOARD_LED_SET(BOARD_LED2, 1);
        else
          BOARD_LED_SET(BOARD_LED3, 1);
        config.miiSpeed  = (physts & PHY_LINK_SPEED100)? kENET_MiiSpeed100M: kENET_MiiSpeed10M;
        config.miiDuplex = (physts & PHY_LINK_FULLDUPLX)? kENET_MiiFullDuplex: kENET_MiiHalfDuplex;
        break;
      } else {
        if (i == 0)
          BOARD_LED_SET(BOARD_LED2, 0);
        else
          BOARD_LED_SET(BOARD_LED3, 0);
      }
    }
    if (physts & PHY_LINK_BUSY)
      vTaskDelay(configTICK_RATE_HZ / 5);
    else
      vTaskDelay(configTICK_RATE_HZ);
  }

  ENET_Init(enet, &g_handle[i], &config, &buffConfig[0], &g_macAddr[i][0], sysClock);
  ENET_ActiveRead(enet);

  uint32_t txnumber = 0;
  while (1) {
    vTaskDelay(5);
    uint32_t length;
    status = ENET_GetRxFrameSize(&g_handle[i], &length);
    if (length != 0) {
      uint8_t *data = (uint8_t *)malloc(length);
      if (data == NULL) {
        printf("%lu: failed to allocate memory\n", i);
        while (true)
          ;
      }
      status = ENET_ReadFrame(enet, &g_handle[i], data, length);
      if (status == kStatus_Success) {
        printf("%lu: A frame received. the length %lu\n", i, length);
        printf("%lu: Dest Address %02x:%02x:%02x:%02x:%02x:%02x Src Address %02x:%02x:%02x:%02x:%02x:%02x\n",
          i,
          data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9],
          data[10], data[11]
        );
      }
      free(data);
    } else if (status == kStatus_ENET_RxFrameError) {
      ENET_GetRxErrBeforeReadFrame(&g_handle[i], &eErrStatic);
      ENET_ReadFrame(enet, &g_handle[i], NULL, 0);
    }

    //if (g_testTxNum < ENET_TRANSMIT_DATA_NUM)
    {
      g_testTxNum++;
      txnumber++;
      status = ENET_SendFrame(enet, &g_handle[i], &g_frame[i][0], ENET_DATA_LENGTH);
      if (status == kStatus_Success)
        printf("%lu: The %lu frame transmitted success!\n", i, txnumber);
      else
        printf("%lu: Transmit frame failed!\n", i);
    }
  }
}
/*********************/

static void mainTask(void *pvParameters)
{
  while (true) {
    vTaskDelay(100000 / portTICK_PERIOD_MS);
    printf("Still Alive....\n");
  }
}

int main()
{
  puts("Programm Started...");
  printf("System clock is %lu\n", SystemCoreClock);
  ENET_BuildBroadCastFrame();
  xTaskCreate(mainTask, "main", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
  xTaskCreate(enetTask, "enet0", 2*configMINIMAL_STACK_SIZE, (void*)0, tskIDLE_PRIORITY, NULL);

  xTaskCreate(enetTask, "enet1", 2*configMINIMAL_STACK_SIZE, (void*)1, tskIDLE_PRIORITY, NULL);
  vTaskStartScheduler();
  return 1;
}
