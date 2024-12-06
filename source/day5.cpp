#include "utils.h"
#include <cassert>
#include <map>

constexpr int32_t c_part = 2;

enum class SortType
{
	Before,
	After
};

struct SortEntry
{
	int32_t m_value;
	SortType m_type;
};

struct SortRule
{
	std::vector<SortEntry> m_entries;
};

using SortRules = std::map<int32_t, SortRule>;
void AddRule(SortRules& rules, int32_t before, int32_t after)
{
	SortRule& rule1 = rules[before];
	rule1.m_entries.push_back({after, SortType::After});
	SortRule& rule2 = rules[after];
	rule2.m_entries.push_back({ before, SortType::Before });
}

using UpdatePages = std::vector<int32_t>;
using Updates = std::vector<UpdatePages>;

void OutputUpdatePages(const UpdatePages& update)
{
	for (const auto& page : update)
	{
		std::cout << page << " ";
	}
	std::cout << std::endl;
}

bool PageOrderCorrect(const SortRules& allRules, int32_t page1, int32_t page2)
{
	auto rulesFound = allRules.find(page1);	// are there rules for page1?
	if (rulesFound != allRules.end())
	{
		auto& entries = rulesFound->second.m_entries;
		auto foundEntry = std::find_if(entries.begin(), entries.end(), [&](const SortEntry& thisEntry) {
			return thisEntry.m_value == page2;
		});
		if (foundEntry != entries.end())
		{
			if (foundEntry->m_type == SortType::Before)
			{
				return false;
			}
		}
	}
	return true;
}

bool UpdateIsInOrder(const SortRules& allRules, const UpdatePages& update)
{
	for (int32_t index1 = 0; index1 < update.size(); ++index1)
	{
		for (int32_t index2 = 0; index2 < update.size(); ++index2)
		{
			if (index1 < index2 && !PageOrderCorrect(allRules, update[index1], update[index2]))
			{
				return false;
			}
			else if (index1 > index2 && !PageOrderCorrect(allRules, update[index2], update[index1]))
			{
				return false;
			}
		}
	}
	return true;
}

int main(int argc, char** args)
{
	Print("Day 5!");

	std::filesystem::path inputPath = std::filesystem::absolute("data/day5/input.txt");
	std::string inputData = LoadTextFile(inputPath);
	if (inputData.length() == 0)
	{
		Print("Failed to open file {}", inputPath.string());
		return -1;
	}

	// Populate sorting rules
	SortRules allRules;
	Updates allUpdates;
	bool parsingRules = true;
	auto populateData = [&](std::string_view oneLine) {
		if (oneLine.length() == 0)		// stop reading rules after empty line
		{
			parsingRules = false;
			return true;
		}
		std::stringstream parseLine(oneLine.data());
		if (parsingRules)
		{
			int32_t beforeVal = 0, afterVal = 0;
			char separator = 0;
			parseLine >> beforeVal;
			parseLine >> separator;
			parseLine >> afterVal;
			if (separator == '|')
			{
				AddRule(allRules, beforeVal, afterVal);
			}
		}
		else
		{
			UpdatePages pages;
			int32_t nextValue = 0;
			while (parseLine >> nextValue)
			{
				pages.push_back(nextValue);
				char separator = 0;
				parseLine >> separator;
				if (separator != ',')
				{
					break;
				}
			}
			allUpdates.push_back(pages);
		}
		return true;
	};
	ForEachLine(inputData, populateData);

	int32_t middleValueSum = 0;
	if constexpr (c_part == 1)
	{
		// Part 1, sum all updates that match the rules
		for (int updateIndex = 0; updateIndex < allUpdates.size(); ++updateIndex)
		{
			OutputUpdatePages(allUpdates[updateIndex]);
			bool matchesRules = UpdateIsInOrder(allRules, allUpdates[updateIndex]);
			std::cout << (matchesRules ? "\tMatch!" : "\tNo Match!") << std::endl;

			// Find middle page
			if (matchesRules)
			{
				assert((allUpdates[updateIndex].size() % 2) != 0);
				int32_t middleIndex = (static_cast<int32_t>(allUpdates[updateIndex].size()) - 1) / 2;
				middleValueSum += allUpdates[updateIndex][middleIndex];
			}
		}
	}
	else
	{
		// abuse sorting to fulfill constraints
		auto fancySort = [&](const int32_t& p1, const int32_t& p2)
		{
			return p1 == p2 || PageOrderCorrect(allRules, p1, p2);
		};

		// Part 2, re-order badly sorted pages based on rules + sum middle page again
		for (int updateIndex = 0; updateIndex < allUpdates.size(); ++updateIndex)
		{
			bool matchesRules = UpdateIsInOrder(allRules, allUpdates[updateIndex]);
			if (!matchesRules)
			{
				auto fixedPages = allUpdates[updateIndex];
				std::sort(fixedPages.begin(), fixedPages.end(), fancySort);
				assert(UpdateIsInOrder(allRules, fixedPages));	// sanity check!

				assert((fixedPages.size() % 2) != 0);
				int32_t middleIndex = (static_cast<int32_t>(fixedPages.size()) - 1) / 2;
				middleValueSum += fixedPages[middleIndex];
			}
		}
	}

	std::cout << std::format("Middle page sum = {}", middleValueSum) << std::endl;

	return 0;
}
