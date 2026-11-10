#pragma once

#include <vector>

class Grid
{
public:
    Grid();

    bool InBounds(int x, int y) const;
    bool IsAlive(int x, int y) const;
    void SetAlive(int x, int y, bool state);
    void Toggle(int x, int y);

    // Alive border around the edges, random walls (~20%) inside.
    void Randomize();

    // Alive squares are white; dead squares are black.
    void Draw() const;

    int GetCols() const;
    int GetRows() const;

private:
    int cols;
    int rows;
    float blockWidth;
    float blockHeight;
    float spacingX;
    float spacingY;
    std::vector<bool> alive; // row-major, cols * rows
};
