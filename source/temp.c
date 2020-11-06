#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board.h"
#include "peripherals.h"

void TEMPMON_LOW_HIGH_IRQHANDLER() {
}

void TEMPMON_PANIC_IRQHANDLER() {
}

static void tempTask(void *pvParameters)
{
  while (true) {
    vTaskDelay(5 * configTICK_RATE_HZ);
    float temp = TEMPMON_GetCurrentTemperature(TEMPMON);
    printf("Still Alive. Temp: %.1f\n", temp);
  }
}

__attribute__((constructor)) static void setup() {
  xTaskCreate(tempTask, "temp", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
}
