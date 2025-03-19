#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_twi_mngr.h"
#include "gpio.h"
#include "microbit_v2.h"
#include "nrf_delay.h"

#include "accl.h"
#include "light.h"
#include "rng.h"
#include "capacitive_touch.h"

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
    printf("i2c config done\n");

    gpio_print();
    uint8_t buttonA = 14;
    uint8_t buttonB = 23;
    gpio_config(buttonA, GPIO_INPUT);
    gpio_config(buttonB, GPIO_INPUT);
    printf("gpio config done\n");

    rng_init(true);
    printf("rng config done\n");

    int A, B, C;
    uint8_t light, random;
    lsm303agr_measurement_t tilt;

    rng_start();

    capacitive_touch_init();
    while (1) {
        A = gpio_read(buttonA) == 0;
        B = gpio_read(buttonB) == 0;
        C = capacitive_touch_is_active();
        tilt = lsm303agr_read_tilt();
        light = light_read_bool(); // takes about ~12 microseconds
        random = rng_read();

        printf("%u,%u,%u,%06.2f,%06.2f,%06.2f,%u,%03u\n", A, B, C, tilt.x_axis, tilt.y_axis, tilt.z_axis, light, random);
        // removed delay since reading light sensor causes a delay
        // nrf_delay_ms(10);
    }
}