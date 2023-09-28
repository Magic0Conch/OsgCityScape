set(OSGCITYSCAPE_SRC_DIR "${CITYSCAPEEDITOR_SOURCES_DIR}/src")
set(TEST_NAME "osgcityscape_base")
file(GLOB_RECURSE TEST_SRC "${OSGCITYSCAPE_SRC_DIR}/*.cpp")
add_library(${TEST_NAME} STATIC ${TEST_SRC})
target_include_directories(
  ${TEST_NAME}
    PUBLIC
    external
        ${OSG_INC_PATH}
        ${CITYSCAPEEDITOR_INC_PATH}
)


target_link_libraries(${TEST_NAME}
    PUBLIC
      imgui
      ${OSG_LIB}
      nativefiledialog
      spdlog
)

install (TARGETS ${TEST_NAME}
    DESTINATION lib)