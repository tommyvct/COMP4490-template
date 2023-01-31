cmake_minimum_required(VERSION 3.21)
project(COMP4490
    DESCRIPTION "COMP4490 OpenGL cross-platform template"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

option(OPENMP "Use OpenMP acceleration" ON)
option(STATIC_GLEW "Use and link with static GLEW" OFF)
option(STATIC_FREEGLUT "Use and link with static freeGLUT" OFF)

# Dependencies
# Don't do this as this applies globally for anything on and below this level 
# GLM is a header-only library, so we can let this slide
# This will be problematic as soon you have this library have another dependencies 
# include_directories("./glm")


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
endif()



# Targets
add_subdirectory(src)
# Visual Studio startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT example0)

