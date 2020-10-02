/* Entry Point */
ENTRY(Reset_Handler)

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x0400;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0400;
VECTOR_RAM_SIZE = DEFINED(__ram_vector_table__) ? 0x00000400 : 0;

/* Specify the memory areas */
MEMORY
{
  m_itcm_vectors        (RX)  : ORIGIN = 0x00000000, LENGTH = 0x00000400
  m_itcm                (RX)  : ORIGIN = 0x00000400, LENGTH = 0x0001FC00
  m_dtcm                (RW)  : ORIGIN = 0x20000000, LENGTH = 0x00020000
  m_ocram               (RW)  : ORIGIN = 0x20200000, LENGTH = 0x00040000
  m_ocram_ncache        (RW)  : ORIGIN = 0x20040000, LENGTH = 0x00040000

  m_flash_vectors       (RX)  : ORIGIN = 0x60100000, LENGTH = 0x00000400
  m_flash               (RX)  : ORIGIN = 0x60100400, LENGTH = 0x006FFC00

  m_sdram_text          (RX)  : ORIGIN = 0x80000000, LENGTH = 0x00800000
  m_sdram               (RW)  : ORIGIN = 0x80800000, LENGTH = 0x00800000
  m_heap                (RW)  : ORIGIN = 0x81000000, LENGTH = 0x01000000
}

/* Define output sections */
SECTIONS
{
  __NCACHE_REGION_START = ORIGIN(m_ocram_ncache);
  __NCACHE_REGION_SIZE  = 0;

  __Vectors = LOADADDR(.vectors);
  __vectors_load_start__ = LOADADDR(.vectors);
  .vectors :
  {
    __VECTOR_TABLE = .;
    __vectors_start__ = .;
    . = ALIGN(4);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(4);
    __vectors_end__ = .;
  } > m_itcm_vectors AT> m_flash_vectors

  /* The program code and other data goes into internal RAM */
  .flash :
  {
    *(.reset_handler)
    *(.text.Reset_Handler .text.SystemInit .text.SystemInitHook \
      .rodata.Reset_Handler .rodata.SystemInit .rodata.SystemInitHook)
  } > m_flash

  __itcm_load_start__ = LOADADDR(.itcm);
  .itcm :
  {
    __itcm_start__ = .;
    . = ALIGN(4);
    *(.text.*_Handler)
    */board*(.text*)
    libdevice.a(.text*)
    lib_a-*(.text*)
    *crtbegin.o(.text* .iplt*)
    libFreeRTOS.a(.text*)
    *freertos_hooks*(.text*)
    *main.*(.text*)
    *syscall*(.text*)
    *swo*(.text*)
    *handlers*(.text*)
    */modules/FreeRTOS*(.text*)

    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
  } > m_itcm AT> m_flash

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
  } > m_itcm AT> m_flash

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_itcm AT> m_flash

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_itcm AT> m_flash

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_itcm AT> m_flash

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    __itcm_end__ = .;
  } > m_itcm AT> m_flash

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
    *main.*(.data* .rodata*)
    *syscalls.*(.data* .rodata*)
    */modules/FreeRTOS*(.data* .rodata*)
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_dtcm_end__ = .;
  } > m_dtcm AT> m_flash

  .ncache.init :
  {
    __noncachedata_start__ = .;   /* create a global symbol at ncache data start */
    *(NonCacheable.init)
    . = ALIGN(4);
    __noncachedata_init_end__ = .;   /* create a global symbol at initialized ncache data end */
  } > m_ocram_ncache
  . = __noncachedata_init_end__;
  .ncache :
  {
    *(NonCacheable)
    . = ALIGN(4);
    __noncachedata_end__ = .;     /* define a global symbol at ncache data end */
  } > m_ocram_ncache

  .bss_dtcm (NOLOAD):
  {
    . = ALIGN(4);
    __bss_dtcm_start__ = .;
    */board*(.data*)
    *(.bss.*_Handler)
    libdevice.a(.bss*)
    lib_a-*(.bss*)
    *crtbegin.o(.bss)
    libFreeRTOS.a(.bss*)
    *freertos_hooks*(.bss*)
    *main.*(.bss*)
    *syscalls.*(.bss*)
    */modules/FreeRTOS*(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_dtcm_end__ = .;
  } > m_dtcm

  .heap (NOLOAD):
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    __HeapBase = .;
    . += HEAP_SIZE;
    __HeapLimit = .;
    __heap_limit = .; /* Add for _sbrk */
  } > m_dtcm

  .stack (NOLOAD):
  {
    __StackTop = .;
    . = ALIGN(8);
    . += STACK_SIZE;
  } > m_dtcm

  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  .ARM.attributes 0 : { *(.ARM.attributes) }


  __text_load_start__ = LOADADDR(.text);
  .text :
  {
    . = ALIGN(4);
    __text_start__ = .;
    . = ALIGN(4);
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

  __data_load_start__ = LOADADDR(.data);
  .data :
  {
    . = ALIGN(4);
    __data_start__ = .;
    *(.data)
    *(.data*)
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
    __data_end__ = .;
  } > m_sdram AT> m_flash

 .bss (NOLOAD):
  {
    . = ALIGN(4);
    __bss_start__ = .;
    *(m_usb_dma_noninit_data)
    *(.bss)
    *(.bss*)
    . = ALIGN(4);
    __bss_end__ = .;
  } > m_sdram

}
