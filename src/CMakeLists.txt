set(OSGCITYSCAPE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(TEST_NAME "osgcityscape-test")
file(GLOB_RECURSE TEST_SRC "${OSGCITYSCAPE_SRC_DIR}/*.cpp")
find_package(OpenGL REQUIRED)
add_executable(${TEST_NAME} ${TEST_SRC})
target_include_directories(
  ${TEST_NAME}
    PUBLIC
    external
        ${OSG_INC_PATH}
        ${OSGCITYSPACE_INC_PATH}
)

target_link_libraries(${TEST_NAME}
    PUBLIC
        ${OSG_LIB}
)



set_target_properties(${TEST_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}
)


