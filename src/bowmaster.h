#pragma once
#include "comms.h"
void UpdateDrawFrameBM(microbit_output_t* m1, microbit_output_t* m2);
void InitGameBM(int screen_width, int screen_height);
void UnloadGameBM(void);
static void UpdateGame(microbit_output_t* m1, microbit_output_t* m2);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
// Additional module functions
static void InitBuildings(void);
static void InitPlayers(void);
static bool UpdatePlayer(int playerTurn, microbit_output_t* m1, microbit_output_t* m2);
static bool UpdateBall(int playerTurn);