#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board.h"
#include "clock_config.h"
#include "emac/emac.h"
#include "fsl_device_registers.h"
#include "fsl_enet.h"
#include "fsl_iomuxc.h"
#include "fsl_phy.h"
#include "interface.h"
#include "pin_mux.h"


static void setupHardwares() {
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

  GPIO_PinWrite(GPIO2, 8, 1);
}

void pDelayMs(uint32_t ms) {
  vTaskDelay(ms / portTICK_PERIOD_MS);
}

static void mainTask(void* arg) {
  extern void iec61850Main();
//  iec61850Main();
  
  printf(">>>> Main Task Finished <<<<\n");
  while(1) {
    vTaskDelay(configTICK_RATE_HZ);
  }
}

static void systemStart() {
  xTaskCreate(mainTask, "main Task", 2048, NULL, (tskIDLE_PRIORITY + 1UL), NULL);
}

static void setLinkUp(ENET_Type * enet, bool full_duplex, bool T100) {
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

static void tcpip_init_done_signal(void *arg)
{
  *(s32_t *) arg = 1;
}

static void lwip_init(struct netif* netif) {
  // FIXME vTaskDelay(NODE_REBOOT_INTERVAL / portTICK_RATE_MS);

  ip4_addr_t ipaddr, netmask, gw;
  volatile s32_t tcpipdone = 0;
  tcpip_init(tcpip_init_done_signal, (void *) &tcpipdone);
  while (!tcpipdone)
    vTaskDelay(1);
#if LWIP_DHCP
  IP4_ADDR(&gw, 0, 0, 0, 0);
  IP4_ADDR(&ipaddr, 0, 0, 0, 0);
  IP4_ADDR(&netmask, 0, 0, 0, 0);
#else
  IP4_ADDR(&gw, 192, 168, 7, 1);
  IP4_ADDR(&ipaddr, 192, 168, 7, 101);
  IP4_ADDR(&netmask, 255, 255, 255, 0);
#endif
  memset(netif, 0, sizeof(struct netif));
  if (!netif_add(netif, &ipaddr, &netmask, &gw, NULL, interface_init, NULL))
  {
    puts("Net interface failed to initialize\r\n");
    while(1);
  }
}

#define ENET_RXBD_NUM (1)
#define ENET_TXBD_NUM (50)
#define ENET_RXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_TXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_DATA_LENGTH (1000)

enet_rx_bd_struct_t rx_desc[2][ENET_RXBD_NUM] __attribute__((section (".bss_dtcm"))) __attribute__((aligned(64)));
enet_tx_bd_struct_t tx_desc[2][ENET_TXBD_NUM] __attribute__((section (".bss_dtcm"))) __attribute__((aligned(64)));
uint8_t enet_frame[ENET_DATA_LENGTH] __attribute__((section (".bss_dtcm"))) __attribute__((aligned(64)));

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

static void sendTask(void* arg) {
  printf("SendTask\r\n");
  static ENET_Type* enet[] = {ENET, ENET2};
  unsigned int i = (unsigned int) arg;
  ENET_Type* e = (ENET_Type*) enet[i];

  uint32_t j = 0;
  while (true) {
    uint32_t eir = ENET->EIR;
    e->EIR = eir & ~(ENET_EIR_MII_MASK);
//    printf("EIR %lx\n", eir);

    extern emac_data_t emac;
    enet_tx_bd_struct_t* tx_desc = emac.port[i].tx_desc;
    tx_desc[j].buffer = enet_frame;
    tx_desc[j].length = ENET_DATA_LENGTH;
    tx_desc[j].control |= ENET_BUFFDESCRIPTOR_TX_LAST_MASK | ENET_BUFFDESCRIPTOR_TX_TRANMITCRC_MASK | ENET_BUFFDESCRIPTOR_TX_READY_MASK;
    e->TDAR |= ENET_TDAR_TDAR_MASK;

    j++;
    if (j >= TX_DESC_COUNT)
      j = 0;
    vTaskDelay(1);
  }
}

static void initRxDesc(enet_rx_bd_struct_t* desc, size_t count) {
  memset(desc, 0, count * sizeof(enet_tx_bd_struct_t));
  desc[count - 1].control |= ENET_BUFFDESCRIPTOR_RX_WRAP_MASK;
}

static void initTxDesc(enet_tx_bd_struct_t* desc, size_t count) {
  memset(desc, 0, count * sizeof(enet_tx_bd_struct_t));
  desc[count - 1].control |= ENET_BUFFDESCRIPTOR_TX_WRAP_MASK;
}


static void ENETInit(ENET_Type* enet, uint32_t rx_desc, uint32_t tx_desc) { 
  enet->ECR |= ENET_ECR_RESET_MASK;

  enet->RCR |= ENET_RCR_RMII_MODE_MASK | ENET_RCR_RMII_MODE_MASK | ENET_RCR_MII_MODE_MASK | ENET_RCR_MAX_FL(1528);
  enet->RCR &= ~(ENET_RCR_LOOP_MASK);
  enet->TCR |= ENET_TCR_ADDINS_MASK;

  enet->PALR = 0x00112233U;
  enet->PAUR = 0x44550000U;
  
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

  enet->ECR |= ENET_ECR_DBSWP_MASK /* | ENET_ECR_EN1588_MASK */| ENET_ECR_ETHEREN_MASK;


}

void enetTask(void* arg) {
//  struct netif n;
//  lwip_init(&n);
  static ENET_Type* enet[] = {ENET, ENET2};

  ENET_BuildBroadCastFrame();
  for (unsigned int i = 1; i < 2; i++) {

    initRxDesc(rx_desc[i], ENET_RXBD_NUM);
    initTxDesc(tx_desc[i], ENET_TXBD_NUM);
    ENETInit(enet[i], (uint32_t)rx_desc[i], (uint32_t)tx_desc[i]);
  }



  static uint32_t phy_addr[] = {BOARD_ENET0_PHY_ADDRESS, BOARD_ENET1_PHY_ADDRESS};
  uint32_t sysClock = CLOCK_GetFreq(kCLOCK_IpgClk);
  struct PhyState phy_state[2];
  for (unsigned int i = 0; i < 2; i++) {
    status_t status = PHY_Init(&phy_state[i], enet[i], phy_addr[i], sysClock);
    if (status != kStatus_Success) {
      printf("failed to intialize the phy %u\n", i);
      while (true)
        ;
    }
  }

  printf("Creating Tasks\r\n");
  xTaskCreate(sendTask, "send", 2*configMINIMAL_STACK_SIZE, (void*)0, tskIDLE_PRIORITY, NULL);
  xTaskCreate(sendTask, "send", 2*configMINIMAL_STACK_SIZE, (void*)1, tskIDLE_PRIORITY, NULL);



/*  NVIC_SetPriority(ENET_IRQn, config_ENET_INTERRUPT_PRIORITY);
  NVIC_SetPriority(ENET2_IRQn, config_ENET_INTERRUPT_PRIORITY);
  NVIC_EnableIRQ(ENET_IRQn);
  NVIC_EnableIRQ(ENET2_IRQn);*/


//  systemStart();


  extern int cntr_adv;
  printf("%p\n",& cntr_adv);
  while (1) {
    printf("counter: %d\n", cntr_adv); // FIXME
    bool busy = false;
    for (unsigned int i = 0; i < 2; i++) {
      uint32_t physts = lpcPHYStsPoll(&phy_state[i]);
      if (physts & PHY_LINK_CHANGED) {
        if (physts & PHY_LINK_CONNECTED) {
          boardLedSet(BOARD_LED2 + i, 1);
          setLinkUp(enet[i], physts & PHY_LINK_FULLDUPLX, physts & PHY_LINK_SPEED100);
          printf("link %d is up\n", i);
        } else {
          boardLedSet(BOARD_LED2 + i, 0);
          setLinkDown(enet[i]);
          printf("link %d is down\n", i);
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

__attribute__((constructor)) static void init() {
  setupHardwares();
  xTaskCreate(enetTask, "enet", 4*configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
}
