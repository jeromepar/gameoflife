cmake_minimum_required(VERSION 3.10.0)
project(gameoflife VERSION 0.1.0 LANGUAGES C CXX)

find_package(SDL2 REQUIRED)

include_directories(gameoflife ${SDL2_INCLUDE_DIRS})

add_executable(gameoflife main.cpp)

target_link_libraries(gameoflife ${SDL2_LIBRARIES})