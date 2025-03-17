#include "comms.h"

int configure_microbit() {
	fd = open(MICROBIT_PORT, O_RDONLY | O_NOCTTY | O_NONBLOCK);
    if (fd == -1) {
        perror("Error opening serial port");
        return -1;
    }

	struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, BAUDRATE);  // Match the baud rate with pyserial
    cfsetospeed(&options, BAUDRATE);
    options.c_cflag |= (CLOCAL | CREAD);
    tcsetattr(fd, TCSANOW, &options);

	return 0;
}

void read_microbit(microbit_output_t* out) {
	char buffer[BUFFER_SIZE];
	ssize_t bytes_read = 0;

	while(!bytes_read) {
		bytes_read = read(fd, buffer, BUFFER_SIZE - 1);
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