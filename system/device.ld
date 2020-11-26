ENTRY(Reset_Handler)

HEAP_SIZE  = LENGTH(m_sdram_heap);
STACK_SIZE = 0x800;
VECTOR_RAM_SIZE = 0x400;

MEMORY
{
  m_itcm_vectors        (RX)  : ORIGIN = 0x00000000, LENGTH = 0x00000400
  m_itcm                (RX)  : ORIGIN = 0x00000400, LENGTH = 0x0003FC00
  m_dtcm                (RW)  : ORIGIN = 0x20000000, LENGTH = 0x00040000
  m_ocram               (RW)  : ORIGIN = 0x20200000, LENGTH = 0x00080000

  m_flash_vectors       (RX)  : ORIGIN = 0x60100000, LENGTH = 0x00000400
  m_flash               (RX)  : ORIGIN = 0x60100400, LENGTH = 0x006FFC00

  m_sdram_text          (RX)  : ORIGIN = 0x80000000, LENGTH = 0x00800000
  m_sdram               (RW)  : ORIGIN = 0x80800000, LENGTH = 0x00800000
  m_sdram_heap          (RW)  : ORIGIN = 0x81000000, LENGTH = 0x01000000
}

SECTIONS
{
  __NCACHE_REGION_START = __noncachedata_start__;
  __NCACHE_REGION_SIZE  = __noncachedata_end__ - __noncachedata_start__;

  __Vectors = LOADADDR(.vectors);
  __vectors_load_start__ = LOADADDR(.vectors);
  .vectors :
  {
    . = ALIGN(4);
    __VECTOR_TABLE = .;
    __vectors_start__ = .;

    KEEP(*(.isr_vector))

    . = ALIGN(4);
    __vectors_end__ = .;
  } > m_itcm_vectors AT> m_flash_vectors

  .text_flash :
  {
    . = ALIGN(4);

    *(.reset_handler)
    *(.text.Reset_Handler .text.SystemInit .text.SystemInitHook \
      .rodata.Reset_Handler .rodata.SystemInit .rodata.SystemInitHook)

    . = ALIGN(4);
  } > m_flash

  __text_itcm_load_start__ = LOADADDR(.text_itcm);
  .text_itcm :
  {
    . = ALIGN(4);
    __text_itcm_start__ = .;

    *(.text.*_Handler)
    */board*(.text*)
    libdevice.a(.text*)
    lib_a-*(.text*)
    *crtbegin.o(.text* .iplt*)
    libFreeRTOS.a(.text*)
    *freertos_hooks*(.text*)
    liblwIP.a(.text*)
    *syscall*(.text*)
    *handlers*(.text*)
    */modules/FreeRTOS*(.text*)
    */source/net*(.text*)
    *(.text*IRQHandler*)

    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4); 
    __text_itcm_end__ = .;
  } > m_itcm AT> m_flash

  __text_load_start__ = LOADADDR(.text);
  .text :
  {
    . = ALIGN(4);
    __text_start__ = .;

    *(.text .text*)

  } > m_sdram_text AT> m_flash

  .ctors :
  {
    __CTOR_LIST__ = .;
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    __CTOR_END__ = .;
  } > m_sdram_text AT> m_flash

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_sdram_text AT> m_flash

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_sdram_text AT> m_flash

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_sdram_text AT> m_flash

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > m_sdram_text AT> m_flash

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > m_sdram_text AT> m_flash

  .ARM :
  {
    __exidx_start = .;

    *(.ARM.exidx*)

    __exidx_end = .;
    __text_end__ = .;
  } > m_sdram_text AT> m_flash

  .ncache.init :
  {
    __noncachedata_start__ = .;
    *(NonCacheable.init)
    . = ALIGN(4);
    __noncachedata_init_end__ = .;
  } > m_ocram
  . = __noncachedata_init_end__;
  .bss_ncache (NOLOAD):
  {
    . = ALIGN(4);
    *(.bss_ncache)
    *(.enet_bss .enet_bss*)
    *(NonCacheable)
    . = ALIGN(4);
    __noncachedata_end__ = .;
  } > m_ocram

  __data_dtcm_load_start__ = LOADADDR(.data_dtcm);
  .data_dtcm :
  {
    . = ALIGN(4);
    __data_dtcm_start__ = .;

    */board*(.data* .rodata*)
    *(.data.*_Handler .rodata.*_Handler)
    libdevice.a(.data* .rodata*)
    lib_a-*(.data* .rodata*)
    *crtbegin.o(.igot.plt .rel.iplt)
    libFreeRTOS.a(.data* .rodata*)
    *freertos_hooks*(.data* .rodata*)
    liblwIP.a(.data* .rodata*)
    *syscalls.*(.data* .rodata*)
    */modules/FreeRTOS*(.data* .rodata*)
    */source/net*(.data* .rodata*)
    *(.data*IRQHandler* .rodata*IRQHandler*)
    KEEP(*(.jcr*))

    *(.data_dtcm .data_dtcm*)

    . = ALIGN(4);
    __data_dtcm_end__ = .;
  } > m_dtcm AT> m_flash

  .bss_dtcm (NOLOAD):
  {
    . = ALIGN(4);
    __bss_dtcm_start__ = .;

    */board*(.bss*)
    *(.bss.*_Handler)
    libdevice.a(.bss*)
    lib_a-*(.bss*)
    *crtbegin.o(.bss)
    libFreeRTOS.a(.bss*)
    *freertos_hooks*(.bss*)
    liblwIP.a(.bss*)
    *syscalls.*(.bss*)
    */modules/FreeRTOS*(.bss*)
    */source/net*(.bss*)
    *(.bss*_IRQHandler*)
    *(COMMON)

    *(.bss_dtcm .bss_dtcm.*)

    . = ALIGN(4);
    __bss_dtcm_end__ = .;
  } > m_dtcm

  __data_load_start__ = LOADADDR(.data);
  .data :
  {
    . = ALIGN(4);
    __data_start__ = .;

    *(.data)
    *(.data*)
    *(.rodata .rodata*)

    . = ALIGN(4);
    __data_end__ = .;
  } > m_sdram AT> m_flash

 .bss (NOLOAD):
  {
    . = ALIGN(4);
    __bss_start__ = .;

    *(.bss)
    *(.bss*)

    . = ALIGN(4);
    __bss_end__ = .;
  } > m_sdram

  .heap (NOLOAD):
  {
    . = ALIGN(8);
    __heap_base__ = .;
    . += HEAP_SIZE;
    __heap_limit__ = .;
  } > m_sdram_heap

  .stack (NOLOAD):
  {
    . = ALIGN(8);
    . += STACK_SIZE;
    . = ALIGN(8);
    __StackTop = .;
    _stack_top = .;
  } > m_dtcm

  PROVIDE(__stack = __StackTop);
}
