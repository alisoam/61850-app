#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

__attribute__((constructor)) static void init() {
}

int main() {
  puts("Programm Started...");
  printf("System clock is %lu\n", SystemCoreClock);

  vTaskStartScheduler();

  for (;;)
    ;
  return 1;
}
