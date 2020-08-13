#include <malloc.h>
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
#include "peripherals.h"
#include "pin_mux.h"

emac_data_t emac __attribute__ ((section (".fast_bss"))) __attribute__((aligned(4)));

void SystemInitHook() {
  IOMUXC_GPR->GPR17 = 0xaaaaffff;
  IOMUXC_GPR->GPR16 |= IOMUXC_GPR_GPR16_FLEXRAM_BANK_CFG_SEL_MASK;
}

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
void pDelayMs(uint32_t ms) {
  vTaskDelay(ms / 5);
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
  uint32_t physts;
  static int prt_ip = 0;
  tcpip_init(tcpip_init_done_signal, (void *) &tcpipdone);
  while (!tcpipdone)
    vTaskDelay(1);
#if LWIP_DHCP
  IP4_ADDR(&gw, 0, 0, 0, 0);
  IP4_ADDR(&ipaddr, 0, 0, 0, 0);
  IP4_ADDR(&netmask, 0, 0, 0, 0);
#else
  IP4_ADDR(&gw, 192, 168, 30, 101);
  IP4_ADDR(&ipaddr, 192, 168, 30, 101);
  IP4_ADDR(&netmask, 255, 255, 255, 0);
#endif
  memset(netif, 0, sizeof(struct netif));
  if (!netif_add(netif, &ipaddr, &netmask, &gw, NULL, interface_init, NULL))
  {
    puts("Net interface failed to initialize\r\n");
    while(1);
  }
}

static void enetTask(void* arg) {
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

  NVIC_SetPriority(ENET_IRQn, config_ENET_INTERRUPT_PRIORITY);
  NVIC_SetPriority(ENET2_IRQn, config_ENET_INTERRUPT_PRIORITY);
  NVIC_EnableIRQ(ENET_IRQn);
  NVIC_EnableIRQ(ENET2_IRQn);


  printf("EIMR: %x\n", ENET->EIMR); // FIXME
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
    vTaskDelay(5 * configTICK_RATE_HZ);
    float temp = TEMPMON_GetCurrentTemperature(TEMPMON);
    printf("Still Alive. Temp: %.1f\n", temp);
  }
}

int main()
{
  puts("Programm Started...");
  printf("System clock is %lu\n", SystemCoreClock);
  printf("GPR17: %x\n", IOMUXC_GPR->GPR17);
  printf("GPR16: %x\n", IOMUXC_GPR->GPR16);

  extern void mem_test();
//  mem_test();


  xTaskCreate(mainTask, "main", 4 * configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
  xTaskCreate(enetTask, "enet", 2*configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);

  vTaskStartScheduler();
  return 1;
}
