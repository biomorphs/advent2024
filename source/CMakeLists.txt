add_library(Utils utils.h utils.cpp)

add_executable(Day1 day1.cpp)
target_link_libraries(Day1 PRIVATE Utils)

add_executable(Day2 day2.cpp)
target_link_libraries(Day2 PRIVATE Utils)

add_executable(Day3 day3.cpp)
target_link_libraries(Day3 PRIVATE Utils)