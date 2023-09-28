set(spdlog_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE "${spdlog_SRC_DIR}/include")