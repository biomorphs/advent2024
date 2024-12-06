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