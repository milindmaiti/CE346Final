#include "raylib.h"
#include <stdlib.h>
#include <math.h>
#include "pong.h"

/**
 * Classic Game: Pong
 * 
 * Two players will each control a rectangle and there will be a ball flying around.
 * The ball will be represented as a circle with a position and a radius, and each player
 * paddle will be able to move up or down with a defined speed.
 */

float random_float(float min, float max) {
    return min + (rand() / (float)RAND_MAX) * (max - min);
}

void generate_random_unit_vector(float *x, float *y) {
    // Generate random components
    *x = random_float(-1.0f, 1.0f);
    *y = random_float(-1.0f, 1.0f);
    
    // Normalize the vector
    float length = sqrt(*x * *x + *y * *y);
    *x /= length;
    *y /= length;
}

typedef struct Ball {
    Vector2 position;
    Vector2 curDirection; // unit vector that describes current direction ball is traveling in
    int radius;
    Color color;
} Ball;

typedef struct Player {
    Rectangle rec;
    Color color;
    float speed;
} Player;

static Ball ball = { 0 };
static Player player1, player2;

static bool gameOver = false;
static bool pause = false;

static int BALL_SPEED = 1;
static int BALL_RADIUS = 24;

static const int PLAYER_X_OFFSET = 30;
static const int PLAYER_WIDTH = 15;
static const int PLAYER_HEIGHT = 40;
static const double dt = 0.1;

static const float maxAngle = 75;

int player1Wins = 0;
int player2Wins = 0;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;

void UpdateGame(void){
    // event loop
    if(!gameOver){
        if (IsKeyDown('P')) pause = !pause;

        if(!pause){
            ball.position = (Vector2){ball.position.x + ball.curDirection.x * dt, ball.position.y + ball.curDirection.y * dt};
            if(IsKeyDown('A')){
                player1.rec.y += player1.speed * dt;
            }
            if(IsKeyDown('S')){
                player1.rec.y -= player1.speed * dt;
            }
            if(IsKeyDown('K')){
                player2.rec.y += player2.speed * dt;
            }
            if(IsKeyDown('L')){
                player2.rec.y -= player2.speed * dt;
            }

            if(CheckCollisionCircleRec(ball.position, ball.radius, player1.rec)){
                // ranges from [-(rectHeight/2)-radius, (rectHeight/2)+radius]
                float relativeOffset = player1.rec.y - ball.position.y;
                float angle = (relativeOffset / (player1.rec.height/2.0 + ball.radius)) * maxAngle;
                ball.curDirection.x = cos(angle);
                ball.curDirection.y = sin(angle);
            }
            else if(CheckCollisionCircleRec(ball.position, ball.radius, player2.rec)){
                float relativeOffset = player2.rec.y - ball.position.y;
                float angle = (relativeOffset / (player2.rec.height/2.0 + ball.radius)) * maxAngle;
                ball.curDirection.x = -cos(angle);
                ball.curDirection.y = sin(angle);
            }

            if(ball.position.x < 0){
                player2Wins++;
                InitGame();
            }
            else if(ball.position.x >= SCREEN_WIDTH){
                player1Wins++;
                InitGame();
            }
        }
    }
}

void DrawGame(void){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    DrawCircle(ball.position.x, ball.position.y, ball.radius, ball.color);
    DrawRectangle(player1.rec.x, player1.rec.y, player1.rec.width, player1.rec.height, player1.color);
    DrawRectangle(player2.rec.x, player2.rec.y, player2.rec.width, player2.rec.height, player2.color);
    EndDrawing();
}

void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}

void InitGame(void){
    ball.radius = BALL_RADIUS;
    ball.position = (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
    generate_random_unit_vector(&ball.curDirection.x, &ball.curDirection.y);

    player1.color = RED;
    player1.rec.x = PLAYER_X_OFFSET;
    player1.rec.y = SCREEN_HEIGHT / 2;
    player1.rec.width = PLAYER_WIDTH;
    player1.rec.height = PLAYER_HEIGHT;
    player1.speed = 0.5;

    player2.color = RED;
    player2.rec.x = SCREEN_WIDTH - PLAYER_X_OFFSET;
    player2.rec.y = SCREEN_HEIGHT / 2;
    player2.rec.width = PLAYER_WIDTH;
    player2.rec.height = PLAYER_HEIGHT;
    player2.speed = 0.5;
}

void UnloadGame(void){

}