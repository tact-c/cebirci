cmake_minimum_required(VERSION 3.28)
project(icbar2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(icbar2 src/cas.c src/parser.c src/adsimp.c)
target_include_directories(icbar2 PRIVATE include)
