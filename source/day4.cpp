#include "utils.h"
#include <cassert>

struct WordGrid {
	std::vector<char> m_data;
	int32_t m_width = 0;
	int32_t m_height = 0;
	int32_t GetIndex(int32_t x, int32_t y) const
	{
		assert(x >= 0 && y >= 0 && x < m_width && y < m_height);
		return x + (y * m_width);
	}
	char& At(int32_t x, int32_t y)
	{
		const auto index = GetIndex(x, y);
		assert(index < m_data.size());
		return m_data[index];
	}
	char At(int32_t x, int32_t y) const
	{
		const auto index = GetIndex(x, y);
		assert(index < m_data.size());
		return m_data[index];
	}
	WordGrid() = default;
	WordGrid(const std::string& inputText)
	{
		auto countDimensions = [&](std::string_view oneLine)
		{
			m_width = static_cast<int32_t>(oneLine.length());
			return true;	// count all lines
		};
		m_height = ForEachLine(inputText, countDimensions);
		m_data.resize(m_width * m_height);
		
		int32_t y = 0;
		auto populate = [&](std::string_view oneLine)
		{
			const auto lineLen = oneLine.length();
			for (int32_t x = 0; x < m_width; ++x)
			{
				assert(x < lineLen);
				At(x, y) = oneLine[x];
			}
			++y;
			return true;
		};
		ForEachLine(inputText, populate);
	}
};

bool FoundWord(std::string_view toFind, const WordGrid& grid,
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

	WordGrid inputGrid(inputData);
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
