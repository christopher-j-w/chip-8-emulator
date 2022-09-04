
#include <fstream>
#include <iostream>
#include <limits>
#include "chip8.hpp"

const unsigned int PROGRAM_MEMORY_START_ADDRESS = 0x200; //The program gets loaded in to memory starting at this address (int 512). This is primarily for compatibility with older ch8 programs
const unsigned int FONT_MEMORY_START_ADDRESS = 0x50; //Start of the font sprites address
const unsigned int MAX_FILE_SIZE = 3583; //max file size in bytes (4095-512)
const unsigned int FONT_SIZE = 80;
const unsigned int PIXEL_COUNT = 204800;

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


// Initialise memory, registers, stack, timers, display to 0.
void Chip8::clear_all()
{	
	for (int i = 0; i < MEMORY_SIZE; ++i) 
		memory[i] = 0; // clear memory

	for (int i =0; i < REGISTERS_COUNT; ++i)
		V_registers[i] = 0; //clear register	

	delay_timer = 0;
	sound_timer = 0;

	for (int i = 0; i < FONT_SIZE; ++i)
		memory[FONT_MEMORY_START_ADDRESS + i] = font[i]; //load font in to memory starting (0x50).

	program_counter = PROGRAM_MEMORY_START_ADDRESS; //Sets the program counter to starting address (0x200).		

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
			cout << file_name << " has been successfully loaded in to memory.\n";		
			delete[] buffer;		
		}
		else 
			cout << "\n File is too big for CHIP-8s memory.\n";

		file.close();	
	}
}

//Read a two-byte op code from memory and increment the Program Counter by two
uint16_t Chip8::get_Op_Code()
{
	
		cout << "Program counter is at: " << program_counter << '\n';
		op_code = (memory[program_counter & 0xFFF] << 8) + memory[program_counter+1 & 0xFFF];
		cout << "Opcode is " << op_code << '\n';
		program_counter += 2;	
		//interpret_Op_Code();
	
	return memory[program_counter];

}

void Chip8::interpret_Op_Code()
{
	switch(op_code & 0xF000)
	{
		case (0x0000):
			cout << "CLEAR CASE FOUND!";
			Op_Code_00E0();		
			break;
		
		case (0x1000):
			cout << "JUMP CASE FOUND!";
			Op_Code_1nnn();
			break;
		case (0x6000):
			Op_Code_6xkk();
			break;
		case (0x7000):
			Op_Code_7xkk();
			break;
		case (0xA000):
			Op_Code_Annn();	
		case (0xD000):
			Op_Code_Dyxn();	
	}
	
}

void Chip8::Op_Code_00E0() //Clear the display
{
	std::fill_n (display, PIXEL_COUNT, 0);
}

void Chip8::Op_Code_1nnn() //Jump to location nnn (nnn is the 2nd, 3rd and 4th nibble)
{	
	program_counter = op_code & 0x0FFF;
}

void Chip8::Op_Code_6xkk() //Set Vx = kk
{	
	int Vx = (op_code & 0x0F00) >> 8;	
	int kk = op_code & 0x00FF;
	V_registers[Vx] = kk;
}

void Chip8::Op_Code_7xkk() //Set Vx = Vx + kk
{
	int Vx = (op_code & 0x0F00) >> 8;
	int kk = op_code & 0x00FF;
	V_registers[Vx] += kk;
}

void Chip8::Op_Code_Annn() //Set index register = nnn
{
	I_register = op_code & 0x0FFF;
}

void Chip8::Op_Code_Dyxn() ////Display n-byte sprite starting at memory location I(index register) at (Vx, Vy), set VF = collision
{
	int x_coord = V_registers[op_code & 0x00F0];
	int y_coord = V_registers[op_code & 0x0F00];
	V_registers[0xF] = 0;
	int n = op_code & 0x000F;
	I_register;
	
}

