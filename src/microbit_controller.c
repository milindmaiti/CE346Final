#include "microbit_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#define MSG_START_BYTE 0xAA
#define MSG_END_BYTE 0xFF
#define PACKET_SIZE 9  // Start + 1 button byte + 6 accel bytes + End

static int serial_fd = -1;
static MicrobitState current_state = {0};
static uint8_t buffer[256];
static int buffer_pos = 0;

bool microbit_init(const char* port) {
    serial_fd = open(port, O_RDWR | O_NOCTTY | O_SYNC);
    if (serial_fd < 0) {
        return false;
    }
    
    // Configure serial port
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(serial_fd, &tty) != 0) {
        close(serial_fd);
        serial_fd = -1;
        return false;
    }
    
    // 115200 baud
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);
    
    // 8-N-1, no flow control
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_cflag &= ~(PARENB | PARODD | CSTOPB | CRTSCTS);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_iflag &= ~(IGNBRK | IXON | IXOFF | IXANY);
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN] = 0;  // Non-blocking read
    tty.c_cc[VTIME] = 1; // 0.1 second timeout
    
    if (tcsetattr(serial_fd, TCSANOW, &tty) != 0) {
        close(serial_fd);
        serial_fd = -1;
        return false;
    }
    
    // Flush any existing data
    tcflush(serial_fd, TCIOFLUSH);
    
    return true;
}

void microbit_close(void) {
    if (serial_fd >= 0) {
        close(serial_fd);
        serial_fd = -1;
    }
}

// Process a complete packet
static void process_packet(uint8_t* packet) {
    // Verify start and end bytes
    if (packet[0] != MSG_START_BYTE || packet[PACKET_SIZE-1] != MSG_END_BYTE) {
        return;
    }
    
    // Extract button states
    current_state.button_a = (packet[1] & 0x01) != 0;
    current_state.button_b = (packet[1] & 0x02) != 0;
    
    // Extract accelerometer data
    int16_t raw_x = (packet[2] << 8) | packet[3];
    int16_t raw_y = (packet[4] << 8) | packet[5];
    int16_t raw_z = (packet[6] << 8) | packet[7];
    
    // Convert to g forces (depends on your sensor range)
    const float sensitivity = 1.0f / 16384.0f;  // For ±2g range
    current_state.accel_x = raw_x * sensitivity;
    current_state.accel_y = raw_y * sensitivity;
    current_state.accel_z = raw_z * sensitivity;
}

bool microbit_update(void) {
    if (serial_fd < 0) {
        return false;
    }
    
    // Read available data
    uint8_t tmp_buf[128];
    int bytes_read = read(serial_fd, tmp_buf, sizeof(tmp_buf));
    
    if (bytes_read <= 0) {
        return false;
    }
    
    // Process received bytes
    bool packet_found = false;
    for (int i = 0; i < bytes_read; i++) {
        // Add byte to buffer
        buffer[buffer_pos++] = tmp_buf[i];
        
        // Keep buffer size in check
        if (buffer_pos >= sizeof(buffer)) {
            buffer_pos = 0;
        }
        
        // Check for packet start
        if (tmp_buf[i] == MSG_START_BYTE) {
            buffer_pos = 1;
            buffer[0] = MSG_START_BYTE;
        }
        
        // Check for complete packet
        if (buffer_pos == PACKET_SIZE && buffer[PACKET_SIZE-1] == MSG_END_BYTE) {
            process_packet(buffer);
            buffer_pos = 0;
            packet_found = true;
        }
    }
    
    return packet_found;
}

bool microbit_button_a(void) {
    return current_state.button_a;
}

bool microbit_button_b(void) {
    return current_state.button_b;
}

float microbit_accel_x(void) {
    return current_state.accel_x;
}

float microbit_accel_y(void) {
    return current_state.accel_y;
}

float microbit_accel_z(void) {
    return current_state.accel_z;
}

const MicrobitState* microbit_get_state(void) {
    return &current_state;
}