function(SETUP_EXAMPLE EXAMPLE_NAME EXAMPLE_DIR)
    file(GLOB_RECURSE EXAMPLE_SRC "${EXAMPLE_DIR}/*.cpp")
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SRC})
    target_link_libraries(${EXAMPLE_NAME}
        PRIVATE
            osgcityscape_base

    )
    set_target_properties(${EXAMPLE_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}
    )
endfunction()
add_subdirectory(Portal2D)
add_subdirectory(HighlightArea)
add_subdirectory(RoadFX)
add_subdirectory(osgsimplegl3)
add_subdirectory(osgpick)
add_subdirectory(osgoutline)