#pragma once

class Player
{
public:
    Player();

    float GetWidth() const;
    float GetHeight() const;
    float GetX() const;
    float GetY() const;

    void SetPosition(float x, float y);

private:
    float width;
    float height;
    float posX;
    float posY;
};
