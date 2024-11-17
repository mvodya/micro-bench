cmake_minimum_required(VERSION 3.28)
project(mandelbulb_baremetal_boot VERSION 1.0.0 LANGUAGES ASM_NASM)

# Set ELF32 format for NASM
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> -felf32 -o <OBJECT> <SOURCE>")

# Build bootloader
add_library(mandelbulb_baremetal_boot OBJECT boot_x86.asm)
