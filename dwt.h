#ifndef DWT_H_
#define DWT_H_

#include <nrf.h>
#include <stdint.h>

// init DWT (Data Watchpoint and Trace unit) for cycle counting
void dwt_init(void);

// get current cycle count
uint32_t dwt_get_cycle(void);

// delay until the cycle counter reaches the specified target cycle
void dwt_delay_until_cycle(uint32_t cycle);

#endif // DWT_H_