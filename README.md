# Ancore Operating System (ancoreOS) 

Created by the Aspen Software Foundation, the Ancore Operating System is a new OS that's main goal is to run multiple executable types, such as Microsoft Windows's EXE file type, and Linux's ELF file type, 
therefore fixing the issue of having to dualboot Windows and Linux.

This operating system uses a modular build system made in the GNU Makefile Language, a fast and complex build language made specifically for large projects. More information can be found here: https://www.gnu.org/software/make/

Currently, the Ancore Operating System uses Cuoreterm for its terminal. Cuoreterm is a super lightweight and freestanding terminal made by JerryJhird for CuoreOS. 
Ancore also features RISCV32 program support, made by Dcraftbg. For more information about either one of these projects, visit the links at the bottom of this README.md.
This OS also features the Limine Bootloader, because of its extensive features and ease of use. I personally support its goals and achievements, which is why i've included it in this project.

---

## Building the OS

To build this OS, simply run the command below in the project's root directory:
```
make
```
The required dependencies are as follows:

- Make
- GCC Toolchain
- QEMU-Full
- Clang
- Bear
- Parted

Note: The method of installing these packages may differ by distribution and operating system. Please refer to the documentation for your distribution and/or OS.

---

## Directory Structure:

This is the average root directory for ancoreOS on the user side

  ```
root-dir:
├── A-Documentation
│   └── 01
│       └── Introduction
├── build
│   ├── gdt.o
│   ├── idt.o
│   ├── io.o
│   ├── kernel.elf
│   ├── kernel.o
│   ├── memory.o
│   ├── pmm.o
│   ├── serial.o
│   ├── stdio.o
│   ├── term.o
│   └── uefi.img
├── compile_commands.json
├── limine
│   ├── BOOTAA64.EFI
│   ├── BOOTIA32.EFI
│   ├── BOOTLOONGARCH64.EFI
│   ├── BOOTRISCV64.EFI
│   ├── BOOTX64.EFI
│   ├── LICENSE
│   ├── limine-bios-cd.bin
│   ├── limine-bios-hdd.h
│   ├── limine-bios-pxe.bin
│   ├── limine-bios.sys
│   ├── limine.c
│   ├── limine-uefi-cd.bin
│   └── Makefile
├── linker.ld
├── Makefile
├── ovmf
│   └── OVMF.fd
└── src
    ├── arch
    │   ├── limine.h
    │   └── x86_64
    │       ├── gdt.c
    │       ├── idt.c
    │       └── io.c
    ├── drivers
    │   ├── emul
    │   │   └── rv_vm.h
    │   │       ├── LICENSE.md
    │   │       └── rv_vm.h
    │   ├── klibc
    │   │   ├── memory.c
    │   │   ├── stdio.c
    │   │   └── string.c
    │   ├── memory
    │   │   ├── alloc
    │   │   │   └── slab-alloc.c
    │   │   └── pmm.c
    │   ├── terminal
    │   │   ├── LICENSE
    │   │   └── src
    │   │       ├── cuoreterm.h
    │   │       ├── kfont.h
    │   │       └── term.c
    │   └── util
    │       └── serial.c
    ├── includes
    │   ├── arch
    │   │   └── x86_64
    │   │       ├── gdt.h
    │   │       ├── idt.h
    │   │       └── io.h
    │   ├── klibc
    │   │   ├── memory.h
    │   │   ├── stdint.h
    │   │   ├── stdio.h
    │   │   └── string.h
    │   ├── memory
    │   │   ├── alloc
    │   │   │   └── slab-alloc.h
    │   │   └── pmm.h
    │   └── util
    │       ├── endian.h
    │       ├── serial.h
    │       └── util.h
    ├── kernel
    │   └── kernel.c
    └── limine.conf
```
---

## License

Most of this project is licensed and shipped under GPLv2 as seen in the LICENSE file, but some code snippets/files may be subject to MPLv2 or different licenses. 
Please refer to the file contents and the disclaimer at the top of the file for more information.

---
## External project links
             
CuoreOS: https://codeberg.org/jerryjhird/CuoreOS

Cuoreterm: https://codeberg.org/jerryjhird/Cuoreterm

rv_vm.h: https://codeberg.org/Dcraftbg/rv_vm.h
