cmake_minimum_required(VERSION 3.10)
project(microbench)

# Common library
option(MB_ENABLE_MICROBENCHLIB "Enable shared microbenchlib" ON)
if(MB_ENABLE_MICROBENCHLIB)
    add_subdirectory(microbenchlib)
endif()

# Single thread mandelbulb
option(MB_ENABLE_MANDELBULB_SINGLETHREAD "Enable benchmark with mandelbulb single thread" ON)
if(MB_ENABLE_MANDELBULB_SINGLETHREAD)
    add_subdirectory(mandelbulb_singlethread)
endif()

# Multi thread mandelbulb
option(MB_ENABLE_MANDELBULB_MULTITHREAD "Enable benchmark with mandelbulb milti-thread" ON)
if(MB_ENABLE_MANDELBULB_MULTITHREAD)
    add_subdirectory(mandelbulb_multithread)
endif()

# Render mandelbulb animation
option(MB_ENABLE_MANDELBULB_ANIMATION "Enable project with rendering animation (via CUDA)" OFF)
if(MB_ENABLE_MANDELBULB_ANIMATION)
    add_subdirectory(mandelbulb_animation)
endif()

# Check is CUDA is avaible
include(CheckLanguage)
check_language(CUDA)

if(CMAKE_CUDA_COMPILER)
    message(STATUS "CUDA compiler detected: ${CMAKE_CUDA_COMPILER}")
    set(MB_ENABLE_MANDELBULB_CUDA ON CACHE BOOL "Enable benchmark with mandelbulb CUDA")
else()
    message(STATUS "CUDA compiler not detected.")
    set(MB_ENABLE_MANDELBULB_CUDA OFF CACHE BOOL "Enable benchmark with mandelbulb CUDA")
endif()

# CUDA mandelbulb
if(MB_ENABLE_MANDELBULB_CUDA)
    add_subdirectory(mandelbulb_cuda)
endif()

if(MB_MANDELBULB_RESOLUTION_WIDTH AND MB_MANDELBULB_RESOLUTION_HEIGHT)
    message(STATUS "Mandelbulb resolution: ${MB_MANDELBULB_RESOLUTION_WIDTH} x ${MB_MANDELBULB_RESOLUTION_HEIGHT}")
endif()