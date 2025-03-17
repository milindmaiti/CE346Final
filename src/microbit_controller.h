#ifndef MICROBIT_CONTROLLER_H
#define MICROBIT_CONTROLLER_H

#include <stdbool.h>

// Initialize connection to micro:bit
bool microbit_init(const char* port);

// Close connection
void microbit_close(void);

// Update all sensor readings (call once per frame)
bool microbit_update(void);

// Button state getters
bool microbit_button_a(void);
bool microbit_button_b(void);

// Accelerometer data getters
float microbit_accel_x(void);
float microbit_accel_y(void);
float microbit_accel_z(void);

// Raw data access if needed
typedef struct {
    bool button_a;
    bool button_b;
    float accel_x;
    float accel_y;
    float accel_z;
    // Add more sensor data as needed
} MicrobitState;

const MicrobitState* microbit_get_state(void);

#endif