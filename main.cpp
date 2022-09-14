/* A chip-8 interpreter by CJW	*/

#include <iostream>
#include <../SDL2/include/SDL2/SDL.h>

#include "chip8.hpp"
#include "display.hpp"

int main(int argc, char** argv)
{
    Chip8 chip8{}; 
    Display_and_input display_and_input;    

    char const* file_name = argv[1];    
    char const* yeet = argv[2];    

    chip8.clear_all();
    display_and_input.begin_display(file_name);
    
    if (!chip8.load_file(file_name))
        {
            SDL_Delay(1000);
            return 0;        
        }
   
    int video_pitch = sizeof(chip8.display_array[0]) * X_RESOLUTION; // the pitch is the length of a row of pixels in bytes    
    
    //cycle will run a single cycle of fetching the instruction from memory at the current program counter, process the instruction, 
    //then execute the instruction and update the display.Constantly checking for keyboard inputs and will close the program if 
    // end_program equates to true, from pressing the escape key.    
    
    bool end_program = false; 
    int count = 0;    

    while(!end_program)
    {      
        end_program = display_and_input.get_key_press(chip8.keyboard_controls); 
        count++;         
        if (count > 10000)
        {               
		    chip8.cycle();           
            display_and_input.update_display(chip8.display_array, video_pitch); 
            display_and_input.get_key_press(chip8.keyboard_controls);  
            count = 0;                           
        }              
        
        //SDL_Delay(2000);  
        //display_and_input.get_key_press(chip8.keyboard_controls);    
    }    
    return 0;
}


//g++ main.cpp chip8.cpp display.cpp -I SDL2/include -L SDL2/lib -lmingw32 -lSDL2main -lSDL2 -o chip8.exe

