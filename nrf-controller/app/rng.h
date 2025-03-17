#include <stdbool.h>
#include <stdint.h>


void rng_init(bool enable_bias_correction);

void rng_start();

uint8_t rng_read();

void rng_stop();
