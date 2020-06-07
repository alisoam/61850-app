#include <stdlib.h>

void HardFault_Handler() {
  puts("\n\n\n>>>Hard Fault<<<\n");
  for (volatile unsigned int i;;)
    ;
}
