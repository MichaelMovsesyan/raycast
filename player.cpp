#include "player.h"

Player::Player()
{
    width = 10.0f;
    height = 10.0f;
    posX = 300.0f; // centre of the 600 x 600 map
    posY = 300.0f;
}

float Player::GetWidth() const { return width; }
float Player::GetHeight() const { return height; }
float Player::GetX() const { return posX; }
float Player::GetY() const { return posY; }

void Player::SetPosition(float x, float y)
{
    posX = x;
    posY = y;
}
