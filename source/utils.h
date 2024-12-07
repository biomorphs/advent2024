#pragma once 

#include <string>
#include <filesystem>
#include <format>
#include <iostream>
#include <functional>

std::string LoadTextFile(const std::filesystem::path& textPath);

using OneLineFn = std::function<bool(std::string_view)>;	// return false = stop iterating
int32_t ForEachLine(const std::string& s, OneLineFn fn);

template <typename... Args>
inline void Print(std::string_view rt_fmt_str, Args&&... args)
{
	std::string m_msg = std::vformat(rt_fmt_str, std::make_format_args(args...));
	std::cout << m_msg << std::endl;
}

// A nxm grid of chars loaded from text
struct CharGrid {
	std::vector<char> m_data;
	int32_t m_width = 0;
	int32_t m_height = 0;
	int32_t GetIndex(int32_t x, int32_t y) const;
	char& At(int32_t x, int32_t y);
	char At(int32_t x, int32_t y) const;
	void Print();
	bool FindFirst(char toFind, int32_t& x, int32_t& y) const;
	CharGrid() = default;
	CharGrid(const std::string& inputText);
};