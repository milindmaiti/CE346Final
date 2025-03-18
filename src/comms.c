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

	while(!bytes_read) {
		bytes_read = read(out->fd, buffer, BUFFER_SIZE - 1);
	}

	if(bytes_read < 3) {
		out->A = -1;
		out->B = -1;
		return;
	}

	if(buffer[1] != ',') {
		out->A = -1;
		out->B = -1;
		return;
	}

	buffer[1] = '\0';
	sscanf(buffer, "%d", &out->A);
	sscanf(buffer + 2, "%d", &out->B);
}