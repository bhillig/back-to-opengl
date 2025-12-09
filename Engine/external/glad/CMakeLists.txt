cmake_minimum_required(VERSION 3.26)

project(glad)

set(SOURCES 
    src/glad.c)

set(HEADERS 
    include/glad/glad.h 
    include/KHR/khrplatform.h)

add_library(glad ${SOURCES} ${HEADERS})

target_include_directories(glad PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glad
    ${CMAKE_CURRENT_SOURCE_DIR}/include/KHR)
