#pragma once

#include <cstdint>

class Chip8 {
    public:  

        uint8_t memory[4096]{};
        uint8_t registers[16]{};
        uint16_t stack[16]{};
        uint8_t delay_timer{};
        uint8_t sound_timer{};
        uint16_t op_code;
        uint32_t display[64*32]{};  
        void load_file(const char * file_name);       


};