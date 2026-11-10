#include "grid.h"
#include "raylib.h"
#include <random>

Grid::Grid()
{
    // 15 x 15 blocks, 38px each with a 2px gap -> fills the left 600 x 600
    cols = 15;
    rows = 15;
    blockWidth = 38.0f;
    blockHeight = 38.0f;
    spacingX = 2.0f;
    spacingY = 2.0f;
    alive.assign(cols * rows, false); // all dead
}

bool Grid::InBounds(int x, int y) const
{
    return x >= 0 && x < cols && y >= 0 && y < rows;
}

bool Grid::IsAlive(int x, int y) const
{
    return InBounds(x, y) && alive[y * cols + x];
}

void Grid::SetAlive(int x, int y, bool state)
{
    if (InBounds(x, y))
        alive[y * cols + x] = state;
}

void Grid::Toggle(int x, int y)
{
    SetAlive(x, y, !IsAlive(x, y));
}

void Grid::Randomize()
{
    const float wallChance = 0.2f; // ~20% of inner squares alive

    std::mt19937 rng(std::random_device{}());
    std::bernoulli_distribution isWall(wallChance);

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            bool onEdge = x == 0 || y == 0 || x == cols - 1 || y == rows - 1;
            SetAlive(x, y, onEdge || isWall(rng));
        }
    }
}

void Grid::Draw() const
{
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            float px = x * (blockWidth + spacingX);
            float py = y * (blockHeight + spacingY);
            Color color = alive[y * cols + x] ? WHITE : BLACK;
            DrawRectangleV({px, py}, {blockWidth, blockHeight}, color);
        }
    }
}

int Grid::GetCols() const { return cols; }
int Grid::GetRows() const { return rows; }
float Grid::GetBlockWidth() const { return blockWidth; }
float Grid::GetBlockHeight() const { return blockHeight; }
float Grid::GetSpacingX() const { return spacingX; }
float Grid::GetSpacingY() const { return spacingY; }

bool Grid::IsWallAt(float px, float py) const
{
    if (px < 0.0f || py < 0.0f)
        return true;

    // Each square owns the gap to its right and below it.
    int x = static_cast<int>(px / (blockWidth + spacingX));
    int y = static_cast<int>(py / (blockHeight + spacingY));

    return !InBounds(x, y) || IsAlive(x, y);
}
