# chip-8-interpreter

The purpose of this project is to gain a deeper understanding of the CPU, registers, memory, stack and how they interact through implementation of a CHIP-8 interpreter. I hope to use this as a stepping stone to a more complex project of writing a NES emulator.

## What is CHIP-8?

CHIP-8 is an interpreted programming language created in the 1970's made to create games that could be programmed for different computers. CHIP-8 is essentially a virtual machine, so can run on anything as long as it has an interpreter.

## Components

Memory - 4096 bytes (0x000 to 0xFFF)
Registers - 16 6-bit data registers (V0 to VF)
Stack - 12 levels
Delay Timer - For timing events of games. Count down at 60Hz until 0
Sound Timer - Non-zero value produces a sound. Count down at 60Hz until 0
Input -
Display - 64x32 resolution. 
Opcodes - 35 opcodes, 2 bytes long, big-endian.

## Useful resources

https://en.wikipedia.org/wiki/CHIP-8

http://devernay.free.fr/hacks/chip8/C8TECH10.HTM