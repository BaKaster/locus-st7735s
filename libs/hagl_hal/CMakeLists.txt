cmake_minimum_required(VERSION 3.8)

# TODO: add explicit dependencies upon st7735 driver
add_library(hagl_hal INTERFACE)
target_include_directories(hagl_hal INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_sources(hagl INTERFACE ${CMAKE_CURRENT_LIST_DIR}/hagl_hal.cpp)
