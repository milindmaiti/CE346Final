#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "gpio.h"
#include "rng.h"
#include "microbit_v2.h"


int main(void) {
    printf("Board started!\n");
    gpio_print();

    uint8_t buttonA = 14;
    uint8_t buttonB = 23;
    gpio_config(buttonA, GPIO_INPUT);
    gpio_config(buttonB, GPIO_INPUT);
    rng_init(true);

    int A, B;
    uint8_t R;

    rng_start();
    while (1) {
        A = gpio_read(buttonA) == 0;
        B = gpio_read(buttonB) == 0;
        R = rng_read();

        printf("%u,%u,%u\n", A, B, R);
        nrf_delay_ms(10);
    }
}