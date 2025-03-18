#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

// #define MICROBIT_PORT "/dev/tty.usbmodem1102"
#define BAUDRATE 38400
#define BUFFER_SIZE 26	// size of buffer
#define INPUT_COOLDOWN 15 // number of cycles until same button press can be considered valid
#define NUM_OUTPUTS 2 // number of expected outputs
#define OUTPUT_SIZE 3 // proper output size

struct microbit_output {
	int A;
	int B;
	int fd;
	float x, y, z;
	float acc;
	float light;
	char port[22];
} typedef microbit_output_t;

int configure_microbit(microbit_output_t* out);

void read_microbit(microbit_output_t* out);