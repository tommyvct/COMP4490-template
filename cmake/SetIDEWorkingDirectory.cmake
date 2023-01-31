macro(SETIDEWORKINGDIRECTORY TARGET_NAME)
    if(MSVC)
    set_property(TARGET ${TARGET_NAME}
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    elseif(CMAKE_GENERATOR STREQUAL Xcode)
    set_property(TARGET ${TARGET_NAME} 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY "$(CMAKE_CURRENT_SOURCE_DIR)")

    else()
    message(NOTICE "Please make sure the working directory is set to ${CMAKE_CURRENT_SOURCE_DIR} before debugging and running.")
    endif()
endmacro()
