cmake_minimum_required(VERSION 3.23)
# Projeect name and description
project(COMP4490
    DESCRIPTION "COMP4490 OpenGL cross-platform template"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    option(OPENMP "Use OpenMP acceleration" OFF)
endif()

# Use system-wide dynamically-linked version of glew and freeglut
if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(NOT_LINUX ON)
endif()

option(STATIC_GLEW "Use and link with static GLEW" ${NOT_LINUX})
option(STATIC_FREEGLUT "Use and link with static freeGLUT" ${NOT_LINUX})

list(APPEND CMAKE_PREFIX_PATH "./contrib/install")

if(STATIC_GLEW)
    set(GLEW_USE_STATIC_LIBS ON)
endif()

find_package(OpenGL REQUIRED)
find_package(FreeGLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)

if(OPENMP)
    find_package(OpenMP)
    if(NOT OpenMP_CXX_FOUND)
        message(WARNING "OpenMP not found or not supported by the toolchain, skipping OpenMP acceleration support.")
    endif()
    if(MSVC)
        message(WARNING "MSVC only supports OpenMP 2.0, which lacks support for range-based for loop.")
    endif()
endif()



# Targets
add_subdirectory(src)
# Visual Studio startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT example0)
