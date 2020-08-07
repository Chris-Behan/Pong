#include "raylib.h"
#include <stdio.h>
#include <math.h>

#define screenWidth 800
#define screenHeight 450
#define padWidth 25.0f
#define padHeight 100.0f
#define movementSpeed 20.0f
#define ballRadius 10.0f

int player1Score = 0;
int player2Score = 0;

char player1ScoreStr[3];
char player2ScoreStr[3];

Vector2 ballPosition = {(float)screenWidth / 2, (float)screenHeight / 2};
Vector2 player1Position = {0.0f, (float)screenHeight / 2 - padHeight / 2};
Vector2 player2Position = {(float)screenWidth - padWidth, (float)screenHeight / 2 - padHeight / 2};
Vector2 padSize = {padWidth, padHeight};
Vector2 ballMovement = {(float)5.0f, (float)0.0f};

Sound p1Sound;
Sound p2Sound;
Sound hitSound;

void initialize(void);
void mainGameLoop(void);
void unInitialize(void);
void handleScoreBoard(void);
void handleGameplay(void);
void updateGame(void);
void drawGame(void);

void handleCollision(void);
void collisionHelper(Vector2 *);
void handlePlayerMovement(void);
void handleWallCollision(void);
void handleScoring(void);

int main()
{
  initialize();
  mainGameLoop();
  unInitialize();
  return 0;
}

void initialize(void)
{
  InitWindow(screenWidth, screenHeight, "pong");
  InitAudioDevice();
  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  p1Sound = LoadSound("sounds/impactPlank_medium_001.ogg");
  p2Sound = LoadSound("sounds/impactPlank_medium_002.ogg");
  hitSound = LoadSound("sounds/impactPlate_heavy_003.ogg");
  sprintf(player1ScoreStr, "%d", player1Score);
  sprintf(player2ScoreStr, "%d", player2Score);
}

void mainGameLoop(void)
{
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    if (player1Score > 2 || player2Score > 2)
    {
      handleScoreBoard();
    }
    else
    {
      handleGameplay();
    }
  }
}

void unInitialize(void)
{
  UnloadSound(p1Sound);
  UnloadSound(p2Sound);
  UnloadSound(hitSound);
  CloseAudioDevice();
  CloseWindow(); // Close window and OpenGL context
}

void handleScoreBoard(void)
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

void handleGameplay(void)
{
  updateGame();
  drawGame();
}

void updateGame(void)
{
  handlePlayerMovement();
  handleCollision();
  handleWallCollision();
  handleScoring();
  ballPosition.x += ballMovement.x;
  ballPosition.y += ballMovement.y;
}

void drawGame(void)
{
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

void handleCollision(void)
{
  // Player 1 collision
  if (ballPosition.x - ballRadius == player1Position.x + padSize.x &&
      (ballPosition.y + ballRadius >= player1Position.y && ballPosition.y - ballRadius <= player1Position.y + padHeight))
  {
    PlaySound(p1Sound);
    collisionHelper(&player1Position);
  }
  // Player 2 collision
  if (ballPosition.x + ballRadius == player2Position.x &&
      (ballPosition.y + ballRadius >= player2Position.y && ballPosition.y - ballRadius <= player2Position.y + padHeight))
  {
    PlaySound(p2Sound);
    collisionHelper(&player2Position);
  }
}

void collisionHelper(Vector2 *playerPosition)
{
  ballMovement.x *= -1.0f;
  float midPad = playerPosition->y + padSize.y / 2;
  // Contact made to top of pad
  if (ballPosition.y < midPad)
  {
    ballMovement.y = (fabs(midPad - ballPosition.y) * -0.1f);
    ballMovement.y = ballMovement.y < -4.0f ? ballMovement.y : -4.0f;
  }
  //contact made to bottom of pad
  else
  {
    ballMovement.y = (fabs(midPad - ballPosition.y) * 0.1f);
    ballMovement.y = ballMovement.y > 4.0f ? ballMovement.y : 4.0f;
  }
}

void handlePlayerMovement(void)
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

void handleWallCollision(void)
{
  if (ballPosition.y <= 0 || ballPosition.y >= screenHeight)
  {
    ballMovement.y *= -1.0f;
  }
}

void handleScoring(void)
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
