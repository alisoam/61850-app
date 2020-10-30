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


extern int counter;

void pDelayMs(uint32_t ms) {
  vTaskDelay(ms / 5);
}

static void mainTask(void* arg) {
  extern void iec61850Main();
//  iec61850Main();
  
  while(1) {
    printf(">>>> Main Task Finished <<<<\n");
    vTaskDelay(configTICK_RATE_HZ / 2);
  }
}

static void systemStart() {
  xTaskCreate(mainTask, "main Task", 2048, NULL, (tskIDLE_PRIORITY + 1UL), NULL);
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

#define ENET_DATA_LENGTH (1000)
uint8_t enet_frame[ENET_DATA_LENGTH] __attribute__((aligned(64)));
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
  static ENET_Type* enet[] = {ENET, ENET2};
  unsigned int i = (unsigned int) arg;
  ENET_Type* e = (ENET_Type*) enet[i];

  uint32_t j = 0;
  while (true) {
///    uint32_t eir = ENET->EIR;
//    e->EIR = eir & ~(ENET_EIR_MII_MASK);
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

void enetTask(void* arg) {
  ENET_BuildBroadCastFrame();
  struct netif n;
  lwip_init(&n);

  static ENET_Type* enet[] = {ENET, ENET2};
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

//  xTaskCreate(sendTask, "send", 2*configMINIMAL_STACK_SIZE, (void*)0, tskIDLE_PRIORITY, NULL);
///  xTaskCreate(sendTask, "send", 2*configMINIMAL_STACK_SIZE, (void*)1, tskIDLE_PRIORITY, NULL);


  NVIC_SetPriority(ENET_IRQn, config_ENET_INTERRUPT_PRIORITY);
  NVIC_SetPriority(ENET2_IRQn, config_ENET_INTERRUPT_PRIORITY);
  NVIC_EnableIRQ(ENET_IRQn);
  NVIC_EnableIRQ(ENET2_IRQn);


  systemStart();

  while (1) {

    //    printf("counter: %d\n", counter); // FIXME
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
