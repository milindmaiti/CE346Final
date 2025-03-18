#include "raylib.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "pong.h"
#include "comms.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))
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

void generate_random_unit_vector(float *x, float *y, int bias) {
    // Generate random components
    *x = random_float(-1.0f, 1.0f);
    *y = random_float(-1.0f, 1.0f);

    if(bias){
        *x += 0.3;
    }
    
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
static bool pauseGame = false;

static int BALL_SPEED = 60;
static int BALL_RADIUS = 15;

static const int PLAYER_X_OFFSET = 30;
static const int PLAYER_WIDTH = 15;
static const int PLAYER_HEIGHT = 40;
static const double dt = 0.1;

static const double maxAngle = (75.0 / 360.0) * (2 * M_PI);

int player1Wins = 0;
int player2Wins = 0;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;

void UpdateGame(microbit_output_t* m1, microbit_output_t* m2){
    // event loop
    if(!gameOver){
        if (IsKeyDown('P')) pauseGame = !pauseGame;
        read_microbit(m1);
        read_microbit(m2);
        if(!pauseGame){
            ball.position = (Vector2){ball.position.x + ball.curDirection.x * BALL_SPEED * dt, ball.position.y + ball.curDirection.y * BALL_SPEED * dt};
            if(ball.position.y - ball.radius < 0){
                ball.curDirection.y = -ball.curDirection.y;
                ball.position.y = ball.radius;
            }
            else if(ball.position.y + ball.radius > SCREEN_HEIGHT){
                ball.curDirection.y = -ball.curDirection.y;
                ball.position.y = SCREEN_HEIGHT - ball.radius;
            }
            if(m1->A){
                player1.rec.y += player1.speed * dt;
                player1.rec.y = min(player1.rec.y, SCREEN_HEIGHT - player1.rec.height);
            }
            if(m1->B){
                player1.rec.y -= player1.speed * dt;
                player1.rec.y = max(player1.rec.y, 0);
            }
            if(m2->A){
                player2.rec.y += player2.speed * dt;
                player2.rec.y = min(player2.rec.y, SCREEN_HEIGHT - player2.rec.height);
            }
            if(m2->B){
                player2.rec.y -= player2.speed * dt;
                player2.rec.y = max(player2.rec.y, 0);
            }

            if(CheckCollisionCircleRec(ball.position, ball.radius, player1.rec)){
                // ranges from [-(rectHeight/2)-radius, (rectHeight/2)+radius]
                float relativeOffset = player1.rec.y + player1.rec.height/2 - ball.position.y;
                float angle = (relativeOffset / (player1.rec.height/2 + ball.radius)) * maxAngle;
                ball.curDirection.x = cos(angle);
                ball.curDirection.y = sin(angle);

                printf("Ball stuff: %f %f %f\n", relativeOffset, (player1.rec.height/2 + ball.radius), angle);
                printf("%f %f\n", ball.curDirection.x, ball.curDirection.y);
                // ensure no weird double collision stuff happens

                // if ball's center within the y-coordinates of the rect, ensure it's
                // fully cleared in the x direction
                if(ball.position.y >= player1.rec.y && ball.position.y <= player1.rec.y + player1.rec.height){
                    ball.position.x = max(ball.position.x, player1.rec.x + player1.rec.width + ball.radius);
                }
                else if(ball.position.y < player1.rec.y){
                    ball.position.y = min(ball.position.y, player1.rec.y - ball.radius);
                }
                else if(ball.position.y > player1.rec.y + player1.rec.height){
                    ball.position.y = max(ball.position.y, player1.rec.y + player1.rec.height + ball.radius);
                }
                
            }
            else if(CheckCollisionCircleRec(ball.position, ball.radius, player2.rec)){
                float relativeOffset = player2.rec.y + player1.rec.height/2 - ball.position.y;
                float angle = (relativeOffset / (player2.rec.height/2 + ball.radius)) * maxAngle;
                ball.curDirection.x = -cos(angle);
                ball.curDirection.y = sin(angle);

                if(ball.position.y >= player2.rec.y && ball.position.y <= player2.rec.y + player2.rec.height){
                    ball.position.x = min(ball.position.x, player2.rec.x - ball.radius);
                }
                else if(ball.position.y < player2.rec.y){
                    ball.position.y = min(ball.position.y, player2.rec.y - ball.radius);
                }
                else if(ball.position.y > player2.rec.y + player2.rec.height){
                    ball.position.y = max(ball.position.y, player2.rec.y + player2.rec.height + ball.radius);
                }
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

void UpdateDrawFrame(microbit_output_t* m1, microbit_output_t* m2)
{
    UpdateGame(m1, m2);
    DrawGame();
}

void InitGame(void){
    ball.color = BLUE;
    ball.radius = BALL_RADIUS;
    ball.position = (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
    generate_random_unit_vector(&ball.curDirection.x, &ball.curDirection.y, 1);

    player1.color = RED;
    player1.rec.x = PLAYER_X_OFFSET;
    player1.rec.y = SCREEN_HEIGHT / 2;
    player1.rec.width = PLAYER_WIDTH;
    player1.rec.height = PLAYER_HEIGHT;
    player1.speed = 100.0;

    player2.color = RED;
    player2.rec.x = SCREEN_WIDTH - PLAYER_X_OFFSET;
    player2.rec.y = SCREEN_HEIGHT / 2;
    player2.rec.width = PLAYER_WIDTH;
    player2.rec.height = PLAYER_HEIGHT;
    player2.speed = 100.0;
}

void UnloadGame(void){

}