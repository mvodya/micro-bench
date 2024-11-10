cmake_minimum_required(VERSION 3.30)
project(microbenchlib VERSION 1.0.0 LANGUAGES CXX)

# Setup C++ standart
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Fix MSVC compiler flags
if(MSVC)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

# Create lib microbenchlib
add_library(microbenchlib src/image.cpp src/vector.cpp)

# Set path to includes
target_include_directories(microbenchlib PUBLIC includes)

# Set props for library
set_target_properties(microbenchlib PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER includes/microbench.h
)

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG b514bdc898e2951020cbdca1304b75f5950d1f59
)
FetchContent_MakeAvailable(googletest)

# Make executable for tests
add_executable(microbench_test tests/image_test.cpp tests/vec3_test.cpp tests/measure_time_test.cpp)

# Link link libs
target_link_libraries(microbench_test PRIVATE microbenchlib gtest gmock gtest_main)

# Set path to includes for tests
target_include_directories(microbench_test PRIVATE includes)

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