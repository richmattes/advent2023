cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
project(advent2023 LANGUAGES CXX)

configure_file(${PROJECT_SOURCE_DIR}/cmake/advent_data.hpp.in
  ${PROJECT_BINARY_DIR}/include/advent2023/advent_data.hpp
  @ONLY)

add_subdirectory(src)
