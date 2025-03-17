// GPIO app
//
// Uses MMIO to control GPIO pins

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"

#include "gpio.h"
#include "microbit_v2.h"

int main(void) {
    printf("Board started!\n");
    gpio_print();

    uint8_t buttonA = 14;
    uint8_t buttonB = 23;
    gpio_config(buttonA, GPIO_INPUT);
    gpio_config(buttonB, GPIO_INPUT);
    int A, B;
    while (1) {
        A = gpio_read(buttonA) == 0;
        B = gpio_read(buttonB) == 0;

        printf("%u,%u\n", A, B);
        nrf_delay_ms(10);
    }
}