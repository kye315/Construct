#ifndef PCG_Kai

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

// Defines
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024
#define TILE_SIZE 64

// Standard Map Dimensions
#define MAP_COLUMNS (SCREEN_WIDTH / TILE_SIZE)
#define MAP_ROWS (SCREEN_HEIGHT / TILE_SIZE)

// Tile Type
#define TILE_TYPE_GRASS 0
#define TILE_TYPE_ROCK 1

// Console Representation
#define GRASS_CHAR '.'
#define ROCK_CHAR '#'

// Forward declarations
void CreateMap(int _tileArray[MAP_ROWS][MAP_COLUMNS]);
void DrawMap(int _tileArray[MAP_ROWS][MAP_COLUMNS]);
void PrintMap(int _tileArray[MAP_ROWS][MAP_COLUMNS]); #pragma once
#endif