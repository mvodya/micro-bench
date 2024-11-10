cmake_minimum_required(VERSION 3.10)
project(microbench)

# Add sub projects
add_subdirectory(microbenchlib)
add_subdirectory(mandelbulb_singlethread)
add_subdirectory(mandelbulb_multithread)
