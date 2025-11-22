# About

JyrOS is a kernel developped to learn the basics about kernel programming. It is written in C and nasm and developped for an i386 architecture.

It follows the 42 school Kernel From Scratch series.

> Notice that the work is still in progress.

# Features

- boot via Grub
- 8 different terminals
  - 80 x 25 chararcters VGA text mode ([code page 437](https://en.wikipedia.org/wiki/Code_page_437))
  - PS/2 keyboard handling ([scan code set 1](https://wiki.osdev.org/PS/2_Keyboard#Scan_Code_Set_1)) (qwerty and azerty layout)
  - scrolling
  - cursor management
- Global Descriptor Table
  - flat segmentation model
- Paging and Virtual Memory
  - page tables (two levels)
  - separated user and kernel space (higher half kernel)
  - virtual and physical allocation
  - buddy allocator
  - malloc and free
- kernel Panic fault handling
- interrupts handling (hardware and software)
- clock handling

# Build from sources

## Requirements

- Docker
- QEMU
- GDB

## Compiling

- Compiling the kernel requires a cross-compiler for the i386 architecture. Our cross-compiler is in a docker that will be downloaded automatically, unless you prefer to build it locally with: `make build_docker`.
- The kernel is build with the command: `make`.
- To run the kernel on QEMU:
  - using the iso (start with Grub): `make start_iso`
  - or the binary (skip Grub): `make start_bin`

## Debug

- For debugging, you can start a gdb session.
- GDB will run in a separate window.
- A default breakpoint is set on kernel_main.
- `make debug`

# terminal

Up to 8 terminals are available.

## terminal shortcuts

- C^ left: previous terminal
- C^ right: next terminal
- C^ [: switch foregroud color
- C^ ]: switch background color

## terminal builtins

- echo
  - `echo <oneword>`
  - print the word given as argument
- azerty
  - switch to an AZERTY keyboard
- qwerty
  - switch to a QWERTY keyboard
- interrupt
  - `interrupt <number>`
  - call the interrupt given as argument
  - see the interrupts section
- bootinfos
  - print infos about boot process
- registers
  - print registers state
- readdump
  - print the stack memory dumped after a Kernel Panic Reboot
- hexdump
  - print the stack
- uptime
  - print time since last boot
- testStackDump
  - test the stack dump mechanism
  - will reboot the computer
- halt
  - turn off the kernel
- reboot
  - reboot the kernel

# Interrupts

- Int 0: Divide Error
- Int 1: Debug Exception
- Int 2: Non Maskable Interrupt
- Int 3: Breakpoint
- Int 4: Overflow
- Int 5: Bounds Check
- Int 6: Invalid Opcode
- Int 7: Coprocessor Not Available
- Int 8: Double Fault (error code)
- Int 9: Coprocessor Segment Overrun
- Int 10: Invalid TSS (error code)
- Int 11: Segment Not Present (error code)
- Int 12: Stack Exception (error code)
- Int 13: General Protection Fault (error code)
- Int 14: Page Fault
- Int 15: reserved
- Int 16: Coprocessor Error
- Int 21: marked as NOT PRESENT. Calling it will trigger a Segement Not Present (10) error
- Int 32: Timer
- Int 33: Keyboard

# Tests

- test builtins:
  - `testStackDump`
- test functions:
  - srcs/interrupts/interrupts_test.c:
    - zero_divide_test()
    - debug_exception_test()
    - nmi_test()
    - breakpoint_test()
    - overflow_test()
    - bound_test()
    - invalid_opcode_test()
    - coprocessor_not_available_test()
    - double_fault_test()
    - copro_segment_test()
    - invalid_tss_test()
    - segment_not_present_test()
    - stack_exception_test()
    - general_protection_test()
    - coprocessor_error_test()
  - srcs/memory/mbook.c:
    - mbook_test()
    - memory_test_vmbook()
  - srcs/memory/memory.c:
    - memory_test()
  - srcs/memory/paging.c:
    - page_testing()
