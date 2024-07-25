#include <iostream>
#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>


struct SnakeSegment {
    Vector2 position;
    Vector2 size;
};

struct Food {
    Vector2 position;
    Vector2 size;
    Color color;
};

enum Direction { UP, DOWN, LEFT, RIGHT };

enum GameState { MENU, GAME, GAME_OVER };

bool CheckSelfCollision(const std::vector<SnakeSegment>& snakeBody) {
    for (size_t i = 1; i < snakeBody.size(); i++) {
        if (snakeBody[0].position.x == snakeBody[i].position.x &&
            snakeBody[0].position.y == snakeBody[i].position.y) {
            return true;
        }
    }
    return false;
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Crabe game");

    // StartUp snake
    SnakeSegment snake;
    snake.position = {screenWidth / 2, screenHeight / 2};
    snake.size = {20, 20};

    std::vector<SnakeSegment> snakeBody;
    snakeBody.push_back(snake);

    Direction dir = RIGHT;
    int speed = 10;
    int frameCounter = 0;

    // StartUp food
    Food food;
    food.size = {20, 20};
    food.color = RED;
    srand(time(0));
    food.position.x = (rand() % (screenWidth / (int)food.size.x)) * food.size.x;
    food.position.y = (rand() % (screenHeight / (int)food.size.y)) * food.size.y;

    SetTargetFPS(60);

    GameState gameState = MENU;

    while (!WindowShouldClose()) {

        switch (gameState) {
            case MENU:
                if (IsKeyPressed(KEY_ENTER)) {
                    gameState = GAME;
                } else if (IsKeyPressed(KEY_ESCAPE)) {
                    CloseWindow();
                }

                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("SNAKE GAME", screenWidth / 2 - 100,
                         screenHeight / 2 - 50, 40, DARKGREEN);
                DrawText("Press ENTER to Start", screenWidth / 2 - 80,
                         screenHeight / 2 + 10, 20, DARKGRAY);
                DrawText("Press ESC to Exit", screenWidth / 2 - 60,
                         screenHeight / 2 + 40, 20, DARKGRAY);
                EndDrawing();
                break;

            case GAME:
                if (IsKeyPressed(KEY_UP) && dir != DOWN) dir = UP;
                if (IsKeyPressed(KEY_DOWN) && dir != UP) dir = DOWN;
                if (IsKeyPressed(KEY_LEFT) && dir != RIGHT) dir = LEFT;
                if (IsKeyPressed(KEY_RIGHT) && dir != LEFT) dir = RIGHT;

                if (frameCounter >= (60 / speed)) {
                    frameCounter = 0;

                    // Body movement
                    for (int i = snakeBody.size() - 1; i > 0; i--) {
                        snakeBody[i].position = snakeBody[i - 1].position;
                    }

                    // Head movement
                    if (dir == UP) snakeBody[0].position.y -= snake.size.y;
                    if (dir == DOWN) snakeBody[0].position.y += snake.size.y;
                    if (dir == LEFT) snakeBody[0].position.x -= snake.size.x;
                    if (dir == RIGHT) snakeBody[0].position.x += snake.size.x;

                    // Tp check
                    if (snakeBody[0].position.x < 0)
                        snakeBody[0].position.x = screenWidth - snake.size.x;
                    if (snakeBody[0].position.x >= screenWidth)
                        snakeBody[0].position.x = 0;
                    if (snakeBody[0].position.y < 0)
                        snakeBody[0].position.y = screenHeight - snake.size.y;
                    if (snakeBody[0].position.y >= screenHeight)
                        snakeBody[0].position.y = 0;

                    // Eating butt check
                    if (CheckSelfCollision(snakeBody)) {
                        gameState = GAME_OVER;
                    }

                    // Food eating check
                    if (CheckCollisionRecs(
                            {snakeBody[0].position.x, snakeBody[0].position.y,
                             snake.size.x, snake.size.y},
                            {food.position.x, food.position.y, food.size.x,
                             food.size.y})) {
                        SnakeSegment newSegment;
                        newSegment.size = snake.size;
                        newSegment.position = snakeBody.back().position;
                        snakeBody.push_back(newSegment);

                        food.position.x =
                            (rand() % (screenWidth / (int)food.size.x)) * food.size.x;
                        food.position.y =
                            (rand() % (screenHeight / (int)food.size.y)) * food.size.y;
                    }
                }

                frameCounter++;

                BeginDrawing();
                ClearBackground(RAYWHITE);

                for (const auto& segment : snakeBody) {
                    DrawRectangleV(segment.position, segment.size, GREEN);
                }

                DrawRectangleV(food.position, food.size, food.color);

                EndDrawing();

                break;

            case GAME_OVER:

                if (IsKeyPressed(KEY_ENTER)) {
                    snakeBody.clear();
                    snakeBody.push_back(snake);
                    dir = RIGHT;
                    gameState = GAME;                   

                    food.position.x =
                        (rand() % (screenWidth / (int)food.size.x)) *
                        food.size.x;
                    food.position.y =
                        (rand() % (screenHeight / (int)food.size.y)) *
                        food.size.y;
                                        
                }

                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("GAME OVER!", screenWidth / 2 - 100,
                         screenHeight / 2 - 50, 40, RED);
                DrawText("Press ENTER to Restart", screenWidth / 2 - 100,
                         screenHeight / 2 + 10, 20, DARKGRAY);
                DrawText("Press ESC to Close the game", screenWidth / 2 - 130,
                         screenHeight / 2 + 40, 20, DARKGRAY);
                EndDrawing();
                break;
        }       
    }

    CloseWindow();

    return 0;
}