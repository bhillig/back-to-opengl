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
add_subdirectory(external/stb_image)

set(SOURCES 
	src/Main.cpp
	src/Window.cpp
	src/Renderer/ElementBuffer.cpp
	src/Renderer/Shader.cpp
	src/Renderer/Texture.cpp
	src/Renderer/VertexArray.cpp
	src/Renderer/VertexBuffer.cpp
	src/Renderer/VertexBufferLayout.cpp
	src/Utils/FileUtils.cpp)

set(HEADERS
	include/Window.h
	include/Renderer/ElementBuffer.h
	include/Renderer/Shader.h
	include/Renderer/Texture.h
	include/Renderer/VertexArray.h
	include/Renderer/VertexBuffer.h
	include/Renderer/VertexBufferLayout.h
	include/Utils/FileUtils.h)

add_executable(BackToOpenGL ${SOURCES} ${HEADERS})

target_link_libraries(BackToOpenGL PRIVATE OpenGL::GL glfw glad stb_image)

target_include_directories(BackToOpenGL PRIVATE include)

# Copy resources to build folder
add_custom_command(TARGET BackToOpenGL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/rc"
        "$<TARGET_FILE_DIR:BackToOpenGL>/rc"
)

# Define SHADER_DIR
target_compile_definitions(BackToOpenGL PRIVATE SHADER_DIR="$<TARGET_FILE_DIR:BackToOpenGL>/rc/shaders")
# Define TEXTURE_DIR
target_compile_definitions(BackToOpenGL PRIVATE TEXTURE_DIR="$<TARGET_FILE_DIR:BackToOpenGL>/rc/textures")
