# set(OSGCITYSCAPE_SRC_DIR "${OSGCITYSPACE_SOURCES_DIR}/src")

function(SETUP_EXAMPLE EXAMPLE_NAME EXAMPLE_SRC)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SRC})
    target_link_libraries(${EXAMPLE_NAME}
        PUBLIC
            osgcityscape_base

    )
    set_target_properties(${EXAMPLE_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}
    )
endfunction()
add_subdirectory(Portal2D)
