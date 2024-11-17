cmake_minimum_required(VERSION 3.28)
project(mandelbulb_baremetal VERSION 1.0.0 LANGUAGES CXX)

# Include bootloader
add_subdirectory(boot)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Setup compiler
set(CMAKE_CXX_COMPILER i686-elf-g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti")

# Set linker script
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/src/linker_x86.ld")

# Build kernel
add_library(mandelbulb_baremetal_kernel OBJECT src/main.cpp)

# Generate final binary
add_custom_target(mandelbulb_baremetal ALL
    COMMAND ${CMAKE_CXX_COMPILER} -T ${LINKER_SCRIPT} -o mandelbulb_baremetal.bin
            -ffreestanding -O2 -nostdlib
            $<TARGET_OBJECTS:mandelbulb_baremetal_boot>
            $<TARGET_OBJECTS:mandelbulb_baremetal_kernel>
            -lgcc
    DEPENDS mandelbulb_baremetal_boot mandelbulb_baremetal_kernel
)
