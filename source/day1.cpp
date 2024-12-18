#include "utils.h"

#include <sstream>
#include <format>
#include <vector>

using LocationID = int32_t;
using Locations = std::vector<LocationID>;

bool ParseLocations(const std::string& inputData, Locations& locs1, Locations& locs2)
{
	auto stream = std::stringstream(inputData);
	LocationID loc1, loc2;
	while ((stream >> loc1) && (stream >> loc2))
	{
		locs1.push_back(loc1);
		locs2.push_back(loc2);
	}

	return true;
}

int main(int argc, char** args)
{
	Print("Day 1!");

	std::filesystem::path inputPath = std::filesystem::absolute("data/day1/input.txt");
	std::string inputData = LoadTextFile(inputPath);
	if (inputData.length() == 0)
	{
		Print("Failed to open file {}", inputPath.string());
		return -1;
	}

	Locations locations1, locations2;
	if (!ParseLocations(inputData, locations1, locations2))
	{
		Print("Failed to parse locations");
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

	Print("Total distance = {}", totalDistance);

	// Part 2, similarity scores!
	int32_t totalSimilarity = 0;
	for (const auto& loc1 : locations1)
	{
		auto foundInList2 = std::find(locations2.begin(), locations2.end(), loc1);
		int32_t foundCount = 0;
		while (foundInList2 != locations2.end() && *foundInList2 == loc1)
		{
			foundCount++;
			foundInList2++;
		}
		const int32_t similarityScore = loc1 * foundCount;
		totalSimilarity += similarityScore;
	}

	Print("Total Similarity = {}", totalSimilarity);

	return 0;
}
