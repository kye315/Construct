/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "PCGKai.h"

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Raylib");

	// Initialize the map (Rows come first in memory for C arrays)
	int tileArray[MAP_ROWS][MAP_COLUMNS] = { 0 };

	CreateMap(tileArray);
	PrintMap(tileArray);

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	// wabbit movement
	int WabX = 0;
	
	int WabSpeed = 5;
	
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLUE);

		// Render the map data
		DrawMap(tileArray);

		// draw some text using the default font
		DrawText("Siiiiiiiiiix seeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee", 200, 100, 20, WHITE);

		// draw our texture to the screen
		DrawTexture(wabbit, WabX, 200, RED);

		WabX += WabSpeed;

		if (WabX > SCREEN_WIDTH)
		{
			WabX = 0;
			WabSpeed = GetRandomValue(2, 20);
		}

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

// Populates array using Row-Major order
void CreateMap(int _tileArray[MAP_ROWS][MAP_COLUMNS])
{
	for (int y = 0; y < MAP_ROWS; y++)
	{
		for (int x = 0; x < MAP_COLUMNS; x++)
		{
			_tileArray[y][x] = GetRandomValue(TILE_TYPE_GRASS, TILE_TYPE_ROCK);
		}
	}
}

// Standard console output
void PrintMap(int _tileArray[MAP_ROWS][MAP_COLUMNS])
{
	printf("\n-------Map Layout:--------\n");
	for (int y = 0; y < MAP_ROWS; y++)
	{
		for (int x = 0; x < MAP_COLUMNS; x++)
		{
			if (_tileArray[y][x] == TILE_TYPE_GRASS) {
				printf("%c", GRASS_CHAR);
			}
			else {
				printf("%c", ROCK_CHAR);
			}
		}
		printf("\n");
	}
}

void DrawMap(int _tileArray[MAP_ROWS][MAP_COLUMNS])
{
	for (int y = 0; y < MAP_ROWS; y++)
	{
		for (int x = 0; x < MAP_COLUMNS; x++)
		{
			Color tileColor = (_tileArray[y][x] == TILE_TYPE_GRASS) ? LIGHTGRAY : DARKGRAY;
			DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE - 1, TILE_SIZE - 1, tileColor);
		}
	}
}