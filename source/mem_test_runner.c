#include <stdio.h>

#include "mem_tests.h"

#define DRAM_BASE_ADDRESS (uint32_t *)(0x80000000)
#define DRAM_SIZE (32 * 1024 * 1024)
#define DEBUGOUT printf
#define DEBUGSTR printf

void mem_test()
{
  MEM_TEST_SETUP_T memSetup;

  /* Walking 0 test */
  memSetup.start_addr = DRAM_BASE_ADDRESS;
  memSetup.bytes = DRAM_SIZE;
  if (mem_test_walking0(&memSetup))
  {
    DEBUGSTR("Walking 0 memory test passed\n");
  }
  else
  {
    DEBUGOUT("Walking 0 memory test failed at address %p\n", memSetup.fail_addr);
    DEBUGOUT(" Expected %08x, actual %08x\n", memSetup.ex_val, memSetup.is_val);
  }

  /* Walking 1 test */
  memSetup.start_addr = DRAM_BASE_ADDRESS;
  memSetup.bytes = DRAM_SIZE;
  if (mem_test_walking1(&memSetup))
  {
    DEBUGSTR("Walking 1 memory test passed\n");
  }
  else
  {
    DEBUGOUT("Walking 1 memory test failed at address %p\n", memSetup.fail_addr);
    DEBUGOUT(" Expected %08x, actual %08x\n", memSetup.ex_val, memSetup.is_val);
  }

  /* Address test */
  memSetup.start_addr = DRAM_BASE_ADDRESS;
  memSetup.bytes = DRAM_SIZE;
  if (mem_test_address(&memSetup))
  {
    DEBUGSTR("Address test passed\n");
  }
  else
  {
    DEBUGOUT("Address test failed at address %p\n", memSetup.fail_addr);
    DEBUGOUT(" Expected %08x, actual %08x\n", memSetup.ex_val, memSetup.is_val);
  }

  /* Inverse address test */
  memSetup.start_addr = DRAM_BASE_ADDRESS;
  memSetup.bytes = DRAM_SIZE;
  if (mem_test_invaddress(&memSetup))
  {
    DEBUGSTR("Inverse address test passed\n");
  }
  else
  {
    DEBUGOUT("Inverse address test failed at address %p\n", memSetup.fail_addr);
    DEBUGOUT(" Expected %08x, actual %08x\n", memSetup.ex_val, memSetup.is_val);
  }

  /* Pattern test */
  memSetup.start_addr = DRAM_BASE_ADDRESS;
  memSetup.bytes = DRAM_SIZE;
  if (mem_test_pattern(&memSetup))
  {
    DEBUGSTR("Pattern (0x55/0xAA) test passed\n");
  }
  else
  {
    DEBUGOUT("Pattern (0x55/0xAA) test failed at address %p\n", memSetup.fail_addr);
    DEBUGOUT(" Expected %08x, actual %08x\n", memSetup.ex_val, memSetup.is_val);
  }

  /* Seeded pattern test */
  memSetup.start_addr = DRAM_BASE_ADDRESS;
  memSetup.bytes = DRAM_SIZE;
  if (mem_test_pattern_seed(&memSetup, 0x12345678, 0x50005))
  {
    DEBUGSTR("Seeded pattern test passed\n");
  }
  else
  {
    DEBUGOUT("Seeded pattern test failed at address %p\n", memSetup.fail_addr);
    DEBUGOUT(" Expected %08x, actual %08x\n", memSetup.ex_val, memSetup.is_val);
  }
}
