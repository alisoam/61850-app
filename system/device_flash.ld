/* Entry Point */
ENTRY(Reset_Handler)

STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0400;
HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x01000000;

/* Specify the memory areas */
MEMORY
{
  m_itcm_ivt            (RX)  : ORIGIN = 0x00000000, LENGTH = 0x00000400
  m_itcm_text           (RX)  : ORIGIN = 0x00000400, LENGTH = 0x0003FC00
  m_dtcm                (RW)  : ORIGIN = 0x20000000, LENGTH = 0x00040000
  m_ocram               (RW)  : ORIGIN = 0x20200000, LENGTH = 0x00040000
  m_ocram_ncache        (RW)  : ORIGIN = 0x20040000, LENGTH = 0x00040000
  m_flash_ivt           (RX)  : ORIGIN = 0x60000000, LENGTH = 0x00000400
  m_flash               (RX)  : ORIGIN = 0x60000400, LENGTH = 0x007FFC00
  m_sdram_heap          (RW)  : ORIGIN = 0x80000000, LENGTH = 0x01000000
  m_sdram               (RW)  : ORIGIN = 0x81000000, LENGTH = 0x00800000
  m_sdram_text          (RX)  : ORIGIN = 0x81800000, LENGTH = 0x00800000
}

/* Define output sections */
SECTIONS
{
  __NCACHE_REGION_START = __noncachedata_start__;
  __NCACHE_REGION_SIZE = __noncachedata_end__ - __noncachedata_start__;
  .itcm_ivt :
  {
    __ivt_start__ = .;
    . = ALIGN(4);
    __RAM_VECTOR_TABLE = .;
    . += (__ivt_flash_end__ - __ivt_flash_start__);
    __ivt_end__ = .;
  } > m_itcm_ivt

  .ivt :
  {
    __ivt_flash_start__ = .;
    . = ALIGN(4);
    __VECTOR_TABLE = .;
    KEEP(*(.isr_vector))
    __ivt_flash_end__ = .;
  } AT> m_flash_ivt

  .flash_text :
  {
    . = ALIGN(4);
    *(.text.Reset_Handler)
  } AT> m_flash
  PROVIDE(__text_flash_start__ = LOADADDR(.itcm_text));
  .itcm_text :
  {
    __text_start__ = .;
    . = ALIGN(4);
    *(.text)                 /* .text sections (code) */
    *(.text*)                /* .text* sections (code) */
    *(.rodata)               /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
  } > m_itcm_text AT> m_flash

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > m_itcm_text AT> m_flash

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > m_itcm_text AT> m_flash

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
  } > m_itcm_text AT> m_flash

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_itcm_text AT> m_flash

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_itcm_text AT> m_flash

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_itcm_text AT> m_flash

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    __text_end__ = .;
  } > m_itcm_text AT> m_flash

  __etext = .;    /* define a global symbol at end of code */

  PROVIDE(__data_flash_start__ = LOADADDR(.data));
  .data :
  {
    __data_start__ = .;
    . = ALIGN(4);
    __DATA_RAM = .;

    *(m_usb_dma_init_data)
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_dtcm AT> m_flash

  /* Uninitialized data section */
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(4);
    __START_BSS = .;
    __bss_start__ = .;
    *(m_usb_dma_noninit_data)
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
    __END_BSS = .;
  } > m_dtcm


  .ncache :
  {
    __noncachedata_start__ = .;
    *(NonCacheable)
    . = ALIGN(4);
    __noncachedata_end__ = .;     /* define a global symbol at ncache data end */
  } > m_ocram_ncache

  .stack :
  {
    . = ALIGN(8);
    __end__ = .;
    __StackTop = .;
    . += STACK_SIZE;
    __StackLimit = .;
  } > m_dtcm
  PROVIDE(__stack = __StackTop);

  .ARM.attributes 0 : { *(.ARM.attributes) }

  .heap :
  {
    . = ALIGN(8);
    __HeapBase = .;
    . += HEAP_SIZE;
    __HeapLimit = .;
    __heap_limit = .; /* Add for _sbrk */
  } > m_sdram_heap

}

