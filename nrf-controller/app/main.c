#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "gpio.h"
#include "rng.h"
#include "microbit_v2.h"
#include "nrf_twi_mngr.h"
#include "accl.h"

NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);

int main(void) {
    printf("Board started!\n");
    nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
    i2c_config.scl = I2C_INTERNAL_SCL;
    i2c_config.sda = I2C_INTERNAL_SDA;
    i2c_config.frequency = NRF_TWIM_FREQ_100K;
    i2c_config.interrupt_priority = 0;
    nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
    lsm303agr_init(&twi_mngr_instance);

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
        lsm303agr_measurement_t tilt = lsm303agr_read_tilt();
        R = rng_read();

        printf("%u,%u,%06.2f,%06.2f,%06.2f\n", A, B, tilt.x_axis, tilt.y_axis, tilt.z_axis);
        // printf("%u,%u,%u\n", A, B, R);
        nrf_delay_ms(10);
    }
}