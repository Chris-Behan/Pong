#include "raylib.h"
#include <stdio.h>
#include <math.h>
//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;
static const int padWidth = 25.0f;
static const int padHeight = 100.0f;
static const int movementSpeed = 20.0f;
static const int ballRadius = 10.0f;
static int player1Score = 0;
static int player2Score = 0;

static char player1ScoreStr[3];
static char player2ScoreStr[3];

static Vector2 ballPosition = {(float)screenWidth / 2, (float)screenHeight / 2};
static Vector2 player1Position = {0.0f, (float)screenHeight / 2 - padHeight / 2};
static Vector2 player2Position = {(float)screenWidth - padWidth, (float)screenHeight / 2 - padHeight / 2};
static Vector2 padSize = {padWidth, padHeight};
static Vector2 ballMovement = {(float)5.0f, (float)0.0f};

static Sound p1Sound;
static Sound p2Sound;
static Sound hitSound;
//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void handlePlayerMovement(void);
static void handleCollision(void);
static void handleWallCollision(void);
static void handleScoring(void);

int main()
{
  // Initialization
  //--------------------------------------------------------------------------------------
  InitWindow(screenWidth, screenHeight, "pong");
  InitAudioDevice();
  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  p1Sound = LoadSound("sounds/impactPlank_medium_001.ogg");
  p2Sound = LoadSound("sounds/impactPlank_medium_002.ogg");
  hitSound = LoadSound("sounds/impactPlate_heavy_003.ogg");
  sprintf(player1ScoreStr, "%d", player1Score);
  sprintf(player2ScoreStr, "%d", player2Score);
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    if (player1Score > 2 || player2Score > 2)
    {
      ClearBackground(RAYWHITE);
      if (IsKeyDown(KEY_SPACE))
      {
        player1Score = 0, player2Score = 0;
        ballMovement.x = 5.0f, ballMovement.y = 0.0f;
        snprintf(player1ScoreStr, 3, "%d", player1Score);
        snprintf(player2ScoreStr, 3, "%d", player2Score);
      }

      BeginDrawing();
      if (player1Score > player2Score)
      {
        DrawText("Red wins!", 100, screenHeight / 2, 50, RED);
      }
      else
      {
        DrawText("Blue wins!", 100, screenHeight / 2, 50, BLUE);
      }
      DrawText("Press Space to Restart", 100, screenHeight / 2 + 60, 50, GRAY);
      EndDrawing();
    }
    else
    {

      // Update
      //----------------------------------------------------------------------------------
      handlePlayerMovement();
      handleCollision();
      handleWallCollision();
      handleScoring();
      ballPosition.x += ballMovement.x;
      ballPosition.y += ballMovement.y;
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

      DrawText(player1ScoreStr, 50, 400, 20, RED);
      DrawText(player2ScoreStr, 750, 400, 20, BLUE);

      EndDrawing();
    }
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  UnloadSound(p1Sound);
  UnloadSound(p2Sound);
  UnloadSound(hitSound);
  CloseAudioDevice();
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

static void handleCollision(void)
{
  // Player 1 collision
  if (ballPosition.x - ballRadius == player1Position.x + padSize.x &&
      (ballPosition.y + ballRadius >= player1Position.y && ballPosition.y - ballRadius <= player1Position.y + padHeight))
  {
    PlaySound(p1Sound);
    ballMovement.x *= -1.0f;
    float midPad = player1Position.y + padSize.y / 2;
    // Contact made to top of pad
    if (ballPosition.y < midPad)
    {
      ballMovement.y = ((midPad - ballPosition.y) * -0.1f);
      ballMovement.y = ballMovement.y < -4.0f ? ballMovement.y : -4.0f;
    }
    //contact made to bottom of bad
    else
    {
      ballMovement.y = ((midPad - ballPosition.y) * -0.1f);
      ballMovement.y = ballMovement.y > 4.0f ? ballMovement.y : 4.0f;
    }
  }
  // Player 2 collision
  if (ballPosition.x + ballRadius == player2Position.x &&
      (ballPosition.y + ballRadius >= player2Position.y && ballPosition.y - ballRadius <= player2Position.y + padHeight))
  {
    PlaySound(p2Sound);
    ballMovement.x *= -1.0f;
    float midPad = player2Position.y + padSize.y / 2;
    // Contact made to top of pad
    if (ballPosition.y < midPad)
    {
      ballMovement.y = ((midPad - ballPosition.y) * -0.1f);
      ballMovement.y = ballMovement.y < -4.0f ? ballMovement.y : -4.0f;
    }
    //contact made to bottom of bad
    else
    {
      ballMovement.y = ((midPad - ballPosition.y) * 0.1f);
      ballMovement.y = ballMovement.y > 4.0f ? ballMovement.y : 4.0f;
    }
  }
}

static void handlePlayerMovement(void)
{
  // Player 1 movement
  if (IsKeyDown(KEY_E) && player1Position.y > 0.0f)
  {
    player1Position.y -= movementSpeed;
  }
  if (IsKeyDown(KEY_D) && player1Position.y < screenHeight - padSize.y)
  {
    player1Position.y += movementSpeed;
  }
  // Player 2 movement
  if (IsKeyDown(KEY_I) && player2Position.y > 0.0f)
  {
    player2Position.y -= movementSpeed;
  }
  if (IsKeyDown(KEY_K) && player2Position.y < screenHeight - padSize.y)
  {
    player2Position.y += movementSpeed;
  }
}

static void handleWallCollision(void)
{
  if (ballPosition.y <= 0 || ballPosition.y >= screenHeight)
  {
    ballMovement.y *= -1.0f;
  }
}

static void handleScoring(void)
{
  int scored = 0;
  // Player2 scores
  if (ballPosition.x <= 0)
  {
    scored = 1;
    player2Score += 1;
    snprintf(player2ScoreStr, 3, "%d", player2Score);
  }
  // Player 1 scores
  if (ballPosition.x >= screenWidth)
  {
    scored = 1;
    player1Score += 1;
    snprintf(player1ScoreStr, 3, "%d", player1Score);
  }

  if (scored)
  {
    PlaySound(hitSound);
    ballPosition.x = (float)screenWidth / 2;
    ballPosition.y = (float)screenHeight / 2;
    ballMovement.x *= -1.0f;
    ballMovement.y = 0.0f;
  }
}
