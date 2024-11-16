# Build options
option(MB_BUILD_LIBSTD_STATIC "Build standard library as static for minimize dependencies" ON)

# Optimization
set(MB_OPTIMIZATION_LEVEL "MAX" CACHE STRING "Optimization levels type: OFF - All optimizations is OFF: STANDART - optimization with O3 and link-time optimizer; HIGH - additional advanced optimizations; MAX - optimizations with float pointer calculations (incompatible with IEEE)")
set_property(CACHE MB_OPTIMIZATION_LEVEL PROPERTY STRINGS "OFF" "STANDART" "HIGH" "MAX")
