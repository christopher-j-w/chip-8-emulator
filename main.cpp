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
    my_Chip8.get_Op_Code();

    bool end_program = false;


    //The loop structure is to fetch the instruction from memory at the program counter, process the instruction, then execute the instruction.
    /*while (!end_program) 
    {
        

    }*/
    return 0;
}


//g++ main.cpp chip8.cpp display.cpp -I SDL2/include -L SDL2/lib -lmingw32 -lSDL2main -lSDL2 -o file2.exe

