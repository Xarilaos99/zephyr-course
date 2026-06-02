cmake_minimum_required(VERSION 3.20.0)



set(DTC_OVERLAY_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/boards/app.overlay"
)

# This will allow west to find the board named my_board
list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
# list(APPEND DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(my_zephyr_app)


add_subdirectory(drivers/our_drivers)
target_sources(app PRIVATE src/main.cpp)
