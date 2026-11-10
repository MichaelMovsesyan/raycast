#pragma once

#include "grid.h"

class Player
{
public:
    Player();

    float GetWidth() const;
    float GetHeight() const;
    float GetX() const;
    float GetY() const;
    float GetDX() const;
    float GetDY() const;
    float GetDA() const;

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

    // Facing direction. da is the angle in radians (0 = facing right,
    // increasing turns left). dx/dy are the step taken per frame.
    // y is flipped because screen y grows downward.
    float dx;
    float dy;
    float da;
};
