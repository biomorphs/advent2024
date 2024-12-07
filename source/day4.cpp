#include "utils.h"
#include <cassert>

bool FoundWord(std::string_view toFind, const CharGrid& grid,
	int32_t startX, int32_t startY, int32_t xDir, int32_t yDir)
{
	const int32_t findLen = static_cast<int32_t>(toFind.length());
	int32_t x = startX;
	int32_t y = startY;
	int32_t targetX = x + xDir * findLen;
	int32_t targetY = y + yDir * findLen;
	int32_t foundMatching = 0;
	while (foundMatching < findLen)
	{
		if (x < 0 || y < 0 || x >= grid.m_width || y >= grid.m_height)
		{
			return false;
		}
		if (toFind[foundMatching] != grid.At(x, y))
		{
			return false;
		}
		x += xDir;
		y += yDir;
		foundMatching++;
	}
	Print("Found {} at {},{}, dir={},{}", toFind, startX, startY, xDir, yDir);
	return true;
}

constexpr int c_part = 2;

int main(int argc, char** args)
{
	Print("Day 4!");

	std::filesystem::path inputPath = std::filesystem::absolute("data/day4/input.txt");
	std::string inputData = LoadTextFile(inputPath);
	if (inputData.length() == 0)
	{
		Print("Failed to open file {}", inputPath.string());
		return -1;
	}

	CharGrid inputGrid(inputData);
	int32_t foundMatches = 0;

	// part 1
	if constexpr (c_part == 1)
	{
		const auto toFind = "XMAS";
		int32_t foundMatches = 0;
		for (int y = 0; y < inputGrid.m_height; ++y)
		{
			for (int x = 0; x < inputGrid.m_width; ++x)
			{
				// l/r
				foundMatches += FoundWord(toFind, inputGrid, x, y, 1, 0);
				foundMatches += FoundWord(toFind, inputGrid, x, y, -1, 0);
				// u/d
				foundMatches += FoundWord(toFind, inputGrid, x, y, 0, 1);
				foundMatches += FoundWord(toFind, inputGrid, x, y, 0, -1);
				// diagonals
				foundMatches += FoundWord(toFind, inputGrid, x, y, 1, 1);
				foundMatches += FoundWord(toFind, inputGrid, x, y, 1, -1);
				foundMatches += FoundWord(toFind, inputGrid, x, y, -1, 1);
				foundMatches += FoundWord(toFind, inputGrid, x, y, -1, -1);
			}
		}
	}

	// part 2
	if constexpr (c_part == 2)
	{
		for (int y = 0; y < inputGrid.m_height; ++y)
		{
			for (int x = 0; x < inputGrid.m_width; ++x)
			{
				if (inputGrid.At(x, y) == 'A')
				{
					int32_t foundMAS = 0;
					foundMAS += FoundWord("MAS", inputGrid, x - 1, y - 1, 1, 1);
					foundMAS += FoundWord("MAS", inputGrid, x + 1, y - 1, -1, 1);
					foundMAS += FoundWord("MAS", inputGrid, x - 1, y + 1, 1, -1);
					foundMAS += FoundWord("MAS", inputGrid, x + 1, y + 1, -1, -1);
					if (foundMAS >= 2)
					{
						foundMatches++;
					}
				}
			}
		}
	}

	Print("Found {} matches", foundMatches);

	return 0;
}
