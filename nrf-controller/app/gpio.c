#include "gpio.h"
#include <stdint.h>
#include <stdio.h>

typedef struct {
  uint32_t padding1[321];
  uint32_t OUT;
  uint32_t OUTSET;
  uint32_t OUTCLR;
  uint32_t IN;
  uint32_t DIR;
  uint32_t DIRSET;
  uint32_t DIRCLR;
  uint32_t LATCH;
  uint32_t DETECTMODE;
  uint32_t padding2[118];
  uint32_t PIN_CNF[32];
  // Step 3:
  // Add register definitions here
} gpio_reg_t;

volatile gpio_reg_t *port0 = (gpio_reg_t *)0x50000000;
volatile gpio_reg_t *port1 = (gpio_reg_t *)0x50000300;
// Inputs:
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
//  dir - gpio direction (INPUT, OUTPUT)
void gpio_config(uint8_t gpio_num, gpio_direction_t dir) {
  // Implement me
  // This function should configure the pin as an input/output
  // Hint: Use proper PIN_CNF instead of DIR
  if (gpio_num >= 32) {
    if (dir == GPIO_OUTPUT) {
      port1->PIN_CNF[gpio_num - 32] |= (1 << 0);
      port1->PIN_CNF[gpio_num - 32] |= (1 << 1);
    } else {
      port1->PIN_CNF[gpio_num - 32] &= ~(1 << 0);
      port1->PIN_CNF[gpio_num - 32] &= ~(1 << 1);
    }
  } else {
    if (dir == GPIO_OUTPUT) {
      port0->PIN_CNF[gpio_num] |= (1 << 0);
      port0->PIN_CNF[gpio_num] |= (1 << 1);
    } else {
      port0->PIN_CNF[gpio_num] &= ~(1 << 0);
      port0->PIN_CNF[gpio_num] &= ~(1 << 1);
    }
  }
}

// Inputs:
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
void gpio_set(uint8_t gpio_num) {
  // Implement me
  // This function should make the pin high
  // It can assume that the pin has already been configured
  if (gpio_num >= 32) {
    gpio_num -= 32;
    port1->OUT |= (1 << gpio_num);
  } else {
    port0->OUT |= (1 << gpio_num);
  }
}

// Inputs:
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
void gpio_clear(uint8_t gpio_num) {
  // Implement me
  // This function should make the pin low
  // It can assume that the pin has already been configured
  if (gpio_num >= 32) {
    gpio_num -= 32;
    port1->OUT &= ~(1 << gpio_num);
  } else {
    port0->OUT &= ~(1 << gpio_num);
  }
}

// Inputs:
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
// Output:
//  bool - pin state (true == high)
bool gpio_read(uint8_t gpio_num) {
  // Implement me
  // This function should read the value from the pin
  // It can assume that the pin has already been configured
  if (gpio_num >= 32) {
    gpio_num -= 32;
    return (port1->IN & (1 << gpio_num));
  } else {
    return (port0->IN & (1 << gpio_num));
  }
}

// prints out some information about the GPIO driver. Can be called from main()
void gpio_print(void) {
  printf("%p\n", &(port0->OUT));
  printf("%p\n", &(port0->PIN_CNF));
  // Use this function for debugging purposes
  // For example, you could print out struct field addresses
  // You don't otherwise have to write anything here
}
