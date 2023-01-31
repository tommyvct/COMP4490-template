set(TARGET_NAME "example2a")

add_executable(${TARGET_NAME} ../main.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. )
target_sources(${TARGET_NAME} PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/.. 
    FILES ../common.h) # more headers go here
target_sources(${TARGET_NAME} PUBLIC 
    example2a.cpp) # C++ codes go here


LinkDependencies(${TARGET_NAME})
SetIDEWorkingDirectory(${TARGET_NAME})