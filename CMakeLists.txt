cmake_minimum_required(VERSION 3.20)
project(chip-8)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB SOURCES "src/*.c")
add_executable(main ${SOURCES})

target_link_libraries(main ${SDL2_LIBRARIES})