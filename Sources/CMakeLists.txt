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
find_package(OpenGL REQUIRED)
message(STATUS "OpenGL found: ${OPENGL_FOUND} " ${OPENGL_LIBRARIES})
# find_package(PNG REQUIRED)
target_link_libraries(${TEST_NAME}
    PUBLIC
      imgui
      ${OSG_LIB}
      nativefiledialog
      spdlog
      json11
      glm
      ${OPENGL_LIBRARIES}
      # PNG::PNG
)

install (TARGETS ${TEST_NAME}
    DESTINATION lib)


add_executable(osgcityscape main.cpp)
target_link_libraries(osgcityscape
PRIVATE
osgcityscape_base

)
set_target_properties(osgcityscape PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}
)



add_executable(osgcityscape main.cpp)
target_link_libraries(osgcityscape
PRIVATE
osgcityscape_base

)
set_target_properties(osgcityscape PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}
)
