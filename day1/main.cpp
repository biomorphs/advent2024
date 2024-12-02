#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <format>
#include <vector>

using LocationID = int32_t;
using Locations = std::vector<LocationID>;

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

bool ParseLocations(const std::string& inputData, Locations& locs1, Locations& locs2)
{
	auto stream = std::stringstream(inputData);
	LocationID loc1, loc2;
	while ((stream >> loc1) && (stream >> loc2))
	{
		std::cout << std::format("{} {}", loc1, loc2) << std::endl;
		locs1.push_back(loc1);
		locs2.push_back(loc2);
	}

	return true;
}

int main(int argc, char** args)
{
	std::cout << std::format("Day 1!") << std::endl;

	std::filesystem::path inputPath = std::filesystem::absolute("input.txt");
	std::string inputData = LoadTextFile(inputPath);
	if (inputData.length() == 0)
	{
		std::cout << std::format("Failed to open file {}", inputPath.string()) << std::endl;
		return -1;
	}

	Locations locations1, locations2;
	if (!ParseLocations(inputData, locations1, locations2))
	{
		std::cout << "Failed to parse locations" << std::endl;
		return -2;
	}

	// sort lists from smallest to largest ID
	auto sortSmallToLarge = [](const LocationID& loc1, const LocationID& loc2) {
		return loc1 < loc2;
	};
	std::sort(locations1.begin(), locations1.end(), sortSmallToLarge);
	std::sort(locations2.begin(), locations2.end(), sortSmallToLarge);

	// calculate distances between location IDs
	int32_t totalDistance = 0;
	for (int32_t index = 0; index < locations1.size(); ++index)
	{
		const int32_t distance = std::abs(locations1[index] - locations2[index]);
		totalDistance += distance;
	}

	std::cout << std::format("Total distance = {}", totalDistance) << std::endl;

	return 0;
}
