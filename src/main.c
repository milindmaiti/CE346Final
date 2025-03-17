#include "raylib.h"
#include "resource_dir.h"

#include "comms.h"
#include "pong.h"

int main () {
	configure_microbit();
	if(fd == -1) {
		return 1;
	}

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

// 	// Create the window and OpenGL context
// 	InitWindow(1280, 800, "Hello Raylib");

// 	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
// 	SearchAndSetResourceDir("./raylib/resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	microbit_output_t out;

	// game loop
	char str[20];
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(RAYWHITE);

		read_microbit(&out);

		printf("%d, %d\n", out.A, out.B);

// 		// draw our texture to the screen
// 		DrawTexture(wabbit, 400, 200, WHITE);
		
// 		// end the frame and get ready for the next one  (display frame, poll input, etc...)
// 		EndDrawing();
// 	}

// 	// cleanup
// 	// unload our texture so it can be cleaned up
// 	UnloadTexture(wabbit);

// 	// destroy the window and cleanup the OpenGL context
// 	CloseWindow();

// 	close(fd);

// 	return 0;
// }