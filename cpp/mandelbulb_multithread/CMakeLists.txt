cmake_minimum_required(VERSION 3.28)
project(mandelbulb_multithread VERSION 1.0.0 LANGUAGES CXX)

include(../cmake/common_config_options.cmake)
include(../cmake/set_optimization_flags.cmake)
include(../cmake/mandelbulb_options.cmake)

# Setup C++ standart
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Static link for standart libs
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang" AND MB_BUILD_LIBSTD_STATIC)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
endif()

# Enable optimizations for Release build
set_optimization_flags(${MB_OPTIMIZATION_LEVEL})

# Generate config
configure_file(
    ../cmake/mandelbulb_config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/mandelbulb_config.h
    @ONLY
)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

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