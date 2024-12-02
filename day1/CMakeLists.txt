add_executable(Day1 main.cpp)

# Setup default working directory
set(TARGET Day1 PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}../data/day1")