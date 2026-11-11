#include "raylib.h"
#include "grid.h"
#include "player.h"

int main()
{
    // Window is twice as wide as it is tall; the grid fills the left half.
    const int screenHeight = 600;
    const int screenWidth = screenHeight * 2;

    // 3D view fills the right half, one wall column every 8 pixels.
    const Rectangle view = {(float)screenHeight, 0.0f,
                            (float)(screenWidth - screenHeight), (float)screenHeight};
    const int columnWidth = 8;
    const int rayCount = (int)view.width / columnWidth;

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
        std::vector<Vector2> hits = player.CastRays(grid, rayCount);
        player.DrawRays(hits);
        player.DrawWalls(hits, view, grid.GetBlockHeight());
        player.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
