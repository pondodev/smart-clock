cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

include(../pico-sdk/pico_sdk_init.cmake)

project (smart-clock)
set     (CMAKE_BUILD_TYPE Release)
set     (CMAKE_C_STANDARD 17)
set     (PICO_BOARD pico_w) # this project requires a pico w!

add_compile_options(-Wall -Wextra -pedantic)

pico_sdk_init()

file(GLOB PROJECT_SOURCES
     src/*.c
     src/*.h
     src/hardware_platform/*.c
     src/hardware_platform/*.h
     src/hardware_platform/drivers/*.c
     src/hardware_platform/drivers/*.h)

set(PROJECT_INCLUDE_DIRS
     src/)

 set(PROJECT_LIBRARIES
     pico_stdlib
     pico_cyw43_arch_none
     hardware_pwm)

add_executable              (${PROJECT_NAME})
target_sources              (${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories  (${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE_DIRS})
target_link_libraries       (${PROJECT_NAME} PRIVATE ${PROJECT_LIBRARIES})

pico_add_extra_outputs(${PROJECT_NAME})
pico_enable_stdio_uart(${PROJECT_NAME} 1)
pico_enable_stdio_usb(${PROJECT_NAME} 1)

