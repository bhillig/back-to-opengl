cmake_minimum_required(VERSION 3.26)

project(stb_image)

add_library(stb_image src/stb_image.cpp include/stb_image.h)

target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
