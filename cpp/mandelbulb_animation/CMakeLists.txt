cmake_minimum_required(VERSION 3.30)
project(mandelbulb_animation)

# Setup C++ standart
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable optimizations for Release build
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -flto")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /arch:AVX2 /GL")
endif()

# Fix MSVC compiler flags
if(MSVC)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

# Create executable app
add_executable(mandelbulb_animation src/main.cpp)

# Linkage microbenchlib
target_link_libraries(mandelbulb_animation PRIVATE microbenchlib)

# Set path to includes for microbenchlib
target_include_directories(mandelbulb_animation PRIVATE ../microbenchlib/includes)

# mandelbulb_animation
add_custom_target(run_mandelbulb_animation
    COMMAND mandelbulb_animation
    DEPENDS mandelbulb_animation
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running mandelbulb_animation..."
)