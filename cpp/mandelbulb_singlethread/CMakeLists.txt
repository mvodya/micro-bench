cmake_minimum_required(VERSION 3.30)
project(mandelbulb_singlethread)

# Setup C++ standart
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Create executable app
add_executable(mandelbulb_singlethread src/main.cpp)

# Linkage microbenchlib
target_link_libraries(mandelbulb_singlethread PRIVATE microbenchlib)

# Set path to includes for microbenchlib
target_include_directories(mandelbulb_singlethread PRIVATE ../microbenchlib/includes)

# mandelbulb_singlethread
add_custom_target(run_mandelbulb_singlethread
    COMMAND mandelbulb_singlethread
    DEPENDS mandelbulb_singlethread
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running mandelbulb_singlethread..."
)