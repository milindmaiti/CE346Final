#include <stdint.h>
#include <stdbool.h>


void light_init(void);

uint8_t light_read_channel(uint8_t channel);

uint8_t light_read();

bool light_read_bool();