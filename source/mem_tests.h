#ifndef __MEM_TESTS_H_
#define __MEM_TESTS_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint32_t *start_addr;
    uint32_t bytes;
    uint32_t *fail_addr;
    uint32_t is_val;
    uint32_t ex_val;
} MEM_TEST_SETUP_T;

bool mem_test_walking0(MEM_TEST_SETUP_T *pMemSetup);

bool mem_test_walking1(MEM_TEST_SETUP_T *pMemSetup);

bool mem_test_address(MEM_TEST_SETUP_T *pMemSetup);

bool mem_test_invaddress(MEM_TEST_SETUP_T *pMemSetup);

bool mem_test_pattern(MEM_TEST_SETUP_T *pMemSetup);

bool mem_test_pattern_seed(MEM_TEST_SETUP_T *pMemSetup, uint32_t seed, uint32_t incr);

#endif /* __MEM_TESTS_H_ */
