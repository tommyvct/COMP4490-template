macro(LINKDEPENDENCIES TARGET_NAME)
    target_link_libraries(${TARGET_NAME} PRIVATE OpenGL::GL glm)

    if(STATIC_GLEW)
        target_compile_definitions(${TARGET_NAME} PRIVATE GLEW_STATIC)
        
        target_link_libraries(${TARGET_NAME} PRIVATE GLEW::glew_s)
    else()
        target_link_libraries(${TARGET_NAME} PRIVATE GLEW::glew)
    endif()

    if (STATIC_FREEGLUT)
        target_compile_definitions(${TARGET_NAME} PRIVATE FREEGLUT_STATIC)
        target_compile_definitions(${TARGET_NAME} PRIVATE FREEGLUT_LIB_PRAGMAS=0)
        target_link_libraries(${TARGET_NAME} PRIVATE FreeGLUT::freeglut_static)
    else()
        target_link_libraries(${TARGET_NAME} PRIVATE FreeGLUT::freeglut)
    endif()
endmacro()