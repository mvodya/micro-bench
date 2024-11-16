cmake_minimum_required(VERSION 3.28)
project(microbenchlib VERSION 1.0.0 LANGUAGES CXX)

include(../cmake/common_config_options.cmake)
include(../cmake/set_optimization_flags.cmake)

# Setup C++ standart
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Static link for standart libs
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang" AND MB_BUILD_LIBSTD_STATIC)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
endif()

# Enable optimizations for Release build
set_optimization_flags(${MB_OPTIMIZATION_LEVEL})

# Create lib microbenchlib
add_library(microbenchlib STATIC src/image.cpp src/vector.cpp)
# Set static build for MSVC
if(MSVC)
    target_compile_options(microbenchlib PRIVATE "/MT$<$<CONFIG:Debug>:d>")
endif()
# Set path to includes
target_include_directories(microbenchlib PUBLIC includes)
# Set props for library
set_target_properties(microbenchlib PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER includes/microbench.h
)

# Google Test
set(gtest_force_shared_crt OFF CACHE BOOL "" FORCE)
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG b514bdc898e2951020cbdca1304b75f5950d1f59
)
FetchContent_MakeAvailable(googletest)

# Make executable for tests
add_executable(microbench_test tests/image_test.cpp tests/vec3_test.cpp tests/measure_time_test.cpp)
# Set static build for MSVC
if(MSVC)
    target_compile_options(microbench_test PRIVATE "/MT$<$<CONFIG:Debug>:d>")
endif()
# Link libs for tests
add_dependencies(microbench_test gtest)
target_include_directories(microbench_test PRIVATE includes)
target_link_libraries(microbench_test PRIVATE gtest gmock gtest_main microbenchlib)

# Enable tests
enable_testing()
add_test(NAME ImageTest COMMAND microbench_test)
add_test(NAME Vec3Test COMMAND microbench_test)
add_test(NAME MeasureTimeTest COMMAND microbench_test)

# microbenchlib tests
add_custom_target(run_microbenchlib_tests
    COMMAND ctest -C Debug --output-on-failure
    DEPENDS microbench_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running tests..."
)