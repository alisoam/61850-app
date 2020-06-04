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
#define ENET_RXBD_NUM (1)
#define ENET_TXBD_NUM (1)
#define ENET_RXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_TXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_DATA_LENGTH (1000)
#define ENET_TRANSMIT_DATA_NUM (20)
#ifndef APP_ENET_BUFF_ALIGNMENT
#define APP_ENET_BUFF_ALIGNMENT ENET_BUFF_ALIGNMENT
#endif

AT_NONCACHEABLE_SECTION_ALIGN(enet_rx_bd_struct_t rx_desc[2][ENET_RXBD_NUM], ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(enet_tx_bd_struct_t tx_desc[2][ENET_TXBD_NUM], ENET_BUFF_ALIGNMENT);

uint8_t rx_buff[2][ENET_RXBD_NUM][ENET_RXBUFF_SIZE];
uint8_t enet_frame[2][ENET_TXBD_NUM][ENET_TXBUFF_SIZE];


static void ENET_BuildBroadCastFrame(void) {
  uint32_t count  = 0;
  uint32_t length = ENET_DATA_LENGTH - 14;

  for (uint32_t i = 0; i < 2; i++) {
    uint8_t* buff = enet_frame[i][0];
    
    for (count = 0; count < 6U; count++)
      buff[count] = 0xFFU;
    buff[12] = (length >> 8) & 0xFFU;
    buff[13] = length & 0xFFU;

    for (count = 0; count < length; count++)
      buff[count + 14] = count % 0xFFU;
  }
}

void pDelayMs(uint32_t ms) {
  vTaskDelay(ms / 5);
}

static void initTxDesc() {
  tx_desc[0][0].length = ENET_DATA_LENGTH;
  tx_desc[0][0].control = ENET_BUFFDESCRIPTOR_TX_WRAP_MASK |
    ENET_BUFFDESCRIPTOR_TX_LAST_MASK | ENET_BUFFDESCRIPTOR_TX_TRANMITCRC_MASK;
  tx_desc[0][0].control &= ~ENET_BUFFDESCRIPTOR_TX_READY_MASK;

  tx_desc[0][0].buffer = enet_frame[0][0];
}

static void initRxDesc() {
  rx_desc[0][0].control = ENET_BUFFDESCRIPTOR_RX_WRAP_MASK;

  rx_desc[0][0].buffer = rx_buff[0][0];
}

static void ENETInit(ENET_Type * enet) {
  CLOCK_EnableClock(kCLOCK_Enet2);

  enet->ECR |= ENET_ECR_RESET_MASK;

  initRxDesc();
  initTxDesc();

  enet->RCR |= ENET_RCR_RMII_MODE_MASK | ENET_RCR_RMII_MODE_MASK | ENET_RCR_MII_MODE_MASK;
  enet->RCR &= ~(ENET_RCR_LOOP_MASK);
  enet->TCR |= ENET_TCR_ADDINS_MASK;

  enet->PALR = 0x00112233U;
  enet->PAUR = 0x44550000U;

//  enet->TFWR |= ENET_TFWR_STRFWD_MASK | 0x1U;

  enet->RDSR = (uint32_t)(&(rx_desc[0][0]));
  enet->TDSR = (uint32_t)(&(tx_desc[0][0]));

  enet->MRBR |= ENET_MRBR_R_BUF_SIZE(ENET_RXBUFF_SIZE);

  enet->RSFL = ENET_RSFL_RX_SECTION_FULL(20);
  enet->RSEM |= ENET_RSEM_RX_SECTION_EMPTY(45);
  enet->RAEM |= ENET_RAEM_RX_ALMOST_EMPTY(10);
  enet->RAFL |= ENET_RAFL_RX_ALMOST_FULL(60);

  enet->TSEM |= ENET_TSEM_TX_SECTION_EMPTY(45);
  enet->TAEM |= ENET_TAEM_TX_ALMOST_EMPTY(10);
  enet->TAFL |= ENET_TAFL_TX_ALMOST_FULL(60);

  enet->TACC |= 0;
  enet->RACC |= ENET_RACC_LINEDIS_MASK;

  enet->ECR |= ENET_ECR_DBSWP_MASK | ENET_ECR_EN1588_MASK | ENET_ECR_ETHEREN_MASK;
}

static void setLinkUP(ENET_Type * enet, bool full_duplex, bool T100) {
  if (full_duplex) {
    enet->RCR &= ~ENET_RCR_DRT_MASK;
    enet->TCR |= ENET_TCR_FDEN_MASK;
  } else {
    enet->RCR |= ENET_RCR_DRT_MASK;
    enet->TCR &= ~ENET_TCR_FDEN_MASK;
  }

  if (T100)
    enet->RCR &= ~ENET_RCR_RMII_10T_MASK;
  else
    enet->RCR |= ENET_RCR_RMII_10T_MASK;
}

static void setLinkDown(ENET_Type * enet) {
}

static void ENETTest() {
  tx_desc[0][0].control |= ENET_BUFFDESCRIPTOR_TX_READY_MASK;
  ENET2->TDAR |= ENET_TDAR_TDAR_MASK;

  while (true) {
    printf("CON %x\n", tx_desc[0][0].control);
    uint32_t eir = ENET2->EIR;
    ENET2->EIR = eir;
    printf("EIR %lx\n", eir);

    tx_desc[0][0].control |= ENET_BUFFDESCRIPTOR_TX_READY_MASK;
    ENET2->TDAR |= ENET_TDAR_TDAR_MASK;
    printf("Sent\n");
    vTaskDelay(configTICK_RATE_HZ);
  }
}

static void enetTask(void* arg) {
  uint32_t i = (uint32_t) arg;
  ENET_Type* enet = (i == 0)? ENET: ENET2;
  uint32_t phy_addr = (i == 0)? BOARD_ENET0_PHY_ADDRESS: BOARD_ENET1_PHY_ADDRESS;

  ENETInit(ENET2);

  uint32_t sysClock = CLOCK_GetFreq(kCLOCK_IpgClk);
  struct PhyState phy_state;
  status_t status = PHY_Init(&phy_state, enet, phy_addr, sysClock);
  if (status != kStatus_Success) {
    printf("%lu: failed to intialize the phy\n", i);
    while (true)
      ;
  }

  printf("START\n");
  while (1) {
    uint32_t physts = lpcPHYStsPoll(&phy_state);
    if (physts & PHY_LINK_CHANGED) {
      if (physts & PHY_LINK_CONNECTED) {
        if (i == 0)
          BOARD_LED_SET(BOARD_LED2, 1);
        else
          BOARD_LED_SET(BOARD_LED3, 1);
        puts("Link is up");
        setLinkUP(enet, physts & PHY_LINK_FULLDUPLX, physts & PHY_LINK_SPEED100);
        ENETTest();
      } else {
        if (i == 0)
          BOARD_LED_SET(BOARD_LED2, 0);
        else
          BOARD_LED_SET(BOARD_LED3, 0);
        puts("Link is down");
        setLinkDown(enet);
      }
    }
    if (physts & PHY_LINK_BUSY)
      vTaskDelay(configTICK_RATE_HZ / 5);
    else
      vTaskDelay(configTICK_RATE_HZ);
  }

  while (1) {
    vTaskDelay(5);
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

//  xTaskCreate(enetTask, "enet0", 2*configMINIMAL_STACK_SIZE, (void*)0, tskIDLE_PRIORITY, NULL);
  xTaskCreate(enetTask, "enet1", 2*configMINIMAL_STACK_SIZE, (void*)1, tskIDLE_PRIORITY, NULL);

  vTaskStartScheduler();
  return 1;
}
