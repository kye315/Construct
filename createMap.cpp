#include "PCG.h"


// ============================================= 
// MAP GENERATOR
// ============================================= 

// ============================================= 
// void CreateMap
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

		int _y = y;

		int _x = x;

		switch (walkMode) {

		case 0: // moving up
			_y--;
			break;

		case 1: // moving forward
			_x++;
			break;

		case 2: // moving down
			_y++;
			break;

		default: // no movement failsafe
			std::cout << "WARNING: No movement!";
			break;
		}

		if (PCG::PathIsBordered(walkMode, _tileArray, _y, _x) == false || _y - 1 < 0 || _y + 1 > (PCG::MAP_ROWS - 1))
		{
			walkMode = 1;
			x++;
		}
		else
		{
			x = _x;
			y = _y;
		}


		if (GetRandomValue(0, 100) > _walkBehaviour.R_TURN) // turn check
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

		if (GetRandomValue(0, 100) > _walkBehaviour.R_MAKEBRANCH) // branch check
		{ 
			if (PCG::PathIsBordered(walkMode, _tileArray, y, x))
			{
				int sendingY = y;
				int sendingX = x;
				int sendingWM;
				if (walkMode == 1)
				{
					if (GetRandomValue(0, 100) > 50)
					{
						sendingY--;
						sendingWM = 0; // up
					}
					else
					{
						sendingY++;
						sendingWM = 2; // down
					}
				}
				else
				{
					if (GetRandomValue(0, 100) > 50)
					{
						sendingX++;
						sendingWM = 1; // right
					}
					else
					{
						sendingX--;
						sendingWM = 3; // left
					}
				}
				PCG::Sprout(_tileArray, _walkBehaviour, sendingY, sendingX, sendingWM);
			}
		}

		std::cout << "Walked to " << y << ", " << x << "!";

		if (x == PCG::MAP_COLUMNS) {
			walking = false; // reset to avoid inf loop
			std::cout << "Completed in " << steps << " steps!";
			std::cout << turnSteps << " out of " << steps << " turned around!";
		}
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

// ============================================= 
// void Sprout
// ============================================= 
void PCG::Sprout(PCG::TileType _tileArray[PCG::MAP_ROWS][PCG::MAP_COLUMNS], WalkBehaviour _walkBehaviour, int _starty, int _startx, int startWalkMode)
{
	bool sproutWalking = true;

	int sproutLength = (_walkBehaviour.BRANCH_LENGTH) + (GetRandomValue(-_walkBehaviour.BRANCH_SCRAMBLE, _walkBehaviour.BRANCH_SCRAMBLE));

	int _y = _starty;
	int _x = _startx;

	int __y;
	int __x;

	int walkMode = startWalkMode;

	_tileArray[_y][_x] = PCG::TileType::TILE_TYPE_BRANCHSTART;

	std::cout << "[[Begin sprout walk!";

	while (sproutWalking) {
		__y = _y;
		__x = _x;
		sproutLength--;
		switch (walkMode) {

		case 0: // moving up
			__y--;
			break;

		case 1: // moving forward
			__x++;
			break;

		case 2: // moving down
			__y++;
			break;

		case 3: // moving left
			__x--;
			break;

		default: // no movement failsafe
			std::cout << "WARNING: No movement!";
			break;
		}

		if (PCG::PathIsBordered(walkMode, _tileArray, __y, __x) == false) 
		{
			_y = __y;
			_x = __x;

			_tileArray[_y][_x] = PCG::TileType::TILE_TYPE_BRANCHPATH;
		}
		else
		{
			walkMode++;
		}

		if (GetRandomValue(0, 100) > _walkBehaviour.R_TURN) // turn check
		{
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

		if (walkMode > 3)
		{
			walkMode = 0;
		}

		if (walkMode < 0)
		{
			walkMode = 3;
		}

		if (sproutLength <= 0)
		{
			sproutWalking = false;
			_tileArray[_y][_x] = PCG::TileType::TILE_TYPE_BRANCHEND;
			std::cout << "End sprout walk!]]";
		}
	}


}

// ============================================= 
// Bool PathIsBordered
// Checks for neighboring paths based on direction
// ============================================= 
bool PCG::PathIsBordered(int _case, TileType _tileArray[PCG::MAP_ROWS][PCG::MAP_COLUMNS], int y, int x)
{
	switch (_case)
	{

	case(0): // up
		if (_tileArray[y][x] == PCG::TileType::TILE_TYPE_WALL && _tileArray[y][x - 1] == PCG::TileType::TILE_TYPE_WALL && _tileArray[y][x - 2] == PCG::TileType::TILE_TYPE_WALL && _tileArray[y][x + 1] == PCG::TileType::TILE_TYPE_WALL && _tileArray[y][x + 2] == PCG::TileType::TILE_TYPE_WALL)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;

	case(1): // right
		if (_tileArray[y][x] == PCG::TileType::TILE_TYPE_WALL && _tileArray[y - 1][x] == PCG::TileType::TILE_TYPE_WALL && _tileArray[y - 2][x] == PCG::TileType::TILE_TYPE_WALL && _tileArray[y + 1][x] == PCG::TileType::TILE_TYPE_WALL && _tileArray[y + 2][x] == PCG::TileType::TILE_TYPE_WALL)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;

	case (2): // down
		if (_tileArray[y][x] == PCG::TileType::TILE_TYPE_WALL && _tileArray[y][x - 2] == PCG::TileType::TILE_TYPE_WALL && _tileArray[y][x + 2] == PCG::TileType::TILE_TYPE_WALL && _tileArray[y][x - 1] == PCG::TileType::TILE_TYPE_WALL && _tileArray[y][x + 1] == PCG::TileType::TILE_TYPE_WALL)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case (3): // left (rare)
		if (_tileArray[y][x] == PCG::TileType::TILE_TYPE_WALL && _tileArray[y - 1][x] == PCG::TileType::TILE_TYPE_WALL && _tileArray[y - 2][x] == PCG::TileType::TILE_TYPE_WALL && _tileArray[y + 1][x] == PCG::TileType::TILE_TYPE_WALL && _tileArray[y + 2][x] == PCG::TileType::TILE_TYPE_WALL && _tileArray[y][x - 1] == PCG::TileType::TILE_TYPE_WALL && _tileArray[y][x - 2] == PCG::TileType::TILE_TYPE_WALL)
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