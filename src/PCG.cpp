#include "PCG.h"
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <fstream>

// ============================================= 
// WALL GENERATOR
// ============================================= 
void PCG::CreateMap(PCG::TileType _tileArray[PCG::MAP_ROWS][PCG::MAP_COLUMNS], WalkBehaviour _walkBehaviour) {

	// START WALL FILL
	for (int y = 0; y < PCG::MAP_ROWS; y++) {
		for (int x = 0; x < PCG::MAP_COLUMNS; x++) {
			_tileArray[y][x] = PCG::TileType::TILE_TYPE_WALL;
		}
	}

	// START WALK
	bool walking = true;

	// current walker position
	int x = 0;
	int y = PCG::MAP_ROWS / 2;

	// current mode
	int walkMode = 1;

	// debug
	int steps = 0;
	int turnSteps = 0;

	_tileArray[y][x] = PCG::TileType::TILE_TYPE_ENTRANCE;
	x++;

	while (walking) // repeatedly walks until reaches end
	{
		steps++;
		std::cout << "[STEP " << steps << "]";
		_tileArray[y][x] = PCG::TileType::TILE_TYPE_PATH;

		if (GetRandomValue(0, 100) > _walkBehaviour.R_TURN)
		{
			turnSteps++;
			if (GetRandomValue(0, 100) > 50)
			{
				walkMode++;
				std::cout << "Turned right!";
			}
			else
			{
				walkMode--;
				std::cout << "Turned left!";
			}
			walkMode = std::clamp(walkMode, 0, 2);
		}
		switch (walkMode) {

		case 0: // moving up
			y--;
			if (y < 0 || _tileArray[y][x] == PCG::TileType::TILE_TYPE_PATH || PCG::PathIsBordered(walkMode, _tileArray, y, x) == false)
			{
				y++;
				walkMode = 1;
				std::cout << "Turned right! (FAILSAFE)";
			}
			break;

		case 1: // moving forward
			x++;
			break;

		case 2: // moving down
			y++;
			if (y > (PCG::MAP_ROWS - 1) || _tileArray[y][x] == PCG::TileType::TILE_TYPE_PATH || PCG::PathIsBordered(walkMode, _tileArray, y, x) == false) {
				y--;
				walkMode = 1;
				std::cout << "Turned left! (FAILSAFE)";
			}
			break;

		default: // no movement failsafe
			std::cout << "WARNING: No movement!";
			break;
		}

		std::cout << "Walked to " << y << ", " << x << "!";

		if (x == PCG::MAP_COLUMNS) {
			walking = false; // reset to avoid inf loop
			std::cout << "Completed in " << steps << " steps!";
			std::cout << turnSteps << " out of " << steps << " turned around!";
		}
	}

	//for (int y = 0; y < PCG::MAP_ROWS; y++) {
	//    for (int x = 0; x < PCG::MAP_COLUMNS; x++) {
	//        int selection;
	//        int roll = GetRandomValue(0, 100);
	//        if (roll > PCG::TileType::COLORTHRESH) { selection = 0; }
	//        else { selection = 1; }


	//        _tileArray[y][x] = (PCG::TileType)selection;
	//    }
	//}
}

// ============================================= 
// Bool PathIsBordered
// Checks for neighboring paths based on direction
// ============================================= 
bool PCG::PathIsBordered(int _walkMode, TileType _tileArray[PCG::MAP_ROWS][PCG::MAP_COLUMNS], int y, int x)
{
	switch (_walkMode)
	{

	case(0): // up
		if (_tileArray[y][x - 1] != PCG::TileType::TILE_TYPE_PATH && _tileArray[y][x - 2] != PCG::TileType::TILE_TYPE_PATH && _tileArray[y][x + 1] != PCG::TileType::TILE_TYPE_PATH && _tileArray[y][x + 2] != PCG::TileType::TILE_TYPE_PATH)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;

	case(1): // right
		if (_tileArray[y - 1][x] != PCG::TileType::TILE_TYPE_PATH && _tileArray[y - 2][x] != PCG::TileType::TILE_TYPE_PATH && _tileArray[y + 1][x] != PCG::TileType::TILE_TYPE_PATH && _tileArray[y + 2][x] != PCG::TileType::TILE_TYPE_PATH)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;

	case (2): // down
		if (_tileArray[y][x - 2] != PCG::TileType::TILE_TYPE_PATH && _tileArray[y][x + 2] != PCG::TileType::TILE_TYPE_PATH && _tileArray[y][x - 1] != PCG::TileType::TILE_TYPE_PATH && _tileArray[y][x + 1] != PCG::TileType::TILE_TYPE_PATH)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	default:
		return false;
		break;
	}
}


// ============================================= 
// Color PCG_GetTileColor(TileType tileType)
// Return a colour based on the type type input
// ============================================= 
Color PCG::GetTileColor(PCG::TileType tileType) {
	switch (tileType) {
	case PCG::TILE_TYPE_WALL: return PCG::WALL_COLOR;
	case PCG::TILE_TYPE_PATH: return PCG::PATH_COLOR;
	case PCG::TILE_TYPE_BRANCHPATH: return PCG::BRANCHPATH_COLOR;
	case PCG::TILE_TYPE_BRANCHEND: return PCG::BRANCHEND_COLOR;
	case PCG::TILE_TYPE_ROOMCENTER: return PCG::ROOMCENTER_COLOR;
	case PCG::TILE_TYPE_ENTRANCE: return PCG::ENTRANCE_COLOR;

	default: return PCG::UNKNOWN_COLOR;
	}
}


// ============================================= 
// void PCG_DrawMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS])
// ============================================= 
void PCG::DrawMap(PCG::TileType _tileArray[PCG::MAP_ROWS][PCG::MAP_COLUMNS]) {
	for (int y = 0; y < PCG::MAP_ROWS; y++) {
		for (int x = 0; x < PCG::MAP_COLUMNS; x++) {
			DrawRectangle(x * PCG::TILE_SIZE, y * PCG::TILE_SIZE, PCG::TILE_SIZE, PCG::TILE_SIZE, PCG::GetTileColor(_tileArray[y][x]));
		}
	}
}

// ============================================= 
// void PCG_PrintMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS])
// ============================================= 
void PCG::PrintMap(PCG::TileType _tileArray[PCG::MAP_ROWS][PCG::MAP_COLUMNS]) {
	printf("\n-------Map Layout:--------\n");
	// (Existing Print Logic here...)
	printf("--------------------------\n");
}

// ============================================= 
// char GetTileChar(TileType tileType)
// Return a char value based on the type of tile passed in
// ============================================= 
char PCG::GetTileChar(PCG::TileType tileType) {
	switch (tileType) {
	case PCG::TILE_TYPE_WALL: return PCG::WALL_CHAR;
	case PCG::TILE_TYPE_PATH: return PCG::PATH_CHAR;
	default: return '?';
	}
}

// ============================================= 
// void PCG_SaveMapData(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* _filename)
// Store our tilemap data to a text file using the input _filename
// ============================================= 
void PCG::SaveMapData(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* _filename) {
	std::fstream file;
	file.open(_filename, std::ios::out); // Open C++ file stream for writing
	//if (file == nullptr) {    // old C-style file open check
	if (!file.is_open()) { // Check if file opened successfully
		return;
	}

	// Write each single tileArray charater into our file stream
	for (int y = 0; y < PCG::MAP_ROWS; y++) {
		for (int x = 0; x < PCG::MAP_COLUMNS; x++) {
			//fputc(PCG::GetTileChar(_tileArray[y][x]), file);  // old c-style file write
			file.put(PCG::GetTileChar(_tileArray[y][x])); // Write char to C++ file stream
		}
		//fputc('\n', file); // New line at end of row
		file.put('\n'); // New line at end of row for C++ file stream
	}
	//fclose(file); // old C-style file close
	file.close(); // Close C++ file stream
	std::cout << "Map saved to " << _filename << std::endl;
}


// ============================================= 
// void PCG_LoadMapData(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* _filename)
// Load our tilemap data from a text file, using input _filename
// ============================================= 
void PCG::LoadMapData(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* _filename) {
	//FILE* file = fopen(_filename, "r"); // "r" = Read // old c style
	std::fstream file;  // C++ file stream object for reading
	file.open(_filename, std::ios::in); // Open C++ file stream for reading
	//if (file == NULL) {   // old C-style file open check
	if (!file.is_open()) { // Check if file opened successfully for C++ stream
		return;
	}

	// Get each character from our file stream, and load it into our tileMap array
	for (int y = 0; y < PCG::MAP_ROWS; y++) {
		for (int x = 0; x < PCG::MAP_COLUMNS; x++) {
			//int ch = fgetc(file); // old C-style file read
			int ch = file.get(); // Get char from C++ file stream
			// Skip invisible newline characters
			while (ch == '\n' || ch == '\r') {
				//ch = fgetc(file); // old C-style file read for skipping newlines
				ch = file.get(); // Get char from C++ file stream for skipping newlines
			}

			if (ch == PCG::WALL_CHAR) {
				_tileArray[y][x] = PCG::TileType::TILE_TYPE_WALL;
			}
			else if (ch == PCG::PATH_CHAR) {
				_tileArray[y][x] = PCG::TileType::TILE_TYPE_PATH;
			}
		}
	}
	//fclose(file); // old C-style file close
	file.close(); // Close C++ file stream
	// printf("Map loaded from %s\n", _filename);   // old C-style print statement
	std::cout << "Map loaded from " << _filename << std::endl; // C++ style print statement
}

// ============================================= 
// void PCG_SaveMapImage(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* filename)
// Store our tileMap data as a .png image, using the input filename.
// ============================================= 
void PCG::SaveMapImage(PCG::TileType _tileArray[PCG::MAP_ROWS][PCG::MAP_COLUMNS], const char* filename) {
	Image mapImage = GenImageColor(PCG::MAP_COLUMNS, PCG::MAP_ROWS, BLACK);

	for (int y = 0; y < PCG::MAP_ROWS; y++) {
		for (int x = 0; x < PCG::MAP_COLUMNS; x++) {
			Color c = PCG::GetTileColor(_tileArray[y][x]);
			ImageDrawPixel(&mapImage, x, y, c);
		}
	}
	if (ExportImage(mapImage, filename)) {
		printf("Image saved: %s\n", filename);
	}
	UnloadImage(mapImage);
}

// Required to call Raylib gui buttons. Add this near the top of PCG.c
#define RAYGUI_IMPLEMENTATION
#include "raygui.h" 

// ============================================= 
// void PCG_DrawGUI(TileType tileArray[MAP_ROWS][MAP_COLUMNS])
// ============================================= 
void PCG::DrawGUI(PCG::TileType tileArray[PCG::MAP_ROWS][PCG::MAP_COLUMNS]) {
	// Reset Button
	if (GuiButton(PCG::RESET_BUTTON_BOUNDS, "Reset Map")) {
		PCG::CreateMap(tileArray, PCG::WalkBehaviour{ 50, 80 });
	}

	// Save Data Button
	Rectangle saveRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 70, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
	if (GuiButton(saveRect, "Save Map Data")) {
		PCG::SaveMapData(tileArray, PCG::MAP_TEXT_FILENAME);
	}

	// Load Data Button
	Rectangle loadRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 140, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
	if (GuiButton(loadRect, "Load Map Data")) {
		PCG::LoadMapData(tileArray, PCG::MAP_TEXT_FILENAME);
	}

	// Save Image Button
	Rectangle imgRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 210, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
	if (GuiButton(imgRect, "Save Map PNG")) {
		PCG::SaveMapImage(tileArray, PCG::MAP_IMAGE_FILENAME);
	}

	//// increase roll
	//Rectangle incRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 280, PCG::BUTTON_WIDTH / 2, PCG::BUTTON_HEIGHT };
	//if (GuiButton(incRect, "Increase Roll")) {
	//    PCG::COLORTHRESH += 5;
	//}
	//
	//// decrease roll
	//Rectangle decRect = { PCG::BUTTON_X + (PCG::BUTTON_WIDTH / 2), PCG::BUTTON_Y - 280, PCG::BUTTON_WIDTH / 2, PCG::BUTTON_HEIGHT};
	//if (GuiButton(decRect, "Decrease Roll")) {
	//    PCG::COLORTHRESH -= 5;
	//}
}