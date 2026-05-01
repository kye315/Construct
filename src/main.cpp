#include "raylib.h"
#include "resource_dir.h"
#include "PCG.h" // Import our new module
#if defined(_WIN32) && !defined(_DEBUG)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(PCG::SCREEN_WIDTH, PCG::SCREEN_HEIGHT, "Construct Demigeon");
    PCG::TileType tileArray[PCG::MAP_ROWS][PCG::MAP_COLUMNS] = { PCG::TileType::TILE_TYPE_WALL };
    PCG::WalkBehaviour initialWalkBehaviour = PCG::WalkBehaviour{ };

    PCG::CreateMap(tileArray, initialWalkBehaviour);

    bool shouldDraw;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        PCG::DrawMap(tileArray); // Function from PCG.c
        DrawText("Construct Demigeon", 20, 20, 20, WHITE);
        shouldDraw = PCG::GetInput(tileArray);
        if (shouldDraw)
        {
            initialWalkBehaviour = PCG::WBGui(initialWalkBehaviour);
            std::cout << initialWalkBehaviour.ROOM_SIZE << '\n';
            PCG::DrawGUI(tileArray, initialWalkBehaviour);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}