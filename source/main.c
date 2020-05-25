#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board.h"
#include "clock_config.h"
#include "fsl_device_registers.h"
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

__attribute__((constructor(101)))
static void setupHardware() {
  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_BootClockRUN();

//  stemCoreClockUpdate();
//  *((uint32_t *)(0x400E0600)) = (1 << 11);
  CLOCK_EnableClock(kCLOCK_Trace);
  SWO_Init();
}

static void mainTask(void *pvParameters)
{
  uint32_t c = 0;
  while (1) {
    extern void SWO_PrintString(const char *s);
    puts("tick");
    GPIO_PinWrite(GPIO3, 16, c & 1);
    GPIO_PinWrite(GPIO3, 12, c & 2);
    GPIO_PinWrite(GPIO3, 15, c & 4);
    c++;
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

int main()
{
  puts("Programm Started...");
  xTaskCreate(mainTask, "main", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
  vTaskStartScheduler();
  return 1;
}
