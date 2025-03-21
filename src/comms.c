#include "comms.h"

int configure_microbit(microbit_output_t* out) {
	out->fd = open(out->port, O_RDONLY | O_NOCTTY | O_NONBLOCK);
    if (out->fd == -1) {
        perror("Error opening serial port");
        return -1;
    }

	struct termios options;
    tcgetattr(out->fd, &options);
    cfsetispeed(&options, BAUDRATE);  // Match the baud rate with pyserial
    cfsetospeed(&options, BAUDRATE);
    options.c_cflag |= (CLOCAL | CREAD);
    tcsetattr(out->fd, TCSANOW, &options);

	return 0;
}

void read_microbit(microbit_output_t* out) {
	char buffer[BUFFER_SIZE];
	ssize_t bytes_read = 0;

	microbit_output_t cp = *out;
	
	while(!bytes_read) {
		bytes_read = read(out->fd, buffer, BUFFER_SIZE - 1);
	}

	if(bytes_read < BUFFER_SIZE - 1) {
		// out->A = -1;
		// out->B = -1;
		// out->x = -1;
		// out->y = -1;
		// out->z = -1;
		return;
	}

	buffer[BUFFER_SIZE - 1] = '\0';
	sscanf(buffer, "%u,%u,%u,%f,%f,%f,%u,%u", &out->A, &out->B, &out->C, &out->x, &out->y, &out->z, &out->L, &out->R);
}

void print_microbit(microbit_output_t* out) {
	printf("%u,%u,%u,%06.2f,%06.2f,%06.2f,%u,%03u\n", out->A, out->B, out->C, out->x, out->y, out->z, out->L, out->R);
}