cmake_minimum_required(VERSION 3.30)
project(mandelbulb_multithread)

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
add_executable(mandelbulb_multithread src/main.cpp)

# Linkage microbenchlib
target_link_libraries(mandelbulb_multithread PRIVATE microbenchlib)

# Set path to includes for microbenchlib
target_include_directories(mandelbulb_multithread PRIVATE ../microbenchlib/includes)

# mandelbulb_multithread
add_custom_target(run_mandelbulb_multithread
    COMMAND mandelbulb_multithread
    DEPENDS mandelbulb_multithread
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running mandelbulb_multithread..."
)