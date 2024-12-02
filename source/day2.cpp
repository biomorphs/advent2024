#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <format>
#include <vector>

using Level = int32_t;
using Report = std::vector<Level>;

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

Report ParseReport(const std::string& oneLine)
{
	Report newReport;
	auto stream = std::stringstream(oneLine);
	Level nextLevel;
	while (stream >> nextLevel)
	{
		newReport.push_back(nextLevel);
	}
	return newReport;
}

bool ReportIsSafe(const Report& r)
{
	if (r.size() > 1)
	{
		bool direction = r[1] > r[0];
		for (int32_t index = 0; index < r.size() - 1; ++index)
		{
			const bool thisDir = r[index + 1] > r[index];
			const int32_t thisDifference = std::abs(r[index + 1] - r[index]);
			const bool isValid = (thisDir == direction) && (thisDifference >= 1 && thisDifference <= 3);
			if (!isValid)
			{
				return false;
			}
		}
	}
	return true;
}

void OutputReport(const Report& r)
{
	for (const auto& it : r)
	{
		std::cout << it << " ";
	}
	std::cout << std::endl;
}

int main(int argc, char** args)
{
	std::cout << std::format("Day 2!") << std::endl;

	std::filesystem::path inputPath = std::filesystem::absolute("data/day2/input.txt");
	std::string inputData = LoadTextFile(inputPath);
	if (inputData.length() == 0)
	{
		std::cout << std::format("Failed to open file {}", inputPath.string()) << std::endl;
		return -1;
	}

	std::stringstream allLines(inputData);
	std::string oneLine;
	int32_t totalSafeReports = 0;
	while (std::getline(allLines, oneLine))
	{
		Report nextReport = ParseReport(oneLine);
		bool isSafe = ReportIsSafe(nextReport);
		std::cout << (isSafe ? "Safe: " : "Unsafe: ");
		OutputReport(nextReport);
		totalSafeReports += isSafe;
	}
	std::cout << std::format("{} Safe Reports", totalSafeReports) << std::endl;
	return 0;
}
