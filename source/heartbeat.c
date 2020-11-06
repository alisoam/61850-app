#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#include "board.h"


static void blinky(void* arg) {
  for (;;) {
    boardLedSet(BOARD_LED1, 0);
    vTaskDelay(2 * configTICK_RATE_HZ);
    boardLedSet(BOARD_LED1, 1);
    vTaskDelay(30 / portTICK_PERIOD_MS);
  }
}

__attribute__((constructor(101))) static void init() {
  xTaskCreate(blinky, "heartbeat", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
}
