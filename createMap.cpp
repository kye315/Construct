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

	std::vector<BranchProperties> bps;

	_tileArray[y][x] = PCG::TileType::TILE_TYPE_ENTRANCE;
	x++;

	// Part 1: Walk
	while (walking) // repeatedly walks until reaches end
	{
		steps++;
		std::cout << "[STEP " << steps << "]";
		if ((GetRandomValue(0, 100) > _walkBehaviour.R_MAKEROOM) && PathIsBordered(4, _tileArray, y, x, _walkBehaviour)) // room check
		{
			MakeRoom(_tileArray, _walkBehaviour, y, x);
			_tileArray[y][x] = PCG::TileType::TILE_TYPE_ROOMCENTER;
			x += ((_walkBehaviour.ROOM_SIZE / 2));
		}
		else
		{
			_tileArray[y][x] = PCG::TileType::TILE_TYPE_PATH;
		}


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

		if (PCG::PathIsBordered(walkMode, _tileArray, _y, _x, _walkBehaviour) == false || _y - 1 < 0 || _y + 1 > (PCG::MAP_ROWS - 1))
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
			walkMode = CoinFlip(walkMode - 1, walkMode + 1);
			walkMode = std::clamp(walkMode, 0, 2);
		}

		if (GetRandomValue(0, 100) > _walkBehaviour.R_MAKEBRANCH) // branch check
		{
			int sendingY = y;
			int sendingX = x;
			int sendingWM;
			int Flip = CoinFlip(-1, 1);
			if (walkMode == 1)
			{
				sendingY += Flip;
				sendingWM = (1 - Flip);
			}
			else
			{
				sendingX += Flip;
				sendingWM = (2 - Flip); // right
			}

			BranchProperties newBP = BranchProperties{ sendingX, sendingY, sendingWM };

			bps.push_back(newBP);
		}

		std::cout << "Walked to " << y << ", " << x << "!";

		if (x >= PCG::MAP_COLUMNS) {
			walking = false; // reset to avoid inf loop
			std::cout << "Completed in " << steps << " steps!";
			std::cout << turnSteps << " out of " << steps << " turned around!";
		}
	}

	// Part 2: Branch
	for (int i = 0; i < bps.size(); i++)
	{
		if (PCG::PathIsBordered(bps[i].startWM, _tileArray, bps[i].starty, bps[i].startx, _walkBehaviour)) // true if clear to go
		{
			PCG::Sprout(_tileArray, _walkBehaviour, bps[i].starty, bps[i].startx, bps[i].startWM);
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

	int sproutSteps = 0;

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

		if (PCG::PathIsBordered(walkMode, _tileArray, __y, __x, _walkBehaviour) && __x < PCG::MAP_ROWS - 1 && __x > 2)
		{
			_y = __y;
			_x = __x;

			sproutSteps++;

			_tileArray[_y][_x] = PCG::TileType::TILE_TYPE_BRANCHPATH;
		}
		else
		{
			walkMode++;
		}

		if (GetRandomValue(0, 100) > _walkBehaviour.R_TURN) // turn check
		{
			walkMode += CoinFlip(-1, +1);
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
			std::cout << "End sprout walk!]]";
			if (sproutSteps != 0) {
				if (GetRandomValue(0, 100) > _walkBehaviour.R_MAKEROOM / 2) // room check
				{
					int Offset = ((_walkBehaviour.ROOM_SIZE / 2) + 1);
					int sendingX = _x;
					int sendingY = _y;
					switch (walkMode)
					{
					case 0: // up
						sendingY -= Offset;
						break;
					case 1: // right
						sendingX += Offset;
						break;
					case 2: // down
						sendingY += Offset;
						break;
					case 3: // left
						sendingX -= Offset;
						break;
					}
					if (PathIsBordered(4, _tileArray, sendingY, sendingX, _walkBehaviour))
					{
						MakeRoom(_tileArray, _walkBehaviour, sendingY, sendingX);
						_tileArray[sendingY][sendingX] = PCG::TileType::TILE_TYPE_ROOMCENTER;
						return;
					}
				}
				_tileArray[_y][_x] = PCG::TileType::TILE_TYPE_BRANCHEND;
			}
			else {
				_tileArray[_y][_x] = PCG::TileType::TILE_TYPE_WALL;
			}
		}
	}

	return;
}

// ============================================= 
// Bool PathIsBordered
// Checks for neighboring paths based on direction
// ============================================= 
bool PCG::PathIsBordered(int _case, TileType _tileArray[PCG::MAP_ROWS][PCG::MAP_COLUMNS], int y, int x, WalkBehaviour _walkBehaviour)
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
	case (4): // room
		std::vector<int> xRange;
		for (int i = (-_walkBehaviour.ROOM_SIZE / 2); i <= _walkBehaviour.ROOM_SIZE / 2; i++)
		{
			for (int z = (-_walkBehaviour.ROOM_SIZE / 2); z <= _walkBehaviour.ROOM_SIZE / 2; z++)
			{
				if (_tileArray[y + i][x + z] != PCG::TileType::TILE_TYPE_WALL && _tileArray[y + i][x + z] != PCG::TileType::TILE_TYPE_PATH || (x + z) < 1 || (x + z) > PCG::MAP_ROWS - 2 || (y + i) < 0 || (y + i) > PCG::MAP_COLUMNS - 2)
				{
					return false;
				}
			}
		}
		return true;
		break;
		//default:
		//	return false;
		//	break;
	}
}

int PCG::CoinFlip(int heads, int tails)
{
	if (GetRandomValue(0, 100) > 50)
	{
		std::cout << "heads";
		return heads;
	}
	else
	{
		std::cout << "tails";
		return tails;
	}
}

void PCG::MakeRoom(TileType _tileArray[PCG::MAP_ROWS][PCG::MAP_COLUMNS], WalkBehaviour _walkBehaviour, int _y, int _x) {
	int offset = _walkBehaviour.ROOM_SIZE / 2;
	for (int i = (-offset); i <= offset; i++) // write room
	{
		for (int z = (-offset); z <= offset; z++)
		{
			_tileArray[_y + i][_x + z] = PCG::TileType::TILE_TYPE_ROOMPATH;
		}
	}
}