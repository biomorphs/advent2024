#include "utils.h"
#include <functional>

using mulCommand = std::function<void(int32_t,int32_t)>;

// return -1 if failed to parse, or the offset to the next limiter
size_t ParseInt(std::string_view inputData, size_t offset, char limiter, int32_t & result)
{
	for (size_t index = offset; index < inputData.length(); ++index)
	{
		if (inputData[index] == limiter && index > offset)	// hit limiter with valid input
		{
			result = std::stoi(inputData.data() + offset);
			return index;
		}
		if (!std::isdigit(inputData[index]))
		{
			return -1;
		}
	}
	return false;
}

void ParseInputForCommands(std::string_view inputData, mulCommand onCmd)
{
	size_t index = 0;
	const size_t inputLength = inputData.length();
	while (index < inputLength && index != std::string::npos)
	{
		index = inputData.find("mul(", index);
		if (index == std::string::npos || index + 4 >= inputLength)	// not found, or not enough space for x,x)
		{
			return;
		}
		int32_t arg1 = 0, arg2 = 0;
		size_t commaIndex = ParseInt(inputData, index + 4, ',', arg1);
		if (commaIndex != -1)
		{
			size_t closeParamIndex = ParseInt(inputData, commaIndex + 1, ')', arg2);
			if (closeParamIndex != -1)
			{
				onCmd(arg1, arg2);
				index = closeParamIndex + 1;
				continue;
			}
		}
		++index;
	}
}

int main(int argc, char** args)
{
	Print("Day 3!");

	std::filesystem::path inputPath = std::filesystem::absolute("data/day3/input.txt");
	std::string inputData = LoadTextFile(inputPath);
	if (inputData.length() == 0)
	{
		Print("Failed to open file {}", inputPath.string());
		return -1;
	}

	int64_t totalResult = 0;
	auto foundCmd = [&totalResult](int32_t a1, int32_t a2) {
		Print("mul({},{})", a1, a2);
		totalResult += a1 * static_cast<int64_t>(a2);
	};
	ParseInputForCommands(inputData, foundCmd);

	Print("Total result = {}", totalResult);

	return 0;
}
