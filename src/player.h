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

    // Moves the player only if the new spot is on the map (left half of
    // the screen) and the whole player is over dead squares.
    // Returns false and stays put otherwise.
    bool SetPosition(float x, float y, const Grid& grid);

    // A/D turn left/right by 0.1 radians per frame.
    // W/S move forward/backward along the facing direction; walls block it.
    void Move(const Grid& grid);

    // Draws the player and a line showing which way it faces.
    void Draw() const;

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
