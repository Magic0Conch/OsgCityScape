set(OSGCITYSCAPE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(TEST_DIR "${OSGCITYSCAPE_SOURCE_DIR}/test")
set(TEST_NAME "osgcityscape-test")
file(GLOB_RECURSE TEST_SRC "${TEST_DIR}/*.cpp")

find_package(OpenGL REQUIRED)
message("11:")
message("${OSG_LIB}")
add_executable(${TEST_NAME} ${TEST_SRC})
target_include_directories(
  ${TEST_NAME}
    PUBLIC
    external
        ${OSG_INC_PATH}
        # ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(${TEST_NAME}
    PUBLIC
        ${OSG_LIB}
)

set_target_properties(${TEST_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}
)


