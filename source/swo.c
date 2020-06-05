#include "board.h"

void SWO_Init() {
  uint32_t SWOSpeed = 64000;
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
