#include "utils.h"
#include <sstream>
#include <vector>

using Level = int32_t;
using Report = std::vector<Level>;

Report ParseReport(std::string_view oneLine)
{
	Report newReport;
	auto stream = std::stringstream(oneLine.data());
	Level nextLevel;
	while (stream >> nextLevel)
	{
		newReport.push_back(nextLevel);
	}
	return newReport;
}

void OutputReport(const Report& r)
{
	std::cout << "{ ";
	for (const auto& it : r)
	{
		std::cout << it << " ";
	}
	std::cout << "}";
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

bool ReportIsSafeWithProblemDamper(const Report& r)
{
	Report reportCopy;
	for (int32_t index = 0; index < r.size(); ++index)
	{
		reportCopy = r;
		reportCopy.erase(reportCopy.begin() + index);
		if (ReportIsSafe(reportCopy))
		{
			OutputReport(r);
			Print(" is safe with index {} ({}) removed", index, r[index]);
			return true;
		}
	}
	return false;
}

int main(int argc, char** args)
{
	Print("Day 2!");

	std::filesystem::path inputPath = std::filesystem::absolute("data/day2/input.txt");
	std::string inputData = LoadTextFile(inputPath);
	if (inputData.length() == 0)
	{
		Print("Failed to open file {}", inputPath.string());
		return -1;
	}

	int32_t totalSafeReports = 0;
	int32_t totalSafeWithDamper = 0;
	auto forEachLine = [&](std::string_view oneLine)
	{
		Report nextReport = ParseReport(oneLine);
		bool isSafe = ReportIsSafe(nextReport);
		totalSafeReports += isSafe;
		if (!isSafe)
		{
			totalSafeWithDamper += ReportIsSafeWithProblemDamper(nextReport);
		}
		return true;
	};
	ForEachLine(inputData, forEachLine);
	Print("{} Safe Reports, {} with damper", totalSafeReports, totalSafeReports + totalSafeWithDamper);

	return 0;
}
