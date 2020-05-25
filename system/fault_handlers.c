#include <stdlib.h>

void HardFault_Handler() {
//  my_uart_puts("\n\n\n>>>Hard Fault<<<\n");
  for (volatile unsigned int i;;)
    ;
}
