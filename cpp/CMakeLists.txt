cmake_minimum_required(VERSION 3.10)
project(microbench)

# Add sub projects
add_subdirectory(microbenchlib)
add_subdirectory(mandelbulb_singlethread)
add_subdirectory(mandelbulb_multithread)
add_subdirectory(mandelbulb_animation)

include(CheckLanguage)
check_language(CUDA)
if(CMAKE_CUDA_COMPILER)
  add_subdirectory(mandelbulb_cuda)
endif()