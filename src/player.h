#pragma once

#include "grid.h"
#include "raylib.h"
#include <vector>

class Player
{
public:
    Player();

    float GetWidth() const;
    float GetHeight() const;
    float GetX() const;
    float GetY() const;
    Vector2 GetDir() const;

    // Sizes the player to a quarter of a block and places it on the
    // dead square closest to the middle of the map.
    void Spawn(const Grid& grid);

    // A/D turn left/right by 0.1 radians per frame.
    // W/S move forward/backward along the facing direction. The player only
    // moves onto spots that are on the map and fully over dead squares.
    void Move(const Grid& grid);

    // Draws the player and a line showing which way it faces.
    void Draw() const;

    // Casts count rays fanned across a 60 degree view (30 either side of
    // the facing direction), left to right. Each hops from grid line to grid
    // line and stops at the first alive cube, or after MAX_RAY_STEPS lines.
    // Returns where each ray stopped.
    std::vector<Vector2> CastRays(const Grid& grid, int count) const;

    // Draws the rays from CastRays on the 2D map.
    void DrawRays(const std::vector<Vector2>& hits) const;

    // Draws one wall column per ray across view, left to right. Column height
    // is blockSize * view.height / distance, capped at view.height, and
    // centred vertically.
    void DrawWalls(const std::vector<Vector2>& hits, Rectangle view,
                   float blockSize) const;

private:
    bool CanStandAt(float x, float y, const Grid& grid) const;

    float width;
    float height;
    float posX; // centre of the player
    float posY;

    // Facing direction as a unit vector in screen coordinates
    // ({1, 0} = facing right, {0, -1} = facing up).
    Vector2 dir;
};
