#include "light.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrfx_saadc.h"
#include "microbit_v2.h"


#define LIGHT_aPIN_1 NRF_SAADC_INPUT_AIN4
#define LIGHT_aPIN_2 NRF_SAADC_INPUT_AIN6
#define LIGHT_aPIN_3 NRF_SAADC_INPUT_AIN7

#define LIGHT_gPIN_1 28
#define LIGHT_gPIN_2 30
#define LIGHT_gPIN_3 31

#define LIGHT_CHANNEL_1 1
#define LIGHT_CHANNEL_2 2
#define LIGHT_CHANNEL_3 3


nrfx_saadc_config_t saadc_config = {
    .resolution = NRF_SAADC_RESOLUTION_14BIT,
    .oversample = NRF_SAADC_OVERSAMPLE_DISABLED
};

#define LIGHT_CHANNEL_CONFIG(PIN_P) { \
.resistor_p = NRF_SAADC_RESISTOR_DISABLED, \
.resistor_n = NRF_SAADC_RESISTOR_DISABLED, \
.gain = NRF_SAADC_GAIN1_6, \
.reference = NRF_SAADC_REFERENCE_INTERNAL, \
.acq_time = NRF_SAADC_ACQTIME_10US, \
.mode = NRF_SAADC_MODE_SINGLE_ENDED, \
.burst = NRF_SAADC_BURST_DISABLED, \
.pin_p = (nrf_saadc_input_t)(PIN_P), \
.pin_n = NRF_SAADC_INPUT_DISABLED \
}

nrf_saadc_channel_config_t light_channel_1_config = LIGHT_CHANNEL_CONFIG(LIGHT_aPIN_1);
nrf_saadc_channel_config_t light_channel_2_config = LIGHT_CHANNEL_CONFIG(LIGHT_aPIN_2);
nrf_saadc_channel_config_t light_channel_3_config = LIGHT_CHANNEL_CONFIG(LIGHT_aPIN_3);


void led_init() {
    nrf_gpio_pin_dir_set(LED_ROW1, NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_dir_set(LED_ROW2, NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_dir_set(LED_ROW3, NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_dir_set(LED_ROW4, NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_dir_set(LED_ROW5, NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_dir_set(LED_COL1, NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_dir_set(LED_COL2, NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_dir_set(LED_COL3, NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_dir_set(LED_COL4, NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_dir_set(LED_COL5, NRF_GPIO_PIN_DIR_OUTPUT);

    nrf_gpio_pin_write(LED_ROW1, false);
    nrf_gpio_pin_write(LED_ROW2, false);
    nrf_gpio_pin_write(LED_ROW3, false);
    nrf_gpio_pin_write(LED_ROW4, false);
    nrf_gpio_pin_write(LED_ROW5, false);
    nrf_gpio_pin_write(LED_COL1, true);
    nrf_gpio_pin_write(LED_COL2, true);
    nrf_gpio_pin_write(LED_COL3, true);
    nrf_gpio_pin_write(LED_COL4, true);
    nrf_gpio_pin_write(LED_COL5, true);
}

void adc_init() {
    void saadc_event_callback(nrfx_saadc_evt_t const* _unused) {}
    nrfx_saadc_init(&saadc_config, saadc_event_callback);
    nrfx_saadc_channel_init(LIGHT_CHANNEL_1, &light_channel_1_config);
    nrfx_saadc_channel_init(LIGHT_CHANNEL_2, &light_channel_2_config);
    nrfx_saadc_channel_init(LIGHT_CHANNEL_3, &light_channel_3_config);
}

void light_init(void) {
    led_init();
    adc_init();
}

void adc_acquire(uint8_t col_pin) {
    nrf_gpio_pin_write(col_pin, true);
    nrf_gpio_pin_dir_set(col_pin, NRF_GPIO_PIN_DIR_INPUT);
    nrf_gpio_cfg_input(col_pin, NRF_GPIO_PIN_NOPULL);
    nrf_saadc_enable();
    adc_init();
}

void adc_release(uint8_t col_pin) {
    nrf_saadc_disable();
    nrf_gpio_pin_dir_set(col_pin, NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_write(col_pin, true);
}

uint8_t light_read_channel(uint8_t channel) {
    uint8_t col_pin;
    switch(channel) {
        case LIGHT_CHANNEL_1:
            col_pin = LIGHT_gPIN_1;
            break;
        case LIGHT_CHANNEL_2:
            col_pin = LIGHT_gPIN_2;
            break;
        case LIGHT_CHANNEL_3:
            col_pin = LIGHT_gPIN_3;
            break;
        default:
            return 0;
    }

    adc_acquire(col_pin);
    nrf_delay_ms(4);

    int16_t adc_counts = 0;
    nrfx_saadc_sample_convert(channel, &adc_counts);
    uint8_t light_level = (adc_counts * -255) / 16384 + 255; 

    adc_release(col_pin);

    return light_level;
}

uint8_t light_read() {
    uint16_t sum = 0;
    sum += light_read_channel(LIGHT_CHANNEL_1);
    sum += light_read_channel(LIGHT_CHANNEL_2);
    sum += light_read_channel(LIGHT_CHANNEL_3);
    return sum / 3;
}

bool light_read_bool() {
    return light_read() > 60;
}
