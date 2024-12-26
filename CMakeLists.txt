# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(Template_Project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

include_directories($ENV{NANOPB_PATH})

# Add Nanopb source files as a library
add_library(nanopb STATIC
    $ENV{NANOPB_PATH}/pb_common.c
    $ENV{NANOPB_PATH}/pb_encode.c
    $ENV{NANOPB_PATH}/pb_decode.c
)

# Add the executable
add_executable(${PROJECT_NAME} main.c uart.pb.c)

# Link libraries to the project
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    nanopb
)

# Pico-specific configuration
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
pico_add_extra_outputs(${PROJECT_NAME})
