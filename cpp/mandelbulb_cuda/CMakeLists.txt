cmake_minimum_required(VERSION 3.28)
project(mandelbulb_cuda VERSION 1.0.0 LANGUAGES CXX CUDA)

# Setup C++ standart
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Setup CUDA
set(CMAKE_CUDA_ARCHITECTURES 52)

# Static link for standart libs
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
endif()

# Enable optimizations for Release build
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -flto")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mtune=native -funroll-loops -mavx2 -mfma -funsafe-math-optimizations -fno-math-errno -fomit-frame-pointer -ffast-math -fipa-pta -fgraphite-identity -floop-nest-optimize -fno-rtti")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /arch:AVX2 /GL")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /fp:fast /favor:INTEL64 /Ot /Ob2 /Oi /Oy /GF /Gy /Qpar /GS- /Zc:inline /Zc:strictStrings /Zc:threadSafeInit")
endif()

# Create executable app
add_executable(mandelbulb_cuda src/main.cu)
# Set static build for MSVC & CUDA
if(MSVC)
    set(CMAKE_CUDA_FLAGS_DEBUG "${CMAKE_CUDA_FLAGS_DEBUG} -Xcompiler=/MTd")
    set(CMAKE_CUDA_FLAGS_RELEASE "${CMAKE_CUDA_FLAGS_RELEASE} -Xcompiler=/MT")
endif()
# Link microbenchlib
add_dependencies(mandelbulb_cuda microbenchlib)
target_include_directories(mandelbulb_cuda PRIVATE ../microbenchlib/includes)
target_link_libraries(mandelbulb_cuda PRIVATE microbenchlib)
# Setup CUDA params
set_target_properties(mandelbulb_cuda PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
    CUDA_RUNTIME_LIBRARY Static
    CUDA_STANDARD 17
)

# mandelbulb_cuda
add_custom_target(run_mandelbulb_cuda
    COMMAND mandelbulb_cuda
    DEPENDS mandelbulb_cuda
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running mandelbulb_cuda..."
)