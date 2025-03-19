#include "raylib.h"
#include "resource_dir.h"
#include <string.h>
#include <stdio.h>

#include "comms.h"
#include "pong.h"
#include "bowmaster.h"

int main () {
	microbit_output_t out1;
	out1.A = 0; out1.B = 0; out1.x = 0; out1.y = 0; out1.z = 0;
	microbit_output_t out2;
	out2.A = 0; out2.B = 0; out2.x = 0; out2.y = 0; out2.z = 0;
	strcpy(out1.port, "/dev/tty.usbmodem1102");
	strcpy(out2.port, "/dev/tty.usbmodem1302");
	
	if(configure_microbit(&out1) == -1) {
		printf("Couldn't configure microbit1\n");
		return -1;
	}
	if(configure_microbit(&out2) == -1){
		printf("Couldn't configure microbit2\n");
		return -1;
	}

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	int SCREEN_WIDTH = 800;
	int SCREEN_HEIGHT = 450;
	// Create the window and OpenGL context
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Raylib");
	SetTargetFPS(60);
	InitGameBM(SCREEN_WIDTH, SCREEN_HEIGHT);
	

	// game loop
	char str[20];
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		// UpdateDrawFrame(&out1, &out2);
		// UpdateDrawFramePong(NULL, &out2);
		// UpdateDrawFrameBM(NULL, &out2);
		UpdateDrawFrameBM(&out1, &out2);

		// read_microbit(&out1);
		// printf("%u,%u,%f,%f,%f\n", out2.A, out2.B, out2.x, out2.y, out2.z);

	}

	close(out1.fd);
	close(out2.fd);

	return 0;
}