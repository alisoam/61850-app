#include <stdio.h>

#include "board.h"


void HardFault_Handler() {
  for (;;) {
    boardLedSet(BOARD_LED1, 1); 
    for (volatile unsigned int j = 0; j < 4000000; j++)
      ;
    boardLedSet(BOARD_LED1, 0);
    for (volatile unsigned int j = 0; j < 4000000; j++)
      ;
  }
}

void BoardInit() {
  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitBootPeripherals();
}

void Reset_Handler() {
  asm ("cpsid i");

  extern void SystemInit();
  SystemInit();

  unsigned int* VTOR = (unsigned int *) 0xE000ED08;
  extern void* __isr_vector;
  *VTOR = (unsigned int) &__isr_vector;

  unsigned int* startPointer,
              * endPointer,
              * loudPointer;

  extern unsigned int __vectors_load_start__,
                      __vectors_start__,
                      __vectors_end__;
  loudPointer = &__vectors_load_start__;
  startPointer = &__vectors_start__;
  endPointer = &__vectors_end__;
  while (startPointer < endPointer) {
    *startPointer = *loudPointer;
    loudPointer++;
    startPointer++;
  }

  extern unsigned int __itcm_load_start__,
                      __itcm_start__,
                      __itcm_end__;
  loudPointer = &__itcm_load_start__;
  startPointer = &__itcm_start__;
  endPointer = &__itcm_end__;
  while (startPointer < endPointer) {
    *startPointer = *loudPointer;
    loudPointer++;
    startPointer++;
  }

  extern unsigned int __data_dtcm_load_start__,
                      __data_dtcm_start__,
                      __data_dtcm_end__;
  loudPointer = &__data_dtcm_load_start__;
  startPointer = &__data_dtcm_start__;
  endPointer = &__data_dtcm_end__;
  while (startPointer < endPointer) {
    *startPointer = *loudPointer;
    loudPointer++;
    startPointer++;
  }

  extern unsigned int __bss_dtcm_start__,
                      __bss_dtcm_end__;
  startPointer = &__bss_dtcm_start__;
  endPointer = &__bss_dtcm_end__;
  while (startPointer < endPointer) {
    *startPointer = 0;
    startPointer++;
  }

  BoardInit();

  extern unsigned int __text_load_start__,
                      __text_start__,
                      __text_end__;
  loudPointer = &__text_load_start__;
  startPointer = &__text_start__;
  endPointer = &__text_end__;
  while (startPointer < endPointer) {
    *startPointer = *loudPointer;
    loudPointer++;
    startPointer++;
  }

  extern unsigned int __data_load_start__,
                      __data_start__,
                      __data_end__;
  loudPointer = &__data_load_start__;
  startPointer = &__data_start__;
  endPointer = &__data_end__;
  while (startPointer < endPointer) {
    *startPointer = *loudPointer;
    loudPointer++;
    startPointer++;
  }

  extern unsigned int __bss_start__,
                      __bss_end__;
  startPointer = &__bss_start__;
  endPointer = &__bss_end__;
  while (startPointer < endPointer) {
    *startPointer = 0;
    startPointer++;
  }

  asm ("cpsie i");
  extern void _start();
  _start();
}
