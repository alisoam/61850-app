#include <stddef.h>

void startup();
void main();
void NMI_Handler();
void HardFault_Handler();
void MemManage_Handler();
void BusFault_Handler();
void UsageFault_Handler();
void UnHandled_Vector();
void SVC_Handler();
void DebugMon_Handler();
void PendSV_Handler();
void SysTick_Handler();
void UART_Handler();
void ENET0_Handler();
void ENET1_Handler();

extern unsigned int _stack_top;
unsigned int* __vector_table[]
__attribute__ ((section("vectors")))
__attribute__((__used__))
= {
  (unsigned int *)    &_stack_top,                    // 0 Top of Stack
  (unsigned int *)    startup,                        // 1 Reset Handler
  (unsigned int *)    NMI_Handler,                    // 2 NMI Handler
  (unsigned int *)    HardFault_Handler,              // 3 Hard Fault Handler
  (unsigned int *)    MemManage_Handler,              // 4 MPU Fault Handler
  (unsigned int *)    BusFault_Handler,               // 5 Bus Fault Handler
  (unsigned int *)    UsageFault_Handler,             // 6 Usage Fault Handler
  (unsigned int *)    UnHandled_Vector,               // 7 Reserved
  (unsigned int *)    UnHandled_Vector,               // 8 Reserved
  (unsigned int *)    UnHandled_Vector,               // 9 Reserved
  (unsigned int *)    UnHandled_Vector,               // 10 Reserved
  (unsigned int *)    SVC_Handler,                    // 11 SVCall Handler
  (unsigned int *)    DebugMon_Handler,               // 12 Debug Monitor Handler
  (unsigned int *)    UnHandled_Vector,               // 13 Reserved
  (unsigned int *)    PendSV_Handler,                 // 14 PendSV Handler
  (unsigned int *)    SysTick_Handler,                // 15 SysTick Handler
  (unsigned int *)    UART_Handler,                   // 16 SysTick Handler
  (unsigned int *)    ENET0_Handler,                  // 17 ENET0 Handler
  (unsigned int *)    ENET1_Handler,                  // 18 ENET1 Handler
};

void startup()
{
  extern void SystemInit();
  SystemInit();

  extern unsigned int _start_fast_text_flash;
  unsigned int * fast_text_rom_start_p = &_start_fast_text_flash;
  extern unsigned int _start_fast_text;
  unsigned int * fast_text_ram_start_p = &_start_fast_text;
  extern unsigned int _end_fast_text;
  unsigned int * fast_text_ram_end_p = &_end_fast_text;
  while(fast_text_ram_start_p != fast_text_ram_end_p)
  {
    *fast_text_ram_start_p = *fast_text_rom_start_p;
    fast_text_ram_start_p++;
    fast_text_rom_start_p++;
  }

  extern unsigned int _start_fast_bss;
  unsigned int * fast_bss_start_p = &_start_fast_bss;
  extern unsigned int _end_fast_bss;
  unsigned int * fast_bss_end_p = &_end_fast_bss;
  while(fast_bss_start_p != fast_bss_end_p)
  {
    *fast_bss_start_p = 0;
    fast_bss_start_p++;
  }

  extern unsigned int _start_fast_data_flash;
  unsigned int * fast_data_rom_start_p = &_start_fast_data_flash;
  extern unsigned int _start_fast_data;
  unsigned int * fast_data_ram_start_p = &_start_fast_data;
  extern unsigned int _end_fast_data;
  unsigned int * fast_data_ram_end_p = &_end_fast_data;
  while(fast_data_ram_start_p != fast_data_ram_end_p)
  {
    *fast_data_ram_start_p = *fast_data_rom_start_p;
    fast_data_ram_start_p++;
    fast_data_rom_start_p++;
  }

  extern unsigned int _start_bss;
  unsigned int * bss_start_p = &_start_bss;
  extern unsigned int _end_bss;
  unsigned int * bss_end_p = &_end_bss;
  while(bss_start_p != bss_end_p)
  {
    *bss_start_p = 0;
    bss_start_p++;
  }

  extern unsigned int _start_data_flash;
  unsigned int * data_rom_start_p = &_start_data_flash;
  extern unsigned int _start_data;
  unsigned int * data_ram_start_p = &_start_data;
  extern unsigned int _end_data;
  unsigned int * data_ram_end_p = &_end_data;
  while(data_ram_start_p != data_ram_end_p)
  {
    *data_ram_start_p = *data_rom_start_p;
    data_ram_start_p++;
    data_rom_start_p++;
  }

  extern void __syscalls_system_init();
  __syscalls_system_init();

  main();

  for (volatile unsigned int i;;)
    ;
}

__attribute__((weak)) void NMI_Handler()
{
  for (volatile unsigned int i;;)
    ;
}

__attribute__((weak)) void HardFault_Handler()
{
  for (volatile unsigned int i;;)
    ;
}

__attribute__((weak)) void MemManage_Handler()
{
  for (volatile unsigned int i;;)
    ;
}

__attribute__((weak)) void BusFault_Handler()
{
  for (volatile unsigned int i;;)
    ;
}

__attribute__((weak)) void UsageFault_Handler()
{
  for (volatile unsigned int i;;)
    ;
}

__attribute__((weak)) void UnHandled_Vector()
{
  for (volatile unsigned int i;;)
    ;
}

__attribute__((weak)) void SVC_Handler()
{
  for (volatile unsigned int i;;)
    ;
}

__attribute__((weak)) void DebugMon_Handler()
{
  for (volatile unsigned int i;;)
    ;
}

__attribute__((weak)) void PendSV_Handler()
{
  for (volatile unsigned int i;;)
    ;
}

__attribute__((weak)) void SysTick_Handler()
{
  for (volatile unsigned int i;;)
    ;
}

__attribute__((weak)) void UART_Handler()
{
  for (volatile unsigned int i;;)
    ;
}

__attribute__((weak)) void ENET0_Handler()
{
  for (volatile unsigned int i;;)
    ;
}

__attribute__((weak)) void ENET1_Handler()
{
  for (volatile unsigned int i;;)
    ;
}
