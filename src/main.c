#include "raylib.h"
#include "resource_dir.h"
#include <string.h>
#include <stdio.h>

#include "comms.h"
#include "pong.h"

int main () {
	microbit_output_t out1;
	microbit_output_t out2;
	strcpy(out1.port, "/dev/tty.usbmodem1102");
	strcpy(out2.port, "/dev/tty.usbmodem1302");
	
	if(configure_microbit(&out1) == -1) {
		printf("Couldn't configure microbit1\n");
		return -1;
	}
	else if(configure_microbit(&out2) == -1){
		printf("Couldn't configure microbit2\n");
		return -1;
	}

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Raylib");
	SetTargetFPS(60);
	InitGame();
	

	// game loop
	char str[20];
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		UpdateDrawFrame(&out1, &out2);

		// read_microbit(&out1);
		printf("%u,%u,%f,%f,%f\n", out1.A, out1.B, out1.x, out1.y, out1.z);

	}

	close(out1.fd);
	close(out2.fd);

	return 0;
}