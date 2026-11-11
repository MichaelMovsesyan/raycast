#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include <cmath>

namespace
{
const float MOVE_SPEED = 4.0f;  // pixels per frame
const float TURN_SPEED = 0.1f;  // radians per frame
const int MAX_RAY_STEPS = 20;   // grid lines a ray may cross before giving up

// Result of stepping a ray across one set of grid lines.
struct RayHit
{
    Vector2 point; // where the ray stopped
    bool hit;      // true if it stopped on an alive cube
};

// Casts against the horizontal grid lines (y = k * pitchY).
RayHit CastHorizontal(Vector2 origin, Vector2 dir, const Grid& grid)
{
    if (dir.y == 0.0f)
        return {origin, false}; // parallel to the lines: never crosses one

    float pitchY = grid.GetBlockHeight() + grid.GetSpacingY();

    // First line in front of the player.
    float rayY = std::floor(origin.y / pitchY) * pitchY;
    if (dir.y > 0.0f)
        rayY += pitchY;

    // Slide along the ray to that line, then line to line by a fixed offset.
    Vector2 point = {origin.x + (rayY - origin.y) * dir.x / dir.y, rayY};
    float offsetY = dir.y > 0.0f ? pitchY : -pitchY;
    Vector2 offset = {offsetY * dir.x / dir.y, offsetY};

    // Nudge into the cube on the far side of the line before checking it.
    float nudge = dir.y > 0.0f ? 0.01f : -0.01f;

    for (int i = 0; i < MAX_RAY_STEPS; i++)
    {
        if (grid.IsWallAt(point.x, point.y + nudge))
            return {point, true};
        point = Vector2Add(point, offset);
    }
    return {point, false};
}

// Casts against the vertical grid lines (x = k * pitchX).
RayHit CastVertical(Vector2 origin, Vector2 dir, const Grid& grid)
{
    if (dir.x == 0.0f)
        return {origin, false};

    float pitchX = grid.GetBlockWidth() + grid.GetSpacingX();

    float rayX = std::floor(origin.x / pitchX) * pitchX;
    if (dir.x > 0.0f)
        rayX += pitchX;

    Vector2 point = {rayX, origin.y + (rayX - origin.x) * dir.y / dir.x};
    float offsetX = dir.x > 0.0f ? pitchX : -pitchX;
    Vector2 offset = {offsetX, offsetX * dir.y / dir.x};

    float nudge = dir.x > 0.0f ? 0.01f : -0.01f;

    for (int i = 0; i < MAX_RAY_STEPS; i++)
    {
        if (grid.IsWallAt(point.x + nudge, point.y))
            return {point, true};
        point = Vector2Add(point, offset);
    }
    return {point, false};
}
}

Player::Player()
{
    width = 0.0f;  // set to a quarter of a block in Spawn()
    height = 0.0f;
    posX = 0.0f;
    posY = 0.0f;

    dir = {1.0f, 0.0f}; // facing right
}

float Player::GetWidth() const { return width; }
float Player::GetHeight() const { return height; }
float Player::GetX() const { return posX; }
float Player::GetY() const { return posY; }
Vector2 Player::GetDir() const { return dir; }

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

    // Put the player in the centre of that square. The player is a quarter
    // of a block, so it always fits inside a dead square.
    float pitchX = grid.GetBlockWidth() + grid.GetSpacingX();
    float pitchY = grid.GetBlockHeight() + grid.GetSpacingY();
    posX = bestX * pitchX + grid.GetBlockWidth() / 2.0f;
    posY = bestY * pitchY + grid.GetBlockHeight() / 2.0f;
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

void Player::Move(const Grid& grid)
{
    // Turning. Screen y grows downward, so a negative rotation turns the
    // player left (counter-clockwise on screen).
    if (IsKeyDown(KEY_A))
        dir = Vector2Rotate(dir, -TURN_SPEED);
    if (IsKeyDown(KEY_D))
        dir = Vector2Rotate(dir, TURN_SPEED);
    dir = Vector2Normalize(dir); // keep rounding error from building up

    // Forward / backward.
    Vector2 step = {0.0f, 0.0f};
    if (IsKeyDown(KEY_W))
        step = Vector2Add(step, Vector2Scale(dir, MOVE_SPEED));
    if (IsKeyDown(KEY_S))
        step = Vector2Subtract(step, Vector2Scale(dir, MOVE_SPEED));

    // Each axis is checked separately so the player slides along walls
    // instead of sticking to them. IsWallAt treats anything outside the
    // grid as a wall, so this also keeps the player on the map.
    if (CanStandAt(posX + step.x, posY, grid))
        posX += step.x;
    if (CanStandAt(posX, posY + step.y, grid))
        posY += step.y;
}

void Player::Draw() const
{
    DrawRectangleV({posX - width / 2.0f, posY - height / 2.0f},
                   {width, height}, RED);

    // Direction line: starts at the player's centre, points forward.
    const float lineLength = 30.0f;
    const float lineThickness = 2.0f;
    Vector2 start = {posX, posY};
    Vector2 end = Vector2Add(start, Vector2Scale(dir, lineLength));
    DrawLineEx(start, end, lineThickness, RED);
}

void Player::DrawRay(const Grid& grid) const
{
    Vector2 origin = {posX, posY};
    RayHit horizontal = CastHorizontal(origin, dir, grid);
    RayHit vertical = CastVertical(origin, dir, grid);

    // A pass that never crosses a line can't stop the ray.
    float hDist = dir.y == 0.0f ? INFINITY
                                : Vector2DistanceSqr(origin, horizontal.point);
    float vDist = dir.x == 0.0f ? INFINITY
                                : Vector2DistanceSqr(origin, vertical.point);

    // Whichever set of lines stops the ray first is where it ends.
    Vector2 end = hDist < vDist ? horizontal.point : vertical.point;
    DrawLineEx(origin, end, 2.0f, GREEN);
}
