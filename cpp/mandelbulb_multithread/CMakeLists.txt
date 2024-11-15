cmake_minimum_required(VERSION 3.30)
project(mandelbulb_multithread VERSION 1.0.0 LANGUAGES CXX)

# Setup C++ standart
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable optimizations for Release build
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -flto")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /arch:AVX2 /GL")
endif()

# Create executable app
add_executable(mandelbulb_multithread src/main.cpp)
# Set static build for MSVC
if(MSVC)
    target_compile_options(mandelbulb_multithread PRIVATE "/MT$<$<CONFIG:Debug>:d>")
endif()
# Link microbenchlib
add_dependencies(mandelbulb_multithread microbenchlib)
target_include_directories(mandelbulb_multithread PRIVATE ../microbenchlib/includes)
target_link_libraries(mandelbulb_multithread PRIVATE microbenchlib)

# mandelbulb_multithread
add_custom_target(run_mandelbulb_multithread
    COMMAND mandelbulb_multithread
    DEPENDS mandelbulb_multithread
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running mandelbulb_multithread..."
)