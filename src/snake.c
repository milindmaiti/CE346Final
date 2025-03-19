/*******************************************************************************************
 *
 *   raylib - classic game: snake
 *
 *   Sample game developed by Ian Eito, Albert Martos and Ramon Santamaria
 *
 *   This game has been created using raylib v1.3 (www.raylib.com)
 *   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
 *
 *   Copyright (c) 2015 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "raylib.h"
#include "comms.h"

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define SNAKE_LENGTH 256
#define SQUARE_SIZE 31

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef struct Snake
{
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Color color;
} Snake;

typedef struct Food
{
    Vector2 position;
    Vector2 size;
    bool active;
    Color color;
} Food;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;

static int framesCounter = 0;
static bool gameOver = false;
static bool pauseGame = false;

static Food fruit = {0};
static Snake snake[SNAKE_LENGTH][2] = {0};
static Vector2 snakePosition[SNAKE_LENGTH][2] = {0};
static bool allowMove = false;
static Vector2 offset = {0};
static int counterTail[2];

static int lastA1 = 0;
static int lastB1 = 0;
static int lastA2 = 0;
static int lastB2 = 0;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
void InitGameSnake(int screen_width, int screen_height);
static void UpdateGame(microbit_output_t *m1, microbit_output_t *m2); // Update game (one frame)
static void DrawGame(void);                                           // Draw game (one frame)
void UnloadGameSnake(void);
void UpdateDrawFrameSnake(microbit_output_t *m1, microbit_output_t *m2);

// Initialize game variables
void InitGameSnake(int screen_width, int screen_height)
{
    screen_width = screenWidth;
    screen_height = screenHeight;

    framesCounter = 0;
    gameOver = false;
    pauseGame = false;

    counterTail[0] = 1;
    counterTail[1] = 1;
    allowMove = false;

    offset.x = screenWidth % SQUARE_SIZE;
    offset.y = screenHeight % SQUARE_SIZE;

    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        for(int sn = 0; sn < 2; sn++){
            snake[i][sn].position = (Vector2){(offset.x / 2) + (sn == 1 ? 1 : 0) * 3 * SQUARE_SIZE, (offset.y / 2) + (sn == 1 ? 1 : 0) * 3 * SQUARE_SIZE};
            snake[i][sn].size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
            snake[i][sn].speed = (Vector2){SQUARE_SIZE, 0};

            if (i == 0){
                if(sn == 0)
                snake[i][sn].color = DARKBLUE;
                else
                snake[i][sn].color = DARKGREEN;
            }
            else{
                if(sn == 0)
                snake[i][sn].color = BLUE;
                else
                snake[i][sn].color = GREEN;
            }
                
        }
        
    }

    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        for(int sn = 0; i < 2; i++)
        snakePosition[i][sn] = (Vector2){0.0f, 0.0f};
    }

    fruit.size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
    fruit.color = SKYBLUE;
    fruit.active = false;
}

void handleInput(Snake* s, microbit_output_t* m, int lastA, int lastB){
    if (m && m->A == 1 && lastA == 0 && allowMove)
    {
        //up
        if (s->speed.x == 0 && s->speed.y < 0)
        {
            s->speed = (Vector2){-SQUARE_SIZE, 0};
        }
        //down
        else if(s->speed.x == 0 && s->speed.y > 0)
        {
            s->speed = (Vector2){SQUARE_SIZE, 0};
        }
        //right
        else if(s->speed.x > 0 && s->speed.y == 0)
        {
            s->speed = (Vector2){0, -SQUARE_SIZE};
        }
        //left
        else if(s->speed.x < 0 && s->speed.y == 0)
        {
            s->speed = (Vector2){0, SQUARE_SIZE};
        }
        allowMove = false;
    }
    else if (m && m->B == 1 && lastB == 0 && allowMove)
    {
        //up
        if (s->speed.x == 0 && s->speed.y < 0)
        {
            s->speed = (Vector2){SQUARE_SIZE, 0};
        }
        //down
        else if(s->speed.x == 0 && s->speed.y > 0)
        {
            s->speed = (Vector2){-SQUARE_SIZE, 0};
        }
        //right
        else if(s->speed.x > 0 && s->speed.y == 0)
        {
            s->speed = (Vector2){0, SQUARE_SIZE};
        }
        //left
        else if(s->speed.x < 0 && s->speed.y == 0)
        {
            s->speed = (Vector2){0, -SQUARE_SIZE};
        }
        allowMove = false;
    }
}
// Update game (one frame)
void UpdateGame(microbit_output_t *m1, microbit_output_t *m2)
{
    
    if (!gameOver)
    {
        if(m1 != NULL) read_microbit(m1);
        if(m2 != NULL) read_microbit(m2);
        if (IsKeyPressed('P'))
            pauseGame = !pauseGame;
        
        
        if (!pauseGame)
        {
            // Player control
            handleInput(&snake[0][0], m1, lastA1, lastB1);
            handleInput(&snake[0][1], m2, lastA2, lastB2);

            // Snake movement
            for(int sn = 0; sn < 2; sn++)
                for (int i = 0; i < counterTail[sn]; i++)
                    snakePosition[i][sn] = snake[i][sn].position;

            if ((framesCounter % 8) == 0)
            {
                for(int sn = 0; sn < 2; sn++){
                    for (int i = 0; i < counterTail[sn]; i++)
                    {
                        if (i == 0)
                        {
                            snake[0][sn].position.x += snake[0][sn].speed.x;
                            snake[0][sn].position.y += snake[0][sn].speed.y;
                            allowMove = true;
                        }
                        else
                            snake[i][sn].position = snakePosition[i - 1][sn];
                    }
                }
            }

            // Wall behaviour
            for(int sn = 0; sn < 2; sn++){
                if (((snake[0][sn].position.x) > (screenWidth - offset.x)) ||
                    ((snake[0][sn].position.y) > (screenHeight - offset.y)) ||
                    (snake[0][sn].position.x < 0) || (snake[0][sn].position.y < 0))
                {
                    gameOver = true;
                }
            }
            

            // Collision with yourself
            for(int sn = 0; sn < 2; sn++){
                for (int i = 1; i < counterTail[sn]; i++)
                {
                    if ((snake[0][sn].position.x == snake[i][sn].position.x) && (snake[0][sn].position.y == snake[i][sn].position.y))
                        gameOver = true;
                }
            }

            // collision with another snake
            for(int sn = 0; sn < 2; sn++){
                for(int i = 0; i < counterTail[1-sn]; i++){
                    if((snake[0][sn].position.x == snake[i][1-sn].position.x) && (snake[0][sn].position.y == snake[i][1-sn].position.y))
                    gameOver = true;
                }
            }
            

            // Fruit position calculation
            if (!fruit.active)
            {
                fruit.active = true;
                fruit.position = (Vector2){GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2, GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};
                
                for(int sn = 0; sn < 2; sn++){
                    for (int i = 0; i < counterTail[sn]; i++)
                    {
                        while ((fruit.position.x == snake[i][sn].position.x) && (fruit.position.y == snake[i][sn].position.y))
                        {
                            fruit.position = (Vector2){GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2, GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};
                            i = 0;
                        }
                    }
                }
                
            }

            // Collision
            for(int sn = 0; sn < 2; sn++){
                if ((snake[0][sn].position.x < (fruit.position.x + fruit.size.x) && (snake[0][sn].position.x + snake[0][sn].size.x) > fruit.position.x) &&
                (snake[0][sn].position.y < (fruit.position.y + fruit.size.y) && (snake[0][sn].position.y + snake[0][sn].size.y) > fruit.position.y))
                {
                    snake[counterTail[sn]][sn].position = snakePosition[counterTail[sn] - 1][sn];
                    counterTail[sn] += 1;
                    fruit.active = false;
                }
            }
            

            framesCounter++;
        }

        if(m1 && m1->A == 1) lastA1 = 1;
        else if(m1 && m1->A == 0) lastA1 = 0;

        if(m1 && m1->B == 1) lastB1 = 1;
        else if(m1 && m1->B == 0) lastB1 = 0;

        if(m2 && m2->A == 1) lastA2 = 1;
        else if(m2 && m2->A == 0) lastA2 = 0;

        if(m2 && m2->B == 1) lastB2 = 1;
        else if(m2 && m2->B == 0) lastB2 = 0;
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGameSnake(screenWidth, screenHeight);
            gameOver = false;
        }
    }
}

// Draw game (one frame)
void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if (!gameOver)
    {
        // Draw grid lines
        for (int i = 0; i < screenWidth / SQUARE_SIZE + 1; i++)
        {
            DrawLineV((Vector2){SQUARE_SIZE * i + offset.x / 2, offset.y / 2}, (Vector2){SQUARE_SIZE * i + offset.x / 2, screenHeight - offset.y / 2}, LIGHTGRAY);
        }

        for (int i = 0; i < screenHeight / SQUARE_SIZE + 1; i++)
        {
            DrawLineV((Vector2){offset.x / 2, SQUARE_SIZE * i + offset.y / 2}, (Vector2){screenWidth - offset.x / 2, SQUARE_SIZE * i + offset.y / 2}, LIGHTGRAY);
        }

        // Draw snake
        for(int sn = 0; sn < 2; sn++)
            for (int i = 0; i < counterTail[sn]; i++)
                DrawRectangleV(snake[i][sn].position, snake[i][sn].size, snake[i][sn].color);

        // Draw fruit to pick
        DrawRectangleV(fruit.position, fruit.size, fruit.color);

        if (pauseGame)
            DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
    }
    else
        DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);

    EndDrawing();
}

// Unload game variables
void UnloadGameSnake(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update and Draw (one frame)
void UpdateDrawFrameSnake(microbit_output_t *m1, microbit_output_t *m2)
{
    if(m1 != NULL) read_microbit(m1);
    if(m2 != NULL) read_microbit(m2);
    UpdateGame(m1, m2);
    DrawGame();
}