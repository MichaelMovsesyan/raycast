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

    // Sizes the player to a quarter of a block and places it on the
    // dead square closest to the middle of the map.
    void Spawn(const Grid& grid);

    // Moves the player only if the new spot is on the map (left half of
    // the screen) and the whole player is over dead squares.
    // Returns false and stays put otherwise.
    bool SetPosition(float x, float y, const Grid& grid);

    // WASD moves 2 pixels per frame; walls block movement.
    void Move(const Grid& grid);

    void Draw() const;

private:
    bool CanStandAt(float x, float y, const Grid& grid) const;

    float width;
    float height;
    float posX; // centre of the player
    float posY;
};
