#include "player.h"
#include "raylib.h"
#include <cmath>

namespace
{
const float MOVE_SPEED = 4.0f;  // pixels per frame
const float TURN_SPEED = 0.1f;  // radians per frame
const float TWO_PI = 2.0f * PI;
}

Player::Player()
{
    width = 0.0f;  // set to a quarter of a block in Spawn()
    height = 0.0f;
    posX = 0.0f;
    posY = 0.0f;

    da = 0.0f; // facing right
    dx = MOVE_SPEED * std::cos(da);
    dy = MOVE_SPEED * std::sin(da);
}

float Player::GetWidth() const { return width; }
float Player::GetHeight() const { return height; }
float Player::GetX() const { return posX; }
float Player::GetY() const { return posY; }
float Player::GetDX() const { return dx; }
float Player::GetDY() const { return dy; }
float Player::GetDA() const { return da; }

void Player::Spawn(const Grid& grid)
{
    width = grid.GetBlockWidth() / 4.0f;
    height = grid.GetBlockHeight() / 4.0f;

    // Find the dead square nearest the middle of the map.
    int midX = grid.GetCols() / 2;
    int midY = grid.GetRows() / 2;
    int bestX = -1;
    int bestY = -1;
    int bestDist = 0;

    for (int y = 0; y < grid.GetRows(); y++)
    {
        for (int x = 0; x < grid.GetCols(); x++)
        {
            if (grid.IsAlive(x, y))
                continue;

            int dist = (x - midX) * (x - midX) + (y - midY) * (y - midY);
            if (bestX < 0 || dist < bestDist)
            {
                bestX = x;
                bestY = y;
                bestDist = dist;
            }
        }
    }

    if (bestX < 0)
        return; // every square is alive; nowhere to stand

    // Put the player in the centre of that square.
    float pitchX = grid.GetBlockWidth() + grid.GetSpacingX();
    float pitchY = grid.GetBlockHeight() + grid.GetSpacingY();
    SetPosition(bestX * pitchX + grid.GetBlockWidth() / 2.0f,
                bestY * pitchY + grid.GetBlockHeight() / 2.0f, grid);
}

bool Player::CanStandAt(float x, float y, const Grid& grid) const
{
    // Check all four corners of the player's square.
    float left = x - width / 2.0f;
    float right = x + width / 2.0f;
    float top = y - height / 2.0f;
    float bottom = y + height / 2.0f;

    return !grid.IsWallAt(left, top) &&
           !grid.IsWallAt(right, top) &&
           !grid.IsWallAt(left, bottom) &&
           !grid.IsWallAt(right, bottom);
}

bool Player::SetPosition(float x, float y, const Grid& grid)
{
    // IsWallAt treats anything outside the grid as a wall, so this also
    // keeps the player on the left half of the screen.
    if (!CanStandAt(x, y, grid))
        return false;

    posX = x;
    posY = y;
    return true;
}

void Player::Move(const Grid& grid)
{
    // Turning
    if (IsKeyDown(KEY_A))
    {
        da += TURN_SPEED;
        if (da > TWO_PI)
            da -= TWO_PI;
    }
    if (IsKeyDown(KEY_D))
    {
        da -= TURN_SPEED;
        if (da < 0.0f)
            da += TWO_PI;
    }
    dx = MOVE_SPEED * std::cos(da);
    dy = MOVE_SPEED * std::sin(da);

    // Forward / backward. Each axis is checked separately so the player
    // slides along walls instead of sticking to them.
    if (IsKeyDown(KEY_W))
    {
        SetPosition(posX + dx, posY, grid);
        SetPosition(posX, posY - dy, grid);
    }
    if (IsKeyDown(KEY_S))
    {
        SetPosition(posX - dx, posY, grid);
        SetPosition(posX, posY + dy, grid);
    }
}

void Player::Draw() const
{
    DrawRectangleV({posX - width / 2.0f, posY - height / 2.0f},
                   {width, height}, RED);

    // Direction line: starts at the player's centre, points forward.
    const float lineLength = 30.0f;
    const float lineThickness = 2.0f;
    Vector2 start = {posX, posY};
    Vector2 end = {posX + std::cos(da) * lineLength,
                   posY - std::sin(da) * lineLength};
    DrawLineEx(start, end, lineThickness, RED);
}
