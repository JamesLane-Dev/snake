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

bool IsSnakeOutsideGrid(const std::vector<Position> &snake, int gridSize)
{
    if (snake[0].x < 0 || snake[0].x >= gridSize || snake[0].y < 0 || snake[0].y >= gridSize)
    {
        return true;
    }
    return false;
}

bool IsSnakeEatingFood(const std::vector<Position> &snake, const Position &foodPosition)
{
    if (snake[0].x == foodPosition.x &&
        snake[0].y == foodPosition.y)
    {
        return true;
    }
    return false;
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
void ResetGame(std::vector<Position> &snake, bool &gameOver, int &score, float &moveTimer, Direction &snakeDirection, Position &foodPosition)
{
    snake.clear();
    snake.push_back({10, 10});
    snake.push_back({9, 10});
    snake.push_back({8, 10});
    moveTimer = 0.0f;
    gameOver = false;
    score = 0;
    snakeDirection = {1, 0};
    foodPosition = {5, 5};
}

int main()
{
    const int gridSize = 20;
    const int cellSize = 30;
    float moveTimer = 0.0f;
    const float moveDelay = 0.15f;
    bool gameOver = false;
    int score = 0;

    InitWindow(600, 600, "Snake");
    SetTargetFPS(60);

    Direction snakeDirection = {1, 0};
    std::vector<Position> snake;
    snake.push_back({10, 10});
    snake.push_back({9, 10});
    snake.push_back({8, 10});

    Position foodPosition = {5, 5};

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        moveTimer += dt;
        Position previousTailPosition = snake.back();
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
        if (!gameOver && moveTimer >= moveDelay)
        {
            Position previousPosition = snake[0];
            snake[0].x += snakeDirection.x;
            snake[0].y += snakeDirection.y;
            for (int i = 1; i < snake.size(); i++)
            {
                Position temp = snake[i];
                snake[i] = previousPosition;
                previousPosition = temp;
            }

            moveTimer = 0.0f;
        }
        if (IsSnakeOutsideGrid(snake, gridSize))
        {
            gameOver = true;
        }
        if (IsSnakeCollidingWithItself(snake))
        {
            gameOver = true;
        }

        if (!gameOver && IsSnakeEatingFood(snake, foodPosition))
        {
            snake.push_back(previousTailPosition);
            score += 1;
            do
            {
                foodPosition.x = GetRandomValue(0, gridSize - 1);
                foodPosition.y = GetRandomValue(0, gridSize - 1);
            } while (IsFoodOnSnake(snake, foodPosition));
        }
        if (gameOver && IsKeyPressed(KEY_ENTER))
        {
            ResetGame(snake, gameOver, score, moveTimer, snakeDirection, foodPosition);
        }
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
            foodPosition.x * cellSize,
            foodPosition.y * cellSize,
            cellSize,
            cellSize,
            RED);

        for (Position segment : snake)
        {
            DrawRectangle(
                segment.x * cellSize,
                segment.y * cellSize,
                cellSize,
                cellSize,
                GREEN);
        }
        if (gameOver)
        {
            DrawText("Game Over", 210, 200, 40, RED);
            DrawText(
                TextFormat("Score: %i", score),
                220,
                230,
                40,
                RED);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}