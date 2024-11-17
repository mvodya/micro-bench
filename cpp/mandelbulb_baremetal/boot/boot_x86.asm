; Mandelbulb x86 bootloader

; Declare constants for the multiboot header.
MBALIGN  equ  1 << 0              ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1              ; provide memory map
MBFLAGS  equ  MBALIGN | MEMINFO   ; multiboot flag field
MAGIC    equ  0x1BADB002          ; bootloader magic number
CHECKSUM equ -(MAGIC + MBFLAGS)   ; checksum

; Declare a multiboot header
section .multiboot
align 4
  dd MAGIC
  dd MBFLAGS
  dd CHECKSUM

; Init stack
section .bss
align 16
stack_bottom:
resb 16384    ; 16 KiB
stack_top:

; Entry point
section .text
global _start:function (_start.end - _start)
_start:
  mov esp, stack_top  ; Setup stack

  extern main
	call main    ; Enter the high-level kernel
  ; Kernel done
  cli                 ; Disable interrupts
.hang:	hlt
	jmp .hang
.end: