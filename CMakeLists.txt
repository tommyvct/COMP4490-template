cmake_minimum_required(VERSION 3.21)
project(COMP4490)

set(CMAKE_CXX_STANDARD 11)
set(WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")

option(OPENMP "Use OpenMP acceleration" ON)

# Dependencies
include_directories("./glm")

if (WIN32)
    add_compile_definitions(FREEGLUT_STATIC)
    add_compile_definitions(FREEGLUT_LIB_PRAGMAS=0)
    set(GLUT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/freeglut/include/")
    set(GLUT_glut_LIBRARY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/freeglut/lib/freeglut_staticd.lib")
    set(GLUT_glut_LIBRARY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/freeglut/lib/freeglut_static.lib")

    add_compile_definitions(GLEW_STATIC)
    set(GLEW_USE_STATIC_LIBS TRUE)
    set(GLEW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glew/include/")
    set(GLEW_STATIC_LIBRARY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/glew/lib/glew32s.lib")
    set(GLEW_STATIC_LIBRARY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/glew/lib/glew32s.lib")
    set(GLEW_SHARED_LIBRARY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/glew/lib/glew32.lib")
    set(GLEW_SHARED_LIBRARY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/glew/lib/glew32.lib")
elseif (APPLE)
    link_libraries("-framework OpenGL")

    add_compile_definitions(GLEW_STATIC)
    set(GLEW_USE_STATIC_LIBS TRUE)
    set(GLEW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glew/include/")
    set(GLEW_STATIC_LIBRARY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/glew/lib/libGLEW.a")
    set(GLEW_STATIC_LIBRARY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/glew/lib/libGLEW.a")

    set(CMAKE_XCODE_GENERATE_SCHEME TRUE)
endif()

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLUT_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})

link_libraries(${OPENGL_opengl_LIBRARY})
link_libraries(${GLUT_LIBRARIES})
link_libraries(${GLEW_LIBRARIES})

# Targets
add_executable(example0 
    src/main.cpp 
    src/example0.cpp)

add_executable(example1 
    src/main.cpp 
    src/example1.cpp)

add_executable(example2 
    src/main.cpp 
    src/example2.cpp)

add_executable(example3 
    src/main.cpp 
    src/example3.cpp)

add_executable(example4 
    src/main.cpp 
    src/example4.cpp)

add_executable(example5 
    src/main.cpp 
    src/example5.cpp)

add_executable(example6 
    src/main.cpp 
    src/example6.cpp)

add_executable(example7 
    src/main.cpp 
    src/example7.cpp)
    
add_executable(example8 
    src/main.cpp 
    src/example8.cpp)

add_executable(example9 
    src/main.cpp 
    src/example9.cpp)


if(OPENMP)
    find_package(OpenMP)

    if(OpenMP_CXX_FOUND)
        target_link_libraries(example0 PUBLIC OpenMP::OpenMP_CXX)
        target_link_libraries(example1 PUBLIC OpenMP::OpenMP_CXX)
        target_link_libraries(example2 PUBLIC OpenMP::OpenMP_CXX)
        target_link_libraries(example3 PUBLIC OpenMP::OpenMP_CXX)
        target_link_libraries(example4 PUBLIC OpenMP::OpenMP_CXX)
        target_link_libraries(example5 PUBLIC OpenMP::OpenMP_CXX)
        target_link_libraries(example6 PUBLIC OpenMP::OpenMP_CXX)
        target_link_libraries(example7 PUBLIC OpenMP::OpenMP_CXX)
        target_link_libraries(example8 PUBLIC OpenMP::OpenMP_CXX)
        target_link_libraries(example9 PUBLIC OpenMP::OpenMP_CXX)
    endif()
endif()

# Debugging
if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT example0)
    set_property(TARGET example0
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY WORKING_DIRECTORY)
    set_property(TARGET example1
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY WORKING_DIRECTORY)
    set_property(TARGET example2
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY WORKING_DIRECTORY)
    set_property(TARGET example3
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY WORKING_DIRECTORY)
    set_property(TARGET example4
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY WORKING_DIRECTORY)
    set_property(TARGET example5
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY WORKING_DIRECTORY)
    set_property(TARGET example6
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY WORKING_DIRECTORY)
    set_property(TARGET example7
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY WORKING_DIRECTORY)
    set_property(TARGET example8
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY WORKING_DIRECTORY)
    set_property(TARGET example9
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY WORKING_DIRECTORY)
elseif (CMAKE_GENERATOR STREQUAL Xcode)
    set_property(TARGET example0 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY WORKING_DIRECTORY)
    set_property(TARGET example1 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY WORKING_DIRECTORY)
    set_property(TARGET example2 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY WORKING_DIRECTORY)
    set_property(TARGET example3 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY WORKING_DIRECTORY)
    set_property(TARGET example4 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY WORKING_DIRECTORY)
    set_property(TARGET example5 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY WORKING_DIRECTORY)
    set_property(TARGET example6 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY WORKING_DIRECTORY)
    set_property(TARGET example7 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY WORKING_DIRECTORY)
    set_property(TARGET example8 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY WORKING_DIRECTORY)
    set_property(TARGET example9 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY WORKING_DIRECTORY)
else()
    message(NOTICE "Please make sure the working directory is set to ${WORKING_DIRECTORY} before debugging and running.")
endif()
