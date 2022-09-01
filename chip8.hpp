#pragma once

#include <cstdint>

const unsigned int MEMORY_SIZE = 4096;
const unsigned int REGISTERS_COUNT = 16;
const unsigned int STACK_COUNT = 16;
const unsigned int X_RESOLUTION = 640;
const unsigned int Y_RESOLUTION = 320;



class Chip8 {
    public:   
        Chip8(); 
        uint8_t memory[MEMORY_SIZE] {};
        uint8_t registers[REGISTERS_COUNT] {};
        uint16_t stack[STACK_COUNT] {};
        uint8_t delay_timer {};
        uint8_t sound_timer {};
        uint16_t op_code {};
        uint32_t display[64*32] {};  
        uint16_t pc {};

        void load_file(const char * file_name);     
        void initialise();

};