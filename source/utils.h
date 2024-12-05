#pragma once 

#include <string>
#include <filesystem>
#include <format>
#include <iostream>

std::string LoadTextFile(const std::filesystem::path& textPath);

template <typename... Args>
inline void Print(std::string_view rt_fmt_str, Args&&... args)
{
	std::string m_msg = std::vformat(rt_fmt_str, std::make_format_args(args...));
	std::cout << m_msg << std::endl;
}