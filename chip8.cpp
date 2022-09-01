
#include <fstream>
#include <iostream>
#include <limits>

#include "chip8.hpp"

const unsigned int PROGRAM_MEMORY_START_ADDRESS = 0x200; //512
const unsigned int FONT_MEMORY_START_ADDRESS = 0x50;
const unsigned int MAX_FILE_SIZE = 3583; //max file size in bytes (4095-512)
const unsigned int FONT_SIZE = 80;
const unsigned int PIXEL_COUNT = 2048;

using std::cout;

uint8_t font[FONT_SIZE] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

Chip8::Chip8(){

		
}

// Clears memory, registers, stack, timers, display to 0.
void Chip8::initialise()
{	
	for (int i = 0; i < MEMORY_SIZE; ++i) 
		memory[i] = 0; // clear memory

	for (int i =0; i < REGISTERS_COUNT; ++i)
		registers[i] = 0; //clear register	

	delay_timer = 0;
	sound_timer = 0;

	for (int i = 0; i < FONT_SIZE; ++i)
		memory[FONT_MEMORY_START_ADDRESS + i] = font[i]; //load font in to memory starting (0x50).


	pc = PROGRAM_MEMORY_START_ADDRESS; //Sets the program counter to starting address (0x200).		

	cout << "Chip8 has been initialised.\n"; 

}




//Opens a .ch8 ("rom") file and loads it in to memory if it fits in to CHIP8s memory.
void Chip8::load_file(const char * file_name)
{
	//Open file as stream of binary.
	std::ifstream file;
	file.open(file_name, std::ios::in | std::ios::binary);

	if (file.is_open())
	{
		//Read file to get the file size then reset pointer back to beginning of file
		file.ignore(std::numeric_limits<std::streamsize>::max());
		std::streamsize file_size = file.gcount();
		cout << "The file size is: " << file_size << " bytes. \n";
		file.clear();  
		file.seekg( 0, std::ios_base::beg );
		
		//First, load file in to buffer if it fits in to memory.
		if (file_size < MAX_FILE_SIZE)
		{
			char* buffer = new char[file_size];
			file.read(buffer, file_size);	

			// Load the file from the buffer in to memory, starting at (0x200).
			for (int i = 0; i < file_size; ++i)
			{
				memory[i + PROGRAM_MEMORY_START_ADDRESS] = buffer[i];
			}
			cout << file_name << " has been successfully loaded.\n";
		
			delete[] buffer;
		
		}
		else 
			cout << "\n File is too big for CHIP-8s memory.\n";

		file.close();	

	}
}


