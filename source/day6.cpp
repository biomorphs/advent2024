#include "utils.h"
#include <cassert>
#include <thread>
#include <set>

void GetLookDirection(char guardChar, int32_t& x, int32_t& y)
{
	switch (guardChar)
	{
	case '^':
		x = 0;	y = -1;
		break;
	case '>':
		x = 1; y = 0;
		break;
	case '<':
		x = -1; y = 0;
		break;
	case 'v':
		x = 0; y = 1;
		break;
	default:
		assert("Unknown direction");
	}
}

char GuardChar(int32_t dirX, int32_t dirY)
{
	if (dirX == 0 && dirY == -1)
	{
		return '^';
	}
	else if (dirX == 1 && dirY == 0)
	{
		return '>';
	}
	else if (dirX == 0 && dirY == 1)
	{
		return 'v';
	}
	else if (dirX == -1 && dirY == 0)
	{
		return '<';
	}
	return '@';
}

void RotateRightCW(int32_t& dirX, int32_t& dirY)
{
	if (dirX == 0 && dirY == -1)
	{
		dirX = 1; dirY = 0;
	}
	else if (dirX == 1 && dirY == 0)
	{
		dirX = 0; dirY = 1;
	}
	else if (dirX == 0 && dirY == 1)
	{
		dirX = -1; dirY = 0;
	}
	else if (dirX == -1 && dirY == 0)
	{
		dirX = 0; dirY = -1;
	}
	else
	{
		assert("Unknown direction");
	}
}

int main(int argc, char** args)
{
	Print("Day 6!");

	std::filesystem::path inputPath = std::filesystem::absolute("data/day6/input.txt");
	std::string inputData = LoadTextFile(inputPath);
	if (inputData.length() == 0)
	{
		Print("Failed to open file {}", inputPath.string());
		return -1;
	}

	CharGrid grid(inputData);
	grid.Print();

	int32_t startX = 0, startY = 0;
	if (!grid.FindFirst('^', startX, startY))
	{
		Print("Failed to find start position");
		return -1;
	}
	int32_t guardX = startX, guardY = startY;
	int32_t lookDirX = 0, lookDirY = 0;
	GetLookDirection(grid.At(guardX, guardY), lookDirX, lookDirY);

	using Coord = std::pair<int32_t, int32_t>;
	std::set<Coord> visitedCoords;

	constexpr bool c_showGridProgress = false;
	while (true)
	{
		visitedCoords.insert({ guardX, guardY });

		// move along look dir until we hit an obstacle or we walk out of the grid bounds
		int32_t nextX = guardX + lookDirX;
		int32_t nextY = guardY + lookDirY;
		if (nextX < 0 || nextX >= grid.m_width || nextY < 0 || nextY >= grid.m_height)
		{
			break;	// walked out of the grid
		}

		if (c_showGridProgress)
		{
			auto gridCopy = grid;

			for (const auto& c : visitedCoords)
			{
				gridCopy.At(c.first, c.second) = 'X';
			}
			gridCopy.At(nextX, nextY) = GuardChar(lookDirX, lookDirY);
			gridCopy.Print();
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}

		if (grid.At(nextX, nextY) == '#')	// obstacle
		{
			RotateRightCW(lookDirX, lookDirY);
		}
		else
		{
			guardX = nextX;
			guardY = nextY;
		}
	}

	Print("Visited {} positions", visitedCoords.size());

	return 0;
}
