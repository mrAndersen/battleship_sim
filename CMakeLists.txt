cmake_minimum_required(VERSION 3.20)
project(battleship_sim)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "src/*.h"
        "src/*.hpp"
        "src/*.cpp"
)


add_executable(battleship_sim ${SOURCES} main.cpp)

target_include_directories(
        battleship_sim
        PUBLIC
        src
)