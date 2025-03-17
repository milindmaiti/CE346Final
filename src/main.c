#include "raylib.h"
#include "pong.h"
#include <stdio.h>
int main(void){
    printf("Initializing Window\n");
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "classic game: pong");
    printf("Window Initialized\n");
    

    SetTargetFPS(60);
    InitGame();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateDrawFrame();
        //----------------------------------------------------------------------------------
    }

    UnloadGame();
    CloseWindow(); 

    return 0;
}


/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

// #include "raylib.h"
// #include "resource_dir.h"

// // Reading device port
// #include <stdio.h>
// #include <stdlib.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <termios.h>

// #define MICROBIT_PORT "/dev/tty.usbmodem1102"
// #define BAUDRATE 38400

// int main () {
// 	printf("Opening port\n");
// 	int fd = open(MICROBIT_PORT, O_RDONLY | O_NOCTTY | O_NONBLOCK);
//     if (fd == -1) {
//         perror("Error opening serial port");
//         return 1;
//     }

// 	printf("Opened port\n");

// 	struct termios options;
//     tcgetattr(fd, &options);
//     cfsetispeed(&options, BAUDRATE);  // Match the baud rate with pyserial
//     cfsetospeed(&options, BAUDRATE);
//     options.c_cflag |= (CLOCAL | CREAD);
//     tcsetattr(fd, TCSANOW, &options);

// 	char buffer[1024];

// 	// Tell the window to use vsync and work on high DPI displays
// 	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

// 	// Create the window and OpenGL context
// 	InitWindow(1280, 800, "Hello Raylib");

// 	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
// 	SearchAndSetResourceDir("./raylib/resources");

// 	// Load a texture from the resources directory
// 	Texture wabbit = LoadTexture("wabbit_alpha.png");
	
// 	// game loop
// 	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
// 	{
// 		// drawing
// 		BeginDrawing();

// 		// Setup the back buffer for drawing (clear color and depth buffers)
// 		ClearBackground(BLACK);

// 		ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
//         if (bytes_read > 0) {
//             buffer[bytes_read] = '\0';  // Null-terminate the received string
//             printf("Received: %s\n", buffer);
//         }

// 		// draw some text using the default font
// 		DrawText(buffer, 200,200,20,WHITE);

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