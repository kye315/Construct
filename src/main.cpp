#include "raylib.h"
#include "resource_dir.h"
#include "PCG.h" // Import our new module

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(PCG::SCREEN_WIDTH, PCG::SCREEN_HEIGHT, "Construct Map Editor");

    PCG::TileType tileArray[PCG::MAP_ROWS][PCG::MAP_COLUMNS] = { PCG::TileType::TILE_TYPE_WALL };
    PCG::WalkBehaviour initialWalkBehaviour = PCG::WalkBehaviour{ };
    PCG::CreateMap(tileArray, initialWalkBehaviour);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        PCG::DrawMap(tileArray); // Function from PCG.c
        DrawText("Construct Map Editor", 20, 20, 20, WHITE);
        PCG::DrawGUI(tileArray);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}