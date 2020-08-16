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

/********************************
*********************/

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

  extern void mem_test();
//  mem_test();


//  xTaskCreate(mainTask, "main", 4 * configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
  extern void enetTask();
  xTaskCreate(enetTask, "enet", 4*configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);

  vTaskStartScheduler();
  return 1;
}
