#include "raylib.h"
#include <vector>

struct Position
{
    int x;
    int y;
};
struct Direction
{
    int x;
    int y;
};
struct GameState
{
    std::vector<Position> snake;
    Direction snakeDirection;
    Position foodPosition;
    int score;
    bool gameOver;
    float moveTimer;
    float moveDelay;
};

bool IsSnakeOutsideGrid(const std::vector<Position> &snake, int gridSize)
{
    return (snake[0].x < 0 || snake[0].x >= gridSize || snake[0].y < 0 || snake[0].y >= gridSize);
}

bool IsSnakeEatingFood(const std::vector<Position> &snake, const Position &foodPosition)
{
    return (snake[0].x == foodPosition.x &&
            snake[0].y == foodPosition.y);
}
bool IsFoodOnSnake(const std::vector<Position> &snake, const Position &foodPosition)
{
    for (Position segment : snake)
    {
        if (segment.x == foodPosition.x &&
            segment.y == foodPosition.y)
        {
            return true;
        }
    }
    return false;
}
bool IsSnakeCollidingWithItself(const std::vector<Position> &snake)
{
    for (int i = 1; i < snake.size(); i++)
    {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
        {
            return true;
        }
    }
    return false;
}
void ResetGame(GameState &game)
{
    game.snake.clear();
    game.snake.push_back({10, 10});
    game.snake.push_back({9, 10});
    game.snake.push_back({8, 10});
    game.moveTimer = 0.0f;
    game.gameOver = false;
    game.score = 0;
    game.snakeDirection = {1, 0};
    game.foodPosition = {5, 5};
    game.moveDelay = 0.15f;
}

void HandleInput(Direction &snakeDirection)
{
    if (IsKeyPressed(KEY_RIGHT) && snakeDirection.x != -1)
    {
        snakeDirection.x = 1;
        snakeDirection.y = 0;
    }
    if (IsKeyPressed(KEY_LEFT) && snakeDirection.x != 1)
    {
        snakeDirection.x = -1;
        snakeDirection.y = 0;
    }
    if (IsKeyPressed(KEY_DOWN) && snakeDirection.y != -1)
    {
        snakeDirection.x = 0;
        snakeDirection.y = 1;
    }
    if (IsKeyPressed(KEY_UP) && snakeDirection.y != 1)
    {
        snakeDirection.x = 0;
        snakeDirection.y = -1;
    }
}
Position MoveSnake(GameState &game)
{
    Position previousTailPosition = game.snake.back();
    if (!game.gameOver && game.moveTimer >= game.moveDelay)
    {
        Position previousPosition = game.snake[0];
        game.snake[0].x += game.snakeDirection.x;
        game.snake[0].y += game.snakeDirection.y;
        for (int i = 1; i < game.snake.size(); i++)
        {
            Position temp = game.snake[i];
            game.snake[i] = previousPosition;
            previousPosition = temp;
        }

        game.moveTimer = 0.0f;
    }
    return previousTailPosition;
}
void UpdateGame(GameState &game, int gridSize, const Position &previousTailPosition)
{
    if (!game.gameOver && IsSnakeEatingFood(game.snake, game.foodPosition))
    {
        game.snake.push_back(previousTailPosition);
        game.score += 1;
        game.moveDelay -= 0.005f;

        if (game.moveDelay < 0.05f)
        {
            game.moveDelay = 0.05f;
        }

        do
        {
            game.foodPosition.x = GetRandomValue(0, gridSize - 1);
            game.foodPosition.y = GetRandomValue(0, gridSize - 1);
        } while (IsFoodOnSnake(game.snake, game.foodPosition));
    }
}
void CheckCollisions(GameState &game, int gridSize)
{
    if (IsSnakeOutsideGrid(game.snake, gridSize))
    {
        game.gameOver = true;
    }
    if (IsSnakeCollidingWithItself(game.snake))
    {
        game.gameOver = true;
    }
}
void DrawGame(const GameState &game, int gridSize, int cellSize)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);
    for (int y = 0; y < gridSize; y++)
    {
        for (int x = 0; x < gridSize; x++)
        {
            DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, LIGHTGRAY);
        }
    }

    DrawRectangle(
        game.foodPosition.x * cellSize,
        game.foodPosition.y * cellSize,
        cellSize,
        cellSize,
        RED);

    for (Position segment : game.snake)
    {
        DrawRectangle(
            segment.x * cellSize,
            segment.y * cellSize,
            cellSize,
            cellSize,
            GREEN);
    }
    if (game.gameOver)
    {
        DrawText("Game Over", 210, 200, 40, RED);
        DrawText(
            TextFormat("Score: %i", game.score),
            220,
            230,
            40,
            RED);
    }
    EndDrawing();
}

int main()
{
    const int gridSize = 20;
    const int cellSize = 30;

    InitWindow(600, 600, "Snake");
    SetTargetFPS(60);

    GameState game;
    ResetGame(game);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        game.moveTimer += dt;

        // snake direction.
        HandleInput(game.snakeDirection);
        // move snake.
        Position previousTailPosition = MoveSnake(game);
        // Check collisions.
        CheckCollisions(game, gridSize);
        // Update.
        UpdateGame(game, gridSize, previousTailPosition);

        if (game.gameOver && IsKeyPressed(KEY_ENTER))
        {
            ResetGame(game);
        }
        // Draw
        DrawGame(game, gridSize, cellSize);
    }
    CloseWindow();
    return 0;
}