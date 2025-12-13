project(App)

set(SOURCES 
    src/Main.cpp
    src/GameSceneLayer.cpp
    src/GUILayer.cpp
    src/Scene/Scene.cpp
	src/Scene/Scenes/CubeScene.cpp
	src/Scene/Scenes/LightingDemoScene.cpp
	src/Scene/Scenes/ModelScene.cpp)

set(HEADERS
    include/GameSceneLayer.h
    include/GUILayer.h
	include/Scene/Scene.h
	include/Scene/Scenes/CubeScene.h
	include/Scene/Scenes/LightingDemoScene.h
	include/Scene/Scenes/ModelScene.h)

add_executable(App ${SOURCES} ${HEADERS})

target_link_libraries(App PRIVATE Engine)

target_include_directories(App PRIVATE include)

# Sync Visual Studio file directories to what's on disk
set(ALL_FILES ${SOURCES} ${HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${ALL_FILES})

# Define SHADER_DIR
target_compile_definitions(App PRIVATE SHADER_DIR="$<TARGET_FILE_DIR:App>/rc/shaders")
# Define TEXTURE_DIR
target_compile_definitions(App PRIVATE TEXTURE_DIR="$<TARGET_FILE_DIR:App>/rc/textures")
# Define MODEL_DIR
target_compile_definitions(App PRIVATE MODEL_DIR="$<TARGET_FILE_DIR:App>/rc/models")

# Copy resources to build folder
add_custom_command(TARGET App POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/rc"
        "$<TARGET_FILE_DIR:App>/rc"
)
