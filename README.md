# FoxBox

### A bootloader and kernel combination made primarily with x86 in mind, with support planned for x86_64 BIOS/UEFI later on.

*This project is currently in it's very early infancy, and is very, VERY unfinished! Keep in mind, you will likely encounter numerous bugs and incomplete features. In its current state, it is not recommended to run on real hardware, to avoid the chance of accidental damage to files. I put this project on Github early on to make it easier to work with, and to show others my progress over time.*

Submitting issues is encouraged, but please keep the above in mind, and **dont be rude about it!**

## Implemented features

### BIOS bootloader
- [x] Simple memory management
- [ ] Kernel loading
- [ ] Paging support
- [ ] Long mode/x64 support
- [ ] Startup configuration environment

### UEFI bootloader
- [ ] To be determined

### Kernel
- [ ] Also to be determined

## Setup

### Tools required:
> *All tools except i686-elf-tools are available through the Ubuntu package manager.*
- [i686-elf-tools](https://github.com/lordmilko/i686-elf-tools)
- mtools
- dosfstools
- nasm
- make
- qemu-system (optional, for emulation)

### Linux (Ubuntu)
1. Use apt to install the required tools, and obtain i686-elf-tools from the link above.
> *Building i686-elf-tools from source is recommended, though if you must, prebuilt binaries are available!*
2. Once obtained, add the folder containing `i686-elf-*` to PATH in the bottom of `~/.profile`.
    - Example when building from source: `PATH="$HOME/build-i686-elf/linux/output/bin:$PATH"`
3. Clone the project (if you haven't already).
4. Run any of the following commands in the project root:
    - `make`: Builds all files, and creates a disk image.
    - `make run`: Builds all files, creates a disk image, and runs it with qemu. Args are located in the root Makefile.
    - `make clean`: Cleans all build files and images from the project.
    - `make hdd-info`: Gets information about the created disk image. Just a cleaner way to get information about the image and the contents of the root directory.

### Linux (Non-Ubuntu)
Unfortunately, this is left up to you until I try it myself.

### Windows
Your best bet, and likely the best solution, is to use WSL (Windows Subsystem for Linux), as it runs an Ubuntu environment without the need of creating and managing a graphical virtual machine. After following the Linux setup section, simply type `wsl` in the project's root directory, and wait for the environment to start. After that, simply type whatever `make` command you want, and wait for it to build! When using `qemu-system` through WSL on Windows 11, you *should* get a graphical window. I am unsure about WSL on older Windows versions, however.