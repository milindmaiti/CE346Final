// Blink app
//
// Blinks an LED

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

// Pin configurations
#include "microbit_v2.h"

int main(void) {

  // Initialize.
  nrf_gpio_cfg_output(LED_MIC);

  // Enter main loop.

  int cur = 0;
  while (1) {
    printf ("Cur is: %d\n", cur);
    nrf_gpio_pin_toggle(LED_MIC);
    nrf_delay_ms(500);
    cur++;
  }
}
