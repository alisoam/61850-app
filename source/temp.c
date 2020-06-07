#include "board.h"
#include "peripherals.h"

void TEMPMON_LOW_HIGH_IRQHANDLER() {
}

void TEMPMON_PANIC_IRQHANDLER() {
  boardLedSet(BOARD_LED1, 1);
  __WFI();
}

