#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

__attribute__((constructor(101))) void init() {
  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitBootPeripherals();
}

int main() {
  puts("Programm Started...");
  printf("System clock is %lu\n", SystemCoreClock);

  vTaskStartScheduler();

  for (;;)
    ;
  return 1;
}
