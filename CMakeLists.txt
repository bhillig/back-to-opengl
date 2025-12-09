cmake_minimum_required(VERSION 3.26)

project(BackToOpenGL)

# Set C++23 verison
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Engine)
add_subdirectory(App)
