#include "PCG.h"
#include <stdio.h>
// Required to call Raylib gui buttons. Add this near the top of PCG.c
#define RAYGUI_IMPLEMENTATION 
#include "raygui.h" 


// ============================================= 
// Color PCG_GetTileColor(TileType tileType)
// Return a colour based on the type type input
// ============================================= 
Color PCG::GetTileColor(PCG::TileType tileType) {
	switch (tileType) {
	case PCG::TILE_TYPE_WALL: return PCG::WALL_COLOR;
	case PCG::TILE_TYPE_PATH: return PCG::PATH_COLOR;
	case PCG::TILE_TYPE_BRANCHSTART: return PCG::BRANCHSTART_COLOR;
	case PCG::TILE_TYPE_BRANCHPATH: return PCG::BRANCHPATH_COLOR;
	case PCG::TILE_TYPE_BRANCHEND: return PCG::BRANCHEND_COLOR;
	case PCG::TILE_TYPE_ROOMCENTER: return PCG::ROOMCENTER_COLOR;
	case PCG::TILE_TYPE_ROOMPATH: return PCG::ROOMPATH_COLOR;
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
	case PCG::TILE_TYPE_BRANCHSTART: return PCG::BRANCHSTART_CHAR;
	case PCG::TILE_TYPE_BRANCHPATH: return PCG::BRANCHPATH_CHAR;
	case PCG::TILE_TYPE_BRANCHEND: return PCG::BRANCHEND_CHAR;
	case PCG::TILE_TYPE_ROOMCENTER: return PCG::ROOMCENTER_CHAR;
	case PCG::TILE_TYPE_ROOMPATH: return PCG::ROOMPATH_CHAR;
	case PCG::TILE_TYPE_ENTRANCE: return PCG::ENTRANCE_CHAR;
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

// ============================================= 
// void PCG_DrawGUI(TileType tileArray[MAP_ROWS][MAP_COLUMNS])
// ============================================= 
void PCG::DrawGUI(PCG::TileType tileArray[PCG::MAP_ROWS][PCG::MAP_COLUMNS], WalkBehaviour _walkBehaviour) {
	// Reset Button
	if (GuiButton(PCG::RESET_BUTTON_BOUNDS, "Reset Map")) {
		PCG::CreateMap(tileArray, _walkBehaviour);
	}

	// Save Data Button
	Rectangle saveRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 70, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
	if (GuiButton(saveRect, "Save Map Data")) {
		PCG::SaveMapData(tileArray, PCG::MAP_TEXT_FILENAME);
	}

	Rectangle expositionRect = { 50, 90, PCG::BUTTON_WIDTH, 500 };
	GuiLabel(expositionRect, "Paint instructions; \nQ = wall brush; \nW = path brush; \nE = branch entrance brush; \nA = branch pathway brush; \nS = branch end brush; \nD = room center brush; \nZ = room path brush; \nC = place entrance");

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

	//// increase branch rate
	//Rectangle incRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 280, PCG::BUTTON_WIDTH / 2, PCG::BUTTON_HEIGHT };
	//if (GuiButton(incRect, "Increase Branch Rate")) {
	//	initialWalkBehaviour.R_MAKEBRANCH = std::clamp(initialWalkBehaviour.R_MAKEBRANCH + 5, 0, 100);
	//}

	//// decrease roll
	//Rectangle decRect = { PCG::BUTTON_X + (PCG::BUTTON_WIDTH / 2), PCG::BUTTON_Y - 280, PCG::BUTTON_WIDTH / 2, PCG::BUTTON_HEIGHT};
	//if (GuiButton(decRect, "Decrease Branch Rate")) {
	//	initialWalkBehaviour.R_MAKEBRANCH = std::clamp(initialWalkBehaviour.R_MAKEBRANCH - 5, 0, 100);
	//}
}

PCG::WalkBehaviour PCG::WBGui(WalkBehaviour _walkBehaviour)
{

	// increase turn rate
	Rectangle incTurnRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 280, PCG::BUTTON_WIDTH / 2, PCG::BUTTON_HEIGHT };
	if (GuiButton(incTurnRect, "more turns")) {
		_walkBehaviour.R_TURN = std::clamp(_walkBehaviour.R_TURN - 5, 0, 100);
	}

	// decrease turn rate
	Rectangle decTurnRect = { PCG::BUTTON_X + (PCG::BUTTON_WIDTH / 2), PCG::BUTTON_Y - 280, PCG::BUTTON_WIDTH / 2, PCG::BUTTON_HEIGHT};
	if (GuiButton(decTurnRect, "less turns")) {
		_walkBehaviour.R_TURN = std::clamp(_walkBehaviour.R_TURN + 5, 0, 100);
	}

	// increase branch rate
	Rectangle incBranchRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 350, PCG::BUTTON_WIDTH / 2, PCG::BUTTON_HEIGHT };
	if (GuiButton(incBranchRect, "more branches")) {
		_walkBehaviour.R_MAKEBRANCH = std::clamp(_walkBehaviour.R_MAKEBRANCH - 5, 0, 100);
	}

	// decrease branch rate
	Rectangle decBranchRect = { PCG::BUTTON_X + (PCG::BUTTON_WIDTH / 2), PCG::BUTTON_Y - 350, PCG::BUTTON_WIDTH / 2, PCG::BUTTON_HEIGHT };
	if (GuiButton(decBranchRect, "less branches")) {
		_walkBehaviour.R_MAKEBRANCH = std::clamp(_walkBehaviour.R_MAKEBRANCH + 5, 0, 100);
	}

	// increase room rate
	Rectangle incRoomRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 420, PCG::BUTTON_WIDTH / 2, PCG::BUTTON_HEIGHT }; //blaze it
	if (GuiButton(incRoomRect, "more rooms")) {
		_walkBehaviour.R_MAKEROOM = std::clamp(_walkBehaviour.R_MAKEROOM - 5, 0, 100);
	}

	// decrease room rate
	Rectangle decRoomRect = { PCG::BUTTON_X + (PCG::BUTTON_WIDTH / 2), PCG::BUTTON_Y - 420, PCG::BUTTON_WIDTH / 2, PCG::BUTTON_HEIGHT };
	if (GuiButton(decRoomRect, "less rooms")) {
		_walkBehaviour.R_MAKEROOM = std::clamp(_walkBehaviour.R_MAKEROOM + 5, 0, 100);
	}

	// increase branch length
	Rectangle longBranchRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 490, PCG::BUTTON_WIDTH / 2, PCG::BUTTON_HEIGHT }; 
	if (GuiButton(longBranchRect, "longer branches")) {
		_walkBehaviour.BRANCH_LENGTH = std::clamp(_walkBehaviour.BRANCH_LENGTH + 1, 0, 100);
	}

	// decrease branch length
	Rectangle shortBranchRect = { PCG::BUTTON_X + (PCG::BUTTON_WIDTH / 2), PCG::BUTTON_Y - 490, PCG::BUTTON_WIDTH / 2, PCG::BUTTON_HEIGHT };
	if (GuiButton(shortBranchRect, "shorter branches")) {
		_walkBehaviour.BRANCH_LENGTH = std::clamp(_walkBehaviour.BRANCH_LENGTH - 1, 0, 100);
	}

	// increase room size
	Rectangle longRoomRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 490, PCG::BUTTON_WIDTH / 2, PCG::BUTTON_HEIGHT };
	if (GuiButton(longRoomRect, "bigger rooms")) {
		_walkBehaviour.ROOM_SIZE = std::clamp(_walkBehaviour.ROOM_SIZE + 1, 1, 50);
	}

	// decrease branch length
	Rectangle shortRoomRect = { PCG::BUTTON_X + (PCG::BUTTON_WIDTH / 2), PCG::BUTTON_Y - 490, PCG::BUTTON_WIDTH / 2, PCG::BUTTON_HEIGHT };
	if (GuiButton(shortRoomRect, "smaller rooms")) {
		_walkBehaviour.ROOM_SIZE = std::clamp(_walkBehaviour.ROOM_SIZE - 1, 1, 50);
	}


	return _walkBehaviour;
}

// ============================================= 
// void PCG_GetInput
// ============================================= 
bool PCG::GetInput(TileType _tileArray[MAP_ROWS][MAP_COLUMNS])
{
	Vector2 GMP = GetMousePosition();
	int x = std::round(GMP.x / TILE_SIZE);
	int y = std::round(GMP.y / TILE_SIZE);

	TileType Paint;

	bool Pressed = false;

	if (IsKeyDown(KEY_Q))
	{
		Paint = PCG::TILE_TYPE_WALL;
		Pressed = true;
	}
	if (IsKeyDown(KEY_W))
	{
		Paint = PCG::TILE_TYPE_PATH;
		Pressed = true;
	}
	if (IsKeyDown(KEY_E))
	{
		Paint = PCG::TILE_TYPE_BRANCHSTART;
		Pressed = true;
	}
	if (IsKeyDown(KEY_A))
	{
		Paint = PCG::TILE_TYPE_BRANCHPATH;
		Pressed = true;
	}
	if (IsKeyDown(KEY_S))
	{
		Paint = PCG::TILE_TYPE_BRANCHEND;
		Pressed = true;
	}
	if (IsKeyDown(KEY_D))
	{
		Paint = PCG::TILE_TYPE_ROOMCENTER;
		Pressed = true;
	}
	if (IsKeyDown(KEY_Z))
	{
		Paint = PCG::TILE_TYPE_ROOMPATH;
		Pressed = true;
	}
	if (IsKeyDown(KEY_C))
	{
		Paint = PCG::TILE_TYPE_ENTRANCE;
		Pressed = true;
	}

	if (Pressed)
	{
		_tileArray[y][x] = Paint;

		std::cout << GMP.x << "..." << GMP.y << "...";
		return false; // DO NOT draw GUI
	}
	return true; // DRAW gui
}