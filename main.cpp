#include "raylib.h"
#include "grid.h"
#include "player.h"

int main()
{
    // Window is twice as wide as it is tall; the grid fills the left half.
    const int screenHeight = 600;
    const int screenWidth = screenHeight * 2;

    InitWindow(screenWidth, screenHeight, "raycast");
    SetTargetFPS(60);

    Grid grid;

    grid.Randomize();

    Player player;
    player.Spawn(grid);

    while (!WindowShouldClose())
    {
        player.Move(grid);

        BeginDrawing();
        ClearBackground(BLACK);
        grid.Draw();
        player.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
