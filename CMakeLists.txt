cmake_minimum_required(VERSION 3.26)

project(BackToOpenGL)

# Set C++23 verison
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate OpenGL
find_package(OpenGL REQUIRED)

include(FetchContent)

FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

add_subdirectory(external/glad)

set(SOURCES 
	src/main.cpp
	src/Window.cpp
	src/Renderer/VertexBuffer.cpp)

set(HEADERS
	include/Window.h
	include/Renderer/VertexBuffer.h)

add_executable(BackToOpenGL ${SOURCES} ${HEADERS})

target_link_libraries(BackToOpenGL PRIVATE OpenGL::GL glfw glad)

target_include_directories(BackToOpenGL PRIVATE include)