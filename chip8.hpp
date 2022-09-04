#pragma once

const unsigned int MEMORY_SIZE = 4096;
const unsigned int REGISTERS_COUNT = 16;
const unsigned int STACK_COUNT = 16;
const unsigned int X_RESOLUTION = 640;
const unsigned int Y_RESOLUTION = 320;

class Chip8 {
    private:           
        uint8_t memory[MEMORY_SIZE] {};
        uint8_t V_registers[REGISTERS_COUNT] {};
        uint16_t I_register {};
        uint16_t stack[STACK_COUNT] {};
        uint8_t stack_pointer {};
        uint8_t delay_timer {};
        uint8_t sound_timer {};
        uint16_t program_counter {};
        uint16_t op_code {};
        uint32_t display[64*32] {};  
        
        void Op_Code_1nnn(); //Jump to location nnn
        void Op_Code_00E0(); //Clear the display
        void Op_Code_2nnn(); //Call subroutine at nnn
        void Op_Code_3xkk(); //Skip next instruction if Vx = kk
        void Op_Code_4xkk(); //Skip next instruction if Vx != kk
        void Op_Code_5xy0(); //Skip next instruction if Vx = Vy
        void Op_Code_6xkk(); //Set Vx = Vy
        void Op_Code_7xkk(); //Set Vx = Vx + kk
        void Op_Code_8xy0(); //Set Vx = Vy
        void Op_Code_8xy1(); //Set Vx = Vx OR Vy
        void Op_Code_8xy2(); //Set Vx = Vx AND Vy
        void Op_Code_8xy3(); //Set Vx = Vx XOR Vy
        void Op_Code_8xy4(); //Set Vx = Vx + Vy, set VF = carry
        void Op_Code_8xy5(); //Set Vx = Vx - Vy, set VF = NOT borrow
        void Op_Code_8xy6(); //Set Vx = Vx SHR 1
        void Op_Code_8xy7(); //Set Vx = Vy - Vx, set VF = NOT borrow
        void Op_Code_8xyE(); //Set Vx = Vx SHL 1
        void Op_Code_9xy0(); //Skip next instruction if Vx != Vy
        void Op_Code_Annn(); //Set index register = nnn
        void Op_Code_Bnnn(); //Jump to location nnn + V0
        void Op_Code_Cxkk(); //Set Vx = random byte AND kk
        void Op_Code_Dyxn(); //Display n-byte sprite starting at memory location I(index regiser) at (Vx, Vy), set VF = collision
        void Op_Code_Ex9E(); //Skip next instruction if key with the value of Vx is pressed
        void Op_Code_ExA1(); //Skip next instruction if key with the value of Vx is NOT pressed
        void Op_Code_Fx07(); //Set Vx = delay timer value
        void Op_Code_Fx0A(); //Wait for a key press, store the value of the key in Vx
        void Op_Code_Fx15(); //Set delay timer = Vx
        void Op_Code_Fx18(); //Set sound timer = Vx
        void Op_Code_Fx1E(); //Set Index_register = Index_register + Vx
        void Op_Code_Fx29(); //Set Index_register =location of sprite for digit Vx
        void Op_Code_Fx33(); //Store BCD representation of Vx in memory locations I, I+1, I+2
        void Op_Code_Fx55(); //Store registers V0 through Vx in memory starting at location I
        void Op_Code_Fx65(); //Read registers V0 through Vx from memory starting at location I        
        
    public:
        
        Chip8() = default;        
        void load_file(const char * file_name);     
        void clear_all();   
        uint16_t get_Op_Code();    
        void interpret_Op_Code();

};