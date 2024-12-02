cmake_minimum_required(VERSION 3.8)

project(AdventOfCode2024
  VERSION 1.0
  DESCRIPTION "Advent of Code 2024"
  LANGUAGES CXX)
  
set(CMAKE_CXX_STANDARD 20)

if (MSVC)
  add_compile_options(/MP)		# Multi-processor compilation (off by default)
endif ()

add_subdirectory(source)