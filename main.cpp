#include "chip8.h"
#include <iostream>

int main(int argc, char** argv)
{
    char const* file_name = argv[1];
    
    Chip8 chip8;    
    chip8.load_file(file_name);
    return 0;
}