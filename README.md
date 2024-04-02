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
./main {PATH_TO_ROM} {CPU_CLOCK_RATE}
```

The program requires as input two command line arguments: the absolute or relative path to the ROM and the clock rate (in Hz) at which the emulator should run.

The clock rate is required since different ROMs run better at different clock rates (and there is no exact clock rate specification for the CHIP-8).


## License

This project is licensed under the [MIT License](LICENSE).