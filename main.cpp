#include <iostream>

#include "chip8.hpp"
#include "display.hpp"


Display my_Display;

int main(int argc, char** argv)
{
    Chip8 my_Chip8{};    
    char const* file_name = argv[1];   

    my_Chip8.clear_all();
    my_Chip8.load_file(file_name); 
    my_Display.begin_Display(X_RESOLUTION, Y_RESOLUTION);  

    bool close_program = false;

    /*while (!close_program)
    {
        

    }*/
    return 0;
}