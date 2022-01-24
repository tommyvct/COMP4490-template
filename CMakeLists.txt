cmake_minimum_required(VERSION 3.21)
project(COMP4490)

set(CMAKE_CXX_STANDARD 11)

# Dependencies
include_directories("./glm")

if (WIN32)
    add_compile_definitions(FREEGLUT_STATIC)
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
link_libraries(${GLUT_LIBRARY})
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

# Debugging
if (WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT example0)
    set_property(TARGET example0
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
    set_property(TARGET example1
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
    set_property(TARGET example2
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
    set_property(TARGET example3
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
    set_property(TARGET example4
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
    set_property(TARGET example5
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
    set_property(TARGET example6
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
    set_property(TARGET example7
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
    set_property(TARGET example8
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
    set_property(TARGET example9
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")

elseif (APPLE)
    set_property(TARGET example0 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
    set_property(TARGET example1 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
    set_property(TARGET example2 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
    set_property(TARGET example3 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
    set_property(TARGET example4 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
    set_property(TARGET example5 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
    set_property(TARGET example6 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
    set_property(TARGET example7 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
    set_property(TARGET example8 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
    set_property(TARGET example9 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
endif()
