#include "raylib.h"

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;
static const int padWidth = 25.0f;
static const int padHeight = 100.0f;
static const int movementSpeed = 10.0f;
static const int ballRadius = 10.0f;

static Vector2 ballPosition = {(float)screenWidth / 2, (float)screenHeight / 2};
static Vector2 player1Position = {0.0f, (float)screenHeight / 2 - padHeight / 2};
static Vector2 player2Position = {(float)screenWidth - padWidth, (float)screenHeight / 2 - padHeight / 2};
static Vector2 padSize = {padWidth, padHeight};
static Vector2 ballMovement = {(float)5.0f, (float)0.0f};
//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void handlePlayerMovement(void);
static void handleCollision(void);

int main()
{
  // Initialization
  //--------------------------------------------------------------------------------------
  InitWindow(screenWidth, screenHeight, "pong");
  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    handlePlayerMovement();
    handleCollision();

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Move with E and D", 10, 10, 20, RED);
    DrawText("Move with I and K", 610, 10, 20, BLUE);
    DrawCircleV(ballPosition, ballRadius, BLACK);

    DrawRectangleV(player1Position, padSize, RED);
    DrawRectangleV(player2Position, padSize, BLUE);

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

static void handleCollision(void)
{
  if (ballPosition.x - ballRadius == player1Position.x + padSize.x &&
      (ballPosition.y >= player1Position.y && ballPosition.y <= player1Position.y + padHeight))
  {
    ballMovement.x *= -1.0f;
    ballMovement.y *= -1.0f;
  }
  if (ballPosition.x + ballRadius == player2Position.x &&
      (ballPosition.y >= player2Position.y && ballPosition.y <= player2Position.y + padHeight))
  {
    ballMovement.x *= -1.0f;
    ballMovement.y *= -1.0f;
  }
  ballPosition.x += ballMovement.x;
  ballPosition.y += ballMovement.y;
}

static void handlePlayerMovement(void)
{
  if (IsKeyDown(KEY_E) && player1Position.y > 0.0f)
  {
    player1Position.y -= movementSpeed;
  }
  if (IsKeyDown(KEY_D) && player1Position.y < screenHeight - padSize.y)
  {
    player1Position.y += movementSpeed;
  }

  if (IsKeyDown(KEY_I) && player2Position.y > 0.0f)
  {
    player2Position.y -= movementSpeed;
  }
  if (IsKeyDown(KEY_K) && player2Position.y < screenHeight - padSize.y)
  {
    player2Position.y += movementSpeed;
  }
}