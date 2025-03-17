#include "rng.h"


typedef struct {
    uint32_t TASKS_START;       // 0x000 Task starting the random number generator
    uint32_t TASKS_STOP;        // 0x004 Task stopping the random number generator
    uint32_t UNUSED_1[62];      // 0x008 to 0x100
    uint32_t EVENTS_VALRDY;     // 0x100 Event for new random number written to VALUE register
    uint32_t UNUSED_2[63];      // 0x104 to 0x200
    uint32_t SHORTS;            // 0x200 Shortcuts between local events and tasks
    uint32_t UNUSED_3[64];      // 0x204 to 0x304
    uint32_t INTENSET;          // 0x304 Enable interrupt
    uint32_t INTENCLR;          // 0x308 Disable interrupt
    uint32_t UNUSED_4[126];     // 0x30C to 0x504
    uint32_t CONFIG;            // 0x504 Configuration register
    uint8_t VALUE;              // 0x508 Output random number
} RNG;

volatile RNG *rng = (RNG*)0x4000D000;


void rng_init(bool enable_bias_correction) {
    rng->EVENTS_VALRDY = 0;
    rng->SHORTS = 0;
    rng->INTENSET = 0;
    rng->CONFIG = enable_bias_correction;
}

void rng_start() {
    rng->TASKS_START = 1;
}

uint8_t rng_read() {
    return rng->VALUE;
}

void rng_stop() {
    rng->TASKS_STOP = 1;
}
