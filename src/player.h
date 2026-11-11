#pragma once

#include "grid.h"
#include "raylib.h"

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

    // Draws a ray from the player along its facing direction. It hops from
    // grid line to grid line and stops at the first alive cube, or after
    // MAX_RAY_STEPS lines if it never finds one.
    void DrawRay(const Grid& grid) const;

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
