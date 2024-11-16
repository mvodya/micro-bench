function(set_optimization_flags optimization_level)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        # STANDART, HIGH, MAX
        if(${optimization_level} STREQUAL "STANDART"
        OR ${optimization_level} STREQUAL "HIGH"
        OR ${optimization_level} STREQUAL "MAX")
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -flto")
        endif()
        # HIGH, MAX
        if(${optimization_level} STREQUAL "HIGH"
        OR ${optimization_level} STREQUAL "MAX")
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mtune=native -mavx2 -mfma -funroll-loops -fno-math-errno -fomit-frame-pointer -fipa-pta -fgraphite-identity -floop-nest-optimize -fno-rtti")
        endif()
        # MAX only
        if(${optimization_level} STREQUAL "MAX")
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -funsafe-math-optimizations -ffast-math")
        endif()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        # STANDART, HIGH, MAX
        if(${optimization_level} STREQUAL "STANDART"
        OR ${optimization_level} STREQUAL "HIGH"
        OR ${optimization_level} STREQUAL "MAX")
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /GL /arch:AVX2 /Ot /Ob2 /Oi /Oy")
        endif()

        # HIGH, MAX
        if(${optimization_level} STREQUAL "HIGH"
        OR ${optimization_level} STREQUAL "MAX")
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /favor:INTEL64 /Qpar /GS- /Zc:inline")
        endif()

        # MAX only
        if(${optimization_level} STREQUAL "MAX")
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /fp:fast /fastmath /Qopt-report:5 /Qopt-report-phase:vec /Qopt-report-file:stdout")
        endif()
    endif()
    message(STATUS "${PROJECT_NAME} release C++ flags: ${CMAKE_CXX_FLAGS_RELEASE}")
endfunction()