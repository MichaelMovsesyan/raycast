#include "player.h"
#include "raylib.h"

Player::Player()
{
    width = 0.0f;  // set to a quarter of a block in Spawn()
    height = 0.0f;
    posX = 0.0f;
    posY = 0.0f;
}

float Player::GetWidth() const { return width; }
float Player::GetHeight() const { return height; }
float Player::GetX() const { return posX; }
float Player::GetY() const { return posY; }

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
    const float speed = 2.0f;

    float dx = 0.0f;
    float dy = 0.0f;
    if (IsKeyDown(KEY_W)) dy -= speed;
    if (IsKeyDown(KEY_S)) dy += speed;
    if (IsKeyDown(KEY_A)) dx -= speed;
    if (IsKeyDown(KEY_D)) dx += speed;

    // Move each axis separately so the player slides along walls
    // instead of sticking to them.
    SetPosition(posX + dx, posY, grid);
    SetPosition(posX, posY + dy, grid);
}

void Player::Draw() const
{
    DrawRectangleV({posX - width / 2.0f, posY - height / 2.0f},
                   {width, height}, RED);
}
