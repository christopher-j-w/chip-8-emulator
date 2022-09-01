
#include <fstream>
#include <iostream>
#include <limits>

const unsigned int START_ADDRESS = 0x200;
const unsigned int MAX_FILE_SIZE = 3583; //max file size in bytes (4095-512)

using std::cout;

#include "chip8.h"

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

			// Load the file from the buffer in to memory.
			for (int i = 0; i < file_size; ++i)
			{
				memory[START_ADDRESS + i] = buffer[i];
			}
			delete[] buffer;
		
		}
		else 
			cout << "File is too big for CHIP-8s memory." << "\n";

		file.close();	
		

	}
}


