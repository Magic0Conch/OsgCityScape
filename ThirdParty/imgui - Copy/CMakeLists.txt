file(GLOB_RECURSE IMGUI_SRC "${THIRDPARTY_IMGUI_ROOT_DIR}/src/*.cpp")
file(GLOB_RECURSE IMGUI_INC "${THIRDPARTY_IMGUI_ROOT_DIR}/include/*.h")
find_package(OpenGL REQUIRED)
message(${OPENGL_LIBRARIES})
set(LIB_NAME "imgui")
add_library(${LIB_NAME} STATIC ${IMGUI_SRC})
# message(${IMGUI_INC})
target_include_directories(
  ${LIB_NAME}
    PUBLIC        
        ${IMGUI_INC_PATH}
        # ${OPENGL_INCLUDE_DIR}
)

# target_link_libraries(${LIB_NAME}
#     ${OPENGL_LIBRARIES}
# )

install (TARGETS imgui
    DESTINATION lib)