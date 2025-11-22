cmake_minimum_required(VERSION 3.26)

project(BackToOpenGL)

# Set C++23 verison
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate OpenGL
find_package(OpenGL REQUIRED)

include(FetchContent)

# ----------------------------------------------------------
# GLFW
# ----------------------------------------------------------
FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

# ----------------------------------------------------------
# Glad
# ----------------------------------------------------------
add_subdirectory(external/glad)

# ----------------------------------------------------------
# stb_image
# ----------------------------------------------------------
add_subdirectory(external/stb_image)

# ----------------------------------------------------------
# Dear ImGui
# ----------------------------------------------------------
add_library(imgui
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC external/imgui external/imgui/backends)
target_link_libraries(imgui PUBLIC OpenGL::GL glfw)

# ----------------------------------------------------------
# assimp - Open-Asset-Importer-Library
# ----------------------------------------------------------
add_subdirectory(external/assimp)

set(SOURCES 
	src/Application.cpp
	src/Main.cpp
	src/Renderer/ElementBuffer.cpp
	src/Renderer/Mesh.cpp
	src/Renderer/Model.cpp
	src/Renderer/Shader.cpp
	src/Renderer/Texture.cpp
	src/Renderer/VertexArray.cpp
	src/Renderer/VertexBuffer.cpp
	src/Renderer/VertexBufferLayout.cpp
	src/Scene/Camera.cpp
	src/Scene/CameraController.cpp
	src/Scene/Scene.cpp
	src/Scene/Scenes/CubeScene.cpp
	src/Scene/Scenes/LightingDemoScene.cpp
	src/Scene/Scenes/ModelScene.cpp
	src/Utils/FileUtils.cpp)

set(HEADERS
	include/Application.h
	include/Core/Event.h
	include/Core/EventDispatcher.h
	include/Renderer/ElementBuffer.h
	include/Renderer/Mesh.h
	include/Renderer/Model.h
	include/Renderer/Shader.h
	include/Renderer/Texture.h
	include/Renderer/VertexArray.h
	include/Renderer/VertexBuffer.h
	include/Renderer/VertexBufferLayout.h
	include/Scene/Camera.h
	include/Scene/CameraController.h
	include/Scene/Scene.h
	include/Scene/Scenes/CubeScene.h
	include/Scene/Scenes/LightingDemoScene.h
	include/Scene/Scenes/ModelScene.h
	include/Utils/FileUtils.h)

add_executable(BackToOpenGL ${SOURCES} ${HEADERS})

target_link_libraries(BackToOpenGL PRIVATE OpenGL::GL glfw glad stb_image imgui assimp)

target_include_directories(BackToOpenGL PRIVATE include)

# ----------------------------------------------------------
# GLM
# ----------------------------------------------------------
target_include_directories(BackToOpenGL PRIVATE external/glm/include)

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
# Define MODEL_DIR
target_compile_definitions(BackToOpenGL PRIVATE MODEL_DIR="$<TARGET_FILE_DIR:BackToOpenGL>/rc/models")
