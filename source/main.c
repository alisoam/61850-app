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
#include "peripherals.h"
#include "pin_mux.h"


__attribute__((constructor(101))) static void setupHardware() {
  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitBootPeripherals();

  CLOCK_EnableClock(kCLOCK_Trace);
  extern void SWO_Init();
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

  CLOCK_EnableClock(kCLOCK_Enet);
  CLOCK_EnableClock(kCLOCK_Enet2);

  GPIO_PinWrite(GPIO2, 8, 1); //phy 2 rst pin
}

/********************************/
#define ENET_RXBD_NUM (1)
#define ENET_TXBD_NUM (50)
#define ENET_RXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_TXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_DATA_LENGTH (1000)
#define ENET_TRANSMIT_DATA_NUM (20)
#ifndef APP_ENET_BUFF_ALIGNMENT
#define APP_ENET_BUFF_ALIGNMENT ENET_BUFF_ALIGNMENT
#endif
#define ENET_TX_LAST_FRAME_CONTROL (ENET_BUFFDESCRIPTOR_TX_LAST_MASK | ENET_BUFFDESCRIPTOR_TX_TRANMITCRC_MASK)


AT_NONCACHEABLE_SECTION_ALIGN(enet_rx_bd_struct_t rx_desc[2][ENET_RXBD_NUM], ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(enet_tx_bd_struct_t tx_desc[2][ENET_TXBD_NUM], ENET_BUFF_ALIGNMENT);

uint8_t rx_buff[2][ENET_RXBD_NUM][ENET_RXBUFF_SIZE];

uint8_t enet_frame[ENET_TXBUFF_SIZE];

static void ENET_BuildBroadCastFrame(void) {
  uint32_t count  = 0;
  uint32_t length = ENET_DATA_LENGTH - 14;

  uint8_t* buff = enet_frame;
  for (count = 0; count < 6U; count++)
    buff[count] = 0xFFU;
  buff[12] = (length >> 8) & 0xFFU;
  buff[13] = length & 0xFFU;
  for (count = 0; count < length; count++)
    buff[count + 14] = count % 0xFFU;
}

void pDelayMs(uint32_t ms) {
  vTaskDelay(ms / 5);
}

static void initTxDesc(enet_tx_bd_struct_t* desc, size_t count) {
  memset(desc, 0, count * sizeof(enet_tx_bd_struct_t));
  desc[count - 1].control |= ENET_BUFFDESCRIPTOR_TX_WRAP_MASK;
}

static void initRxDesc(enet_rx_bd_struct_t* desc, size_t count) {
  memset(desc, 0, count * sizeof(enet_tx_bd_struct_t));
  desc[count - 1].control |= ENET_BUFFDESCRIPTOR_RX_WRAP_MASK;
}

static void ENETInit(ENET_Type* enet, uint32_t rx_desc, uint32_t tx_desc) {
  enet->ECR |= ENET_ECR_RESET_MASK;

  enet->RCR |= ENET_RCR_RMII_MODE_MASK | ENET_RCR_RMII_MODE_MASK | ENET_RCR_MII_MODE_MASK | ENET_RCR_MAX_FL(1528);
  enet->RCR &= ~(ENET_RCR_LOOP_MASK);
  enet->TCR |= ENET_TCR_ADDINS_MASK;

  enet->PALR = 0x00112233U;
  enet->PAUR = 0x44550000U;

//  enet->TFWR |= ENET_TFWR_STRFWD_MASK | 0x1U;

  enet->RDSR = rx_desc;
  enet->TDSR = tx_desc;

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

  enet->ECR |= ENET_ECR_DBSWP_MASK/* | ENET_ECR_EN1588_MASK*/ | ENET_ECR_ETHEREN_MASK;
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

static ENET_Type* enet[] = {ENET, ENET2};
static void sendTask(void* arg) {
  uint32_t i = (uint32_t)arg;
  ENET_Type* e = enet[i];

  uint32_t j = 0;
  while (true) {
    uint32_t eir = ENET->EIR;
    e->EIR = eir & ~(ENET_EIR_MII_MASK);
//    printf("EIR %lx\n", eir);

    tx_desc[i][j].buffer = enet_frame;
    tx_desc[i][j].length = ENET_DATA_LENGTH;
    tx_desc[i][j].control |= ENET_BUFFDESCRIPTOR_TX_LAST_MASK | ENET_BUFFDESCRIPTOR_TX_TRANMITCRC_MASK | ENET_BUFFDESCRIPTOR_TX_READY_MASK;
    e->TDAR |= ENET_TDAR_TDAR_MASK;

//    printf("Sent %lu:%lu\n", i, j);
    j++;
    if (j >= ENET_TXBD_NUM)
      j = 0;
    vTaskDelay(1);
  }
}

static void enetTask(void* arg) {
  uint32_t phy_addr[] = {BOARD_ENET0_PHY_ADDRESS, BOARD_ENET1_PHY_ADDRESS};
  uint32_t sysClock = CLOCK_GetFreq(kCLOCK_IpgClk);
  struct PhyState phy_state[2];
  for (unsigned int i = 0; i < 2; i++) {
    initRxDesc(rx_desc[i], ENET_RXBD_NUM);
    initTxDesc(tx_desc[i], ENET_TXBD_NUM);
    ENETInit(enet[i], (uint32_t)rx_desc[i], (uint32_t)tx_desc[i]);

    status_t status = PHY_Init(&phy_state[i], enet[i], phy_addr[i], sysClock);
    if (status != kStatus_Success) {
      printf("failed to intialize the phy %u\n", i);
      while (true)
        ;
    }
  }

  ENET_BuildBroadCastFrame();
  xTaskCreate(sendTask, "send", 2*configMINIMAL_STACK_SIZE, (void*)0, tskIDLE_PRIORITY, NULL);
  xTaskCreate(sendTask, "send", 2*configMINIMAL_STACK_SIZE, (void*)1, tskIDLE_PRIORITY, NULL);

  while (1) {
    bool busy = false;
    for (unsigned int i = 0; i < 2; i++) {
      uint32_t physts = lpcPHYStsPoll(&phy_state[i]);
      if (physts & PHY_LINK_CHANGED) {
        if (physts & PHY_LINK_CONNECTED) {
          boardLedSet(BOARD_LED2 + i, 1);
          setLinkUP(enet[i], physts & PHY_LINK_FULLDUPLX, physts & PHY_LINK_SPEED100);
        } else {
          boardLedSet(BOARD_LED2 + i, 0);
          setLinkDown(enet[i]);
        }
        busy |= physts & PHY_LINK_BUSY;
      }
    }
    if (busy)
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
    vTaskDelay(30 * configTICK_RATE_HZ);
    float temp = TEMPMON_GetCurrentTemperature(TEMPMON);
    printf("Still Alive. Temp: %.1f\n", temp);
  }
}

int main()
{
  puts("Programm Started...");
  printf("System clock is %lu\n", SystemCoreClock);
  extern void mem_test();
  mem_test();

  xTaskCreate(mainTask, "main", 4 * configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
  xTaskCreate(enetTask, "enet", 2*configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);

  vTaskStartScheduler();
  return 1;
}
