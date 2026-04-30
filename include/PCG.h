#pragma once
#include "raylib.h"

namespace PCG {
    // Screen & Map Dimensions
    constexpr int SCREEN_WIDTH = 1024;
    constexpr int SCREEN_HEIGHT = 1024;
    constexpr int TILE_SIZE = 16;
    constexpr int MAP_COLUMNS = (SCREEN_WIDTH / TILE_SIZE);
    constexpr int MAP_ROWS = (SCREEN_HEIGHT / TILE_SIZE);

    // Tile Types (Using Enum for readability)
    typedef enum {
        TILE_TYPE_WALL = 0,
        TILE_TYPE_PATH = 1,
        TILE_TYPE_BRANCHPATH = 2,
        TILE_TYPE_BRANCHEND = 3,
        TILE_TYPE_ROOMCENTER = 4,
        TILE_TYPE_ENTRANCE = 5,
        TILE_COUNT  // Automatically counts total types
    } TileType;
     
    // Lower = more probable
    typedef struct {
        int R_TURN = 50;
        int R_MAKEBRANCH = 80;
    } WalkBehaviour;

    // Visual & Character settings
    constexpr char WALL_CHAR = '#';
    constexpr char PATH_CHAR = '+';
    constexpr char ITEM_CHAR = '!';
    constexpr Color WALL_COLOR = { 40, 40, 40, 255 };
    constexpr Color PATH_COLOR = { 150, 150, 150, 255 };
    constexpr Color BRANCHPATH_COLOR = { 150, 180, 150, 255 };
    constexpr Color BRANCHEND_COLOR = { 150, 255, 150, 255 };
    constexpr Color ROOMCENTER_COLOR = { 255, 255, 0, 255 };
    constexpr Color ENTRANCE_COLOR = { 255, 0, 0, 255 };
    constexpr Color UNKNOWN_COLOR = WHITE;

    // Function Declarations
    void CreateMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], WalkBehaviour _walkBehaviour);
    bool PathIsBordered(int _walkMode, TileType _tileArray[PCG::MAP_ROWS][PCG::MAP_COLUMNS], int y, int x);

    void DrawMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]);
    void PrintMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]);
    Color GetTileColor(TileType tileType);

    // File Names
    constexpr char* MAP_TEXT_FILENAME = "pcg_map_data.txt";

    // Helpers
    char GetTileChar(TileType tileType);

    // I/O Functions
    void SaveMapData(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* filename);
    void LoadMapData(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* filename);

    constexpr char* MAP_IMAGE_FILENAME = "pcg_map.png";
    void SaveMapImage(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* filename);

    // UI variable defines used to position buttons on screen
    constexpr int BUTTON_WIDTH = 200;
    constexpr int BUTTON_HEIGHT = 50;
    constexpr int BUTTON_X = (SCREEN_WIDTH - BUTTON_WIDTH - 20);
    constexpr int BUTTON_Y = (SCREEN_HEIGHT - BUTTON_HEIGHT - 20);
    constexpr Rectangle RESET_BUTTON_BOUNDS = { BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };

    // Declare UI drawing function
    void DrawGUI(TileType tileArray[MAP_ROWS][MAP_COLUMNS]);

}