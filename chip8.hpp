#pragma once

const unsigned int MEMORY_SIZE = 4096;
const unsigned int REGISTERS_COUNT = 16;
const unsigned int STACK_COUNT = 16;
const unsigned int X_RESOLUTION = 64;
const unsigned int Y_RESOLUTION = 32;
const unsigned int KEY_COUNT = 16;

using std::cout;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t; 

class Chip8 {
    private:           
        u8 memory[MEMORY_SIZE] {};
        u8 V_registers[REGISTERS_COUNT] {};
        u16 index_register {};
        u16 stack[STACK_COUNT] {};
        u8 stack_pointer {};        
        u8 sound_timer {};       
        u16 program_counter {};
        u16 op_code {}; 
        
        void Op_Code_00E0(); // ! Clear the display
        void Op_Code_00EE();
        void Op_Code_1nnn(); // ! Jump to location nnn
        void Op_Code_2nnn(); // ! Call subroutine at nnn
        void Op_Code_3xkk(u8 Vx); // ! Skip next instruction if Vx = kk
        void Op_Code_4xkk(u8 Vx); // ! Skip next instruction if Vx != kk
        void Op_Code_5xy0(u8 Vx, u8 Vy); // ! Skip next instruction if Vx = Vy
        void Op_Code_6xkk(u8 Vx); // ! Set Vx = Vy
        void Op_Code_7xkk(u8 Vx); // ! Set Vx = Vx + kk
        void Op_Code_8xy0(u8 Vx, u8 Vy); // ! Set Vx = Vy
        void Op_Code_8xy1(u8 Vx, u8 Vy); // ! Set Vx = Vx OR Vy
        void Op_Code_8xy2(u8 Vx, u8 Vy); // ! Set Vx = Vx AND Vy
        void Op_Code_8xy3(u8 Vx, u8 Vy); // ! Set Vx = Vx XOR Vy
        void Op_Code_8xy4(u8 Vx, u8 Vy); // ! Set Vx = Vx + Vy, set VF = carry
        void Op_Code_8xy5(u8 Vx, u8 Vy); // ! Set Vx = Vx - Vy, set VF = NOT borrow
        void Op_Code_8xy6(u8 Vx, u8 Vy); // ! Set Vx = Vx SHR 1
        void Op_Code_8xy7(u8 Vx, u8 Vy); // ! Set Vx = Vy - Vx, set VF = NOT borrow
        void Op_Code_8xyE(u8 Vx, u8 Vy); // ! Set Vx = Vx SHL 1
        void Op_Code_9xy0(u8 Vx, u8 Vy); // ! Skip next instruction if Vx != Vy
        void Op_Code_Annn(); // ! Set index register = nnn
        void Op_Code_Bnnn(); // ! Jump to location nnn + V0
        void Op_Code_Cxkk(u8 Vx); // ! Set Vx = random byte and kk
        void Op_Code_Dxyn(u8 Vx, u8 Vy); // ! Display n-byte sprite starting at memory location I(index regiser) at (Vx, Vy), set VF = collision
        void Op_Code_Ex9E(u8 Vx); //Skip next instruction if key with the value of Vx is pressed
        void Op_Code_ExA1(u8 Vx); //Skip next instruction if key with the value of Vx is NOT pressed
        void Op_Code_Fx07(u8 Vx); //Set Vx = delay timer value
        void Op_Code_Fx0A(u8 Vx); //Wait for a key press, store the value of the key in Vx
        void Op_Code_Fx15(u8 Vx); //Set delay timer = Vx
        void Op_Code_Fx18(u8 Vx); //Set sound timer = Vx
        void Op_Code_Fx1E(u8 Vx); //Set Index_register = Index_register + Vx
        void Op_Code_Fx29(u8 Vx); //Set Index_register =location of sprite for digit Vx
        void Op_Code_Fx33(u8 Vx); //Store BCD representation of Vx in memory locations I, I+1, I+2
        void Op_Code_Fx55(u8 Vx); //Store registers V0 through Vx in memory starting at location I
        void Op_Code_Fx65(u8 Vx); //Read registers V0 through Vx from memory starting at location I            
            
    public:        
        Chip8() = default;      
        u32 display_array[X_RESOLUTION*Y_RESOLUTION] {}; 
        u8 keyboard_controls[KEY_COUNT]{};    
        u8 delay_timer {};  
        bool load_file(std::string const& path);     
        void clear_all();   
        void get_Op_Code();    
        void decode_op_code();     
        void cycle();
        
};