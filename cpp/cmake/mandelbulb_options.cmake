set(MB_MANDELBULB_RESOLUTION_WIDTH "2048" CACHE STRING "Resolution width (x) option (e.g., 512, 1024, 2048)")
set_property(CACHE MB_MANDELBULB_RESOLUTION_WIDTH PROPERTY STRINGS "64" "128" "512" "1024" "2048" "4096")

set(MB_MANDELBULB_RESOLUTION_HEIGHT "2048" CACHE STRING "Resolution height (y) (e.g., 512, 1024, 2048)")
set_property(CACHE MB_MANDELBULB_RESOLUTION_HEIGHT PROPERTY STRINGS "64" "128" "512" "1024" "2048" "4096")

set(MB_MANDELBULB_MAX_MARCHING_STEPS "100" CACHE STRING "Maximum number of steps for ray marching")
set_property(CACHE MB_MANDELBULB_MAX_MARCHING_STEPS PROPERTY STRINGS "10" "20" "50" "100" "200" "500" "1000")

set(MB_MANDELBULB_MAX_DISTANCE "100.0" CACHE STRING "Maximum marching distance before stopping")
set_property(CACHE MB_MANDELBULB_MAX_DISTANCE PROPERTY STRINGS "10.0" "20.0" "50.0" "100.0" "200.0" "500.0" "1000.0")

set(MB_MANDELBULB_SURFACE_DISTANCE "0.001" CACHE STRING "Minimum distance threshold to consider on surface")
set_property(CACHE MB_MANDELBULB_SURFACE_DISTANCE PROPERTY STRINGS "0.1" "0.01" "0.001" "0.0001" "0.00001" "0.000001")

set(MB_MANDELBULB_MAX_ITERATIONS "8" CACHE STRING "Maximum iterations for distance estimator function")
set_property(CACHE MB_MANDELBULB_MAX_ITERATIONS PROPERTY STRINGS "1" "2" "4" "6" "8" "12" "20" "40" "80")

set(MB_MANDELBULB_BAILOUT "2.0" CACHE STRING "Escape radius for distance estimator")
set_property(CACHE MB_MANDELBULB_BAILOUT PROPERTY STRINGS "1.1" "1.2" "1.5" "1.8" "2.0" "2.1" "2.5" "3.0")

set(MB_MANDELBULB_POWER "8.0" CACHE STRING "Power exponent for Mandelbulb fractal shape")
set_property(CACHE MB_MANDELBULB_POWER PROPERTY STRINGS "2.0" "3.0" "4.0" "5.0" "6.0" "7.0" "8.0" "9.0" "10.0")

set(MB_RENDER_NUM "10" CACHE STRING "Number of renders to be performed (identical renders for quality check)")
set_property(CACHE MB_RENDER_NUM PROPERTY STRINGS "1" "2" "5" "10" "15" "20")