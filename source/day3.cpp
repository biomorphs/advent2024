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

// parse all do/donts into a big sorted list
struct DoOrDont
{
	bool m_enable;
	size_t m_index;	// index into input buffer
};

std::vector<DoOrDont> ParseAllDoDont(std::string_view inputData)
{
	std::vector<DoOrDont> results;
	size_t index = 0;
	const size_t inputLength = inputData.length();
	while (index < inputLength && index != std::string::npos)
	{
		size_t nextDo = inputData.find("do()", index);
		size_t nextDont = inputData.find("don't()", index);
		if (nextDo != std::string::npos)	// there is a do()
		{
			if (nextDont == std::string::npos || nextDo < nextDont) // before a dont(), or no more dont()
			{
				results.push_back({true, nextDo});
				index = nextDo + 1;
				continue;
			}
		}
		if (nextDont != std::string::npos)	// there is a dont, and it is after the next do()
		{
			results.push_back({ false, nextDont });
			index = nextDont + 1;
			continue;
		}
		if (nextDo == std::string::npos && nextDont == std::string::npos)
		{
			break;
		}
		++index;
	}
	return results;
}

void ParseInputForCommands(std::string_view inputData, mulCommand onCmd)
{
	size_t index = 0;
	bool mulEnabled = true;
	const size_t inputLength = inputData.length();

	// find all enable/disable entries
	std::vector<DoOrDont> doOrDontList = ParseAllDoDont(inputData);

	while (index < inputLength && index != std::string::npos)
	{
		size_t nextMul = inputData.find("mul(", index);
		if (nextMul == std::string::npos || nextMul + 4 >= inputLength)	// no more multiplies
		{
			return;
		}

		// pop do or dont off stack until we hit the current index in the data
		while (doOrDontList.size() > 0)
		{
			if (doOrDontList[0].m_index < nextMul)
			{
				mulEnabled = doOrDontList[0].m_enable;
				doOrDontList.erase(doOrDontList.begin());
			}
			else
			{
				break;
			}
		}

		// Parse the mul() arguments
		int32_t arg1 = 0, arg2 = 0;
		size_t commaIndex = ParseInt(inputData, nextMul + 4, ',', arg1);
		if (commaIndex != -1)
		{
			size_t closeParamIndex = ParseInt(inputData, commaIndex + 1, ')', arg2);
			if (closeParamIndex != -1)
			{
				if (mulEnabled)
				{
					onCmd(arg1, arg2);
				}
				index = closeParamIndex + 1;
				continue;
			}
		}
		index = nextMul + 1;
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
