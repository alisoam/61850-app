#include "FreeRTOS.h"
#include "task.h"

void appMain() {
  extern void iec61850Main();
 iec61850Main();

 for (;;) {
 }
}

void startApp() {
  xTaskCreate(appMain, "app", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
}
