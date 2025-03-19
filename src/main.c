#include "raylib.h"
#include "resource_dir.h"
#include <string.h>
#include <stdio.h>

#include "comms.h"
#include "pong.h"
#include "bowmaster.h"
#include "snake.h"

#define NUM_GAMES 3
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))
#define abs(a) ((a) < 0 ? -1 * (a) : (a))

typedef struct game {
	char* name;
	int name_x;
	int name_y;
	Image img;
	int img_x;
	int img_y;
} game_t;

game_t games[NUM_GAMES];
static int current_game = 0; // the current screen to show
static bool prev = false; // currently moving to previous game
static bool next = false; // whether we are currently moving to the next game
static float fade = 1.; // amount we are fading current image by
static float EPS = 1e-9;
static bool game_started = false;

microbit_output_t m1, m2;

void UpdateMenu() {
	// read_microbit(&m1);
	// read_microbit(&m2);
	// print_microbit(&m1);
	// print_microbit(&m2);
	if(!prev && !next) {
		if(IsKeyPressed('P') || m1.A == 1 || m2.A == 1) {
			printf("PREV\n");
			prev = true;
		}
		else if(IsKeyPressed('N') || m1.B == 1 || m2.B == 1) {
			printf("NEXT\n");
			next = true;
		}
	}

	if(prev || next) { // must be fading out
		fade = max(fade - 0.02, 0);
		printf("FADING OUT\n");

		if(abs(fade) < EPS) {
			current_game = prev ? (current_game - 1 + NUM_GAMES) % NUM_GAMES : (current_game + 1) % NUM_GAMES;
			prev = next = false;
		}
	}
	else if(abs(fade - 1) >= EPS) { // must be fading in
		printf("FADING IN\n");
		fade = min(fade + 0.02, 1.);
	}
	else if(IsKeyPressed(KEY_ENTER)) {
		game_started = true;
	}
}

void UpdateDrawFrameMenu(void) {
	BeginDrawing();
    ClearBackground(RAYWHITE);
	game_t* cur = games + current_game;
	DrawTexture(LoadTextureFromImage(cur->img), cur->img_x, cur->img_y, Fade(WHITE, max(0, fade)));
	DrawText(cur->name, cur->name_x, cur->name_y, 30, Fade(BLACK, max(0, fade)));
	EndDrawing();
}

void resize_img(Image* img) {
	ImageResize(img, (int) (300.0 / (float) img->height * (float) img->width), 300);
}

int init() {
	// Initialize microbits
	// m1.A = 0; m1.B = 0; m1.x = 0; m1.y = 0; m1.z = 0;
	// m2.A = 0; m2.B = 0; m2.x = 0; m2.y = 0; m2.z = 0;
	// strcpy(m1.port, "/dev/tty.usbmodem1102");
	// strcpy(m2.port, "/dev/tty.usbmodem1302");

	// if(configure_microbit(&m1) == -1) {
	// 	printf("Couldn't configure microbit1\n");
	// 	return -1;
	// }
	// if(configure_microbit(&m2) == -1){
	// 	printf("Couldn't configure microbit2\n");
	// 	return -1;
	// }


	// Initialize game information
	SearchAndSetResourceDir("resources");
	Image pong_img = LoadImage("pong2.png");
	resize_img(&pong_img);
	games[0].name = "Pong";
	games[0].name_x = 370;
	games[0].name_y = 25;
	games[0].img = pong_img;
	games[0].img_x = 115;
	games[0].img_y = 80;

	Image bm_img = LoadImage("bowmaster.png");
	resize_img(&bm_img);
	games[1].name = "Bow Master";
	games[1].name_x = 330;
	games[1].name_y = 25;
	games[1].img = bm_img;
	games[1].img_x = 130;
	games[1].img_y = 80;

	Image snake_img = LoadImage("snake2.png");
	resize_img(&snake_img);
	games[2].name = "Snake";
	games[2].name_x = 350;
	games[2].name_y = 25;
	games[2].img = snake_img;
	games[2].img_x = 130;
	games[2].img_y = 80;

	return 0;
}

int main () {
	if(init()) {
		printf("Failed to initialize\n");
		return -1;
	}

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TinyGamesOS");
	SetTargetFPS(60);
	
	// game loop
	char str[20];
	// InitGamePong(SCREEN_WIDTH, SCREEN_HEIGHT);
	// InitGameBM(SCREEN_WIDTH, SCREEN_HEIGHT);
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{	
		if(game_started) {
			if(current_game == 0) {
				UpdateDrawFramePong(&m1, &m2);
			}
			else if(current_game == 1) {
				UpdateDrawFrameBM(&m1, &m2);
			}
			else if(current_game == 2) {
				UpdateDrawFrameSnake(&m1, &m2);
			}
		}
		else {
			UpdateMenu();
			UpdateDrawFrameMenu();
		}
	}

	close(m1.fd);
	// close(out2.fd);

	return 0;
}