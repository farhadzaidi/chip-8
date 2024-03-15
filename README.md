# CHIP-8

## Overview

This project emulates the [CHIP-8](https://en.wikipedia.org/wiki/CHIP-8) interpreter in C. 

From Wikipedia:
>CHIP-8 is an interpreted programming language, developed by Joseph Weisbecker made on his 1802 Microprocessor. It was initially used on the COSMAC VIP and Telmac 1800 8-bit microcomputers in the mid-1970s. CHIP-8 programs are run on a CHIP-8 virtual machine. It was made to allow video games to be more easily programmed for these

## Usage:
**Make sure you have [SDL2](https://www.libsdl.org/) installed on your machine.

```bash
mkdir build
cd build
cmake ..
make
./main
```

The emulator comes with 6 different games (ROMS), but provides the option to load a custom ROM. Note that if you are loading a custom ROM, you must specifiy (in the following prompt) the CPU clock rate as different games run best at different Hz.


## License

This project is licensed under the [MIT License](LICENSE).