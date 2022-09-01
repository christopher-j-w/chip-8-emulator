#include <iostream>

#include "chip8.hpp"
#include "display.hpp"

Chip8 my_Chip8;    
Display my_Display;

int main(int argc, char** argv)
{
    char const* file_name = argv[1];   

    my_Chip8.initialise();
    my_Chip8.load_file(file_name); 
    my_Display.begin_Display(X_RESOLUTION, Y_RESOLUTION);  

    return 0;
}