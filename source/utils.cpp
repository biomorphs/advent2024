#include "utils.h"
#include <fstream>
#include <cassert>

std::string LoadTextFile(const std::filesystem::path& textPath)
{
	std::string resultBuffer;
	std::ifstream fileStream(textPath.string().data(), std::ios::in);
	if (fileStream.is_open())
	{
		fileStream.seekg(0, fileStream.end);
		const size_t fileSize = fileStream.tellg();
		fileStream.seekg(0, fileStream.beg);

		resultBuffer.resize(fileSize);
		fileStream.read(resultBuffer.data(), fileSize);

		size_t actualSize = strlen(resultBuffer.data());
		resultBuffer.resize(actualSize);
	}
	return resultBuffer;
}

int32_t ForEachLine(const std::string& s, OneLineFn fn)
{
	int32_t lineCount = 0;
	std::stringstream allLines(s);
	std::string oneLine;
	while (std::getline(allLines, oneLine))
	{
		++lineCount;
		if (!fn(oneLine))
		{
			break;
		}
	}
	return lineCount;
}

int32_t CharGrid::GetIndex(int32_t x, int32_t y) const
{
	assert(x >= 0 && y >= 0 && x < m_width && y < m_height);
	return x + (y * m_width);
}

char& CharGrid::At(int32_t x, int32_t y)
{
	const auto index = GetIndex(x, y);
	assert(index < m_data.size());
	return m_data[index];
}

char CharGrid::At(int32_t x, int32_t y) const
{
	const auto index = GetIndex(x, y);
	assert(index < m_data.size());
	return m_data[index];
}

CharGrid::CharGrid(const std::string& inputText)
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

bool CharGrid::FindFirst(char toFind, int32_t& rx, int32_t& ry) const
{
	for (int32_t y = 0; y < m_height; ++y)
	{
		for (int32_t x = 0; x < m_width; ++x)
		{
			if (At(x, y) == toFind)
			{
				rx = x;
				ry = y;
				return true;
			}
		}
		std::cout << std::endl;
	}
	return false;
}

void CharGrid::Print()
{
	for (int32_t y = 0; y < m_height; ++y)
	{
		for (int32_t x = 0; x < m_width; ++x)
		{
			std::cout << At(x, y);
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}