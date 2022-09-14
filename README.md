# chip-8-interpreter

The purpose of this project is to gain a deeper understanding of how emulators function including through implementation of a CHIP-8 interpreter. I hope to use this as a stepping stone to a more complex project of writing a gameboy emulator.

## What is CHIP-8?

CHIP-8 is an interpreted programming language created in the 1970's made to create games that could be programmed for different computers. CHIP-8 is essentially a virtual machine, so can run on anything if there is an interpreter for that system.

## Components

- Memory - 4096 bytes (0x000 to 0xFFF)
- Registers - 16 8-bit data registers (V0 to VF). 
- Stack - 16 levels
- Program Counter (PC) - Holds the address of the next instruction to execute.
- Delay Timer - For timing events of games. Count down at 60Hz until 0
- Sound Timer - Non-zero value produces a sound. Count down at 60Hz until 0
- Input - 16 keys (0x0 to 0xF)
- Display - 64x32 resolution. Monochrome.
- Opcodes - 35 opcodes, 2 bytes long, big-endian.

## Requirements

SDL2 

# Build and run instructions

On linux
    -   To get SDL2:     sudo apt-get install libsdl2-dev
    -   compile using:   g++ main.cpp chip8.cpp display.cpp -o chip8 -lSDL2

To run from terminal:
    ./chip8 ./roms/ibm.ch8

On windows
    - To get SDL2:      https://github.com/libsdl-org/SDL/releases/tag/release-2.24.0   
    - TO FILL IN
    
To run from terminal:
    - Using minGW
    - compile using:    g++ main.cpp chip8.cpp display.cpp -I SDL2/include -L SDL2/lib -lmingw32 -lSDL2main -lSDL2 -o chip8.exe

To run from terminal:
    chip8 roms/<romname>.ch8

## TO-DO

Implement sound
Alter timing cycle and make it user definable
Add example gifs
Add roms to repository

# Credits

Roms taken from https://github.com/dmatlack/chip8

## Useful resources

https://en.wikipedia.org/wiki/CHIP-8

http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

https://wiki.libsdl.org/