#include <stdio.h>

#include "FreeRTOS.h"

int main() {
  puts("Programm Started...");
  printf("System clock is %lu\n", SystemCoreClock);

  vTaskStartScheduler();

  for (;;)
    ;
  return 1;
}
