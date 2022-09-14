#include <fstream>
#include <iostream>
#include <limits>
#include <iomanip>

#ifdef __linux__ 
#include <SDL2/SDL.h>  
#endif

#ifdef _WIN32
#include "SDL2\include\SDL2\SDL.h"
#endif

#include "chip8.hpp"

const unsigned int PROGRAM_MEMORY_START_ADDRESS = 0x200; //The program gets loaded in to memory starting at this address (int 512).
const unsigned int FONT_MEMORY_START_ADDRESS = 0x50; //Start of the font sprites address
const unsigned int MAX_FILE_SIZE = 3583; //max file size in bytes (4095-512)
const unsigned int FONT_SIZE = 80; //This is (16*5). 16 input-keys (0x0 to 0xF).
const unsigned int PIXEL_COUNT = 2048; //(64*32. Maybe should not make it a constant.)

u8 font[FONT_SIZE] =
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

/*	Set to zero the following: memory, V_registers, index register, stack, timers (delay and sound).
  	Set the program counter to 0x200 (512). The first 0x200 bits of memory are for backwards compatibility with older roms.
  	Load font in to memory starting at location 0x50 (this seems to be the convention commonly implemented.)	*/	
void Chip8::clear_all()
{
	index_register = 0;
	delay_timer = 0;
	sound_timer = 0;

	program_counter = PROGRAM_MEMORY_START_ADDRESS; //Sets the program counter to starting address (default 0x200).	
		
	for (unsigned int i = 0; i < MEMORY_SIZE; ++i) 	
		memory[i] = 0;  // clear memory	
	
	for (unsigned int i = 0; i < FONT_SIZE; ++i)
		memory[FONT_MEMORY_START_ADDRESS + i] = font[i]; //load font in to memory starting (0x50).
	
	for (unsigned int i =0; i < REGISTERS_COUNT; ++i)
		V_registers[i] = 0; //clear V registers		

	std::srand( ( u8 )std::time( nullptr ) );
		
	cout << "Chip8 has been initialised.\n"; 
}

//Opens a .ch8 (sometimes also .c8) ("rom") file as a binary stream and loads it in to memory if it fits in to CHIP8s memory.
bool Chip8::load_file(std::string const& file_name)
{
	//Open file as stream of binary.
	std::ifstream file;
	file.open(file_name, std::ios::in | std::ios::binary);

	if (file.is_open())
	{
		//Read file to get the file size then reset pointer back to beginning of file
		file.ignore(std::numeric_limits<std::streamsize>::max());
		std::streamsize file_size = file.gcount();
		cout << "The selected file size is: " << file_size << " bytes.\n";
		file.clear();  
		file.seekg( 0, std::ios_base::beg );
		
		//Load file in to buffer if it fits in to memory.
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
			{
				cout << "File is too big for CHIP-8s memory.\n";
				return false;
			}

		file.close();	
		return true;
	}
	else
	{	
		cout << "Could not open file.\n";
		return false;
	}
	
}


/*	A single cycle will get the current op code, increase program counter by two to point to the next op code, 
	interpret and execute the op code, and decrement the delay and sound timers. */
void Chip8::cycle()
{		
	
	get_Op_Code();		
	//cout << "Current Op code to be executed is: " << op_code << '\n';
	program_counter += 2;		
	decode_op_code();

	if (delay_timer > 0)	
		--delay_timer;		

	if (sound_timer > 0)	
		--sound_timer;	
}

/* 	Gets two consecutive bytes starting from the program counter, and joins them together to get an op_code of length two bytes. */
void Chip8::get_Op_Code()
{				
		op_code = (memory[program_counter] << 8) + memory[program_counter+1];
		//cout << "Opcode is " << std::hex << op_code << '\n';					
}

/*	Decodes the op code and then calls the corresponding function.	*/
void Chip8::decode_op_code()
{
	// Calculate Vx and Vy as they are always in the 2nd and 3rd byte positions (0xy0) of an opcode. If the opcode does not require a Vx or Vy value, it will not be used.
	int Vx = (op_code & 0x0F00) >> 8;
	int Vy = (op_code & 0x00F0) >> 4;

	switch(op_code & 0xF000)
	{	 
		case (0x0000):
			switch(op_code)
			{
				case (0x00E0):	
					Op_Code_00E0();					
					break;
				case (0x00EE):
					Op_Code_00EE();
					break;				
			}
			break;
		case (0x1000):			
			Op_Code_1nnn();
			break;			
		case (0x2000):
			Op_Code_2nnn();
			break;	
		case (0x3000):
			Op_Code_3xkk(Vx);
			break;	
		case (0x4000):			
			Op_Code_4xkk(Vx);
			break;	
		case (0x5000):
			Op_Code_5xy0(Vx, Vy);
			break;						
		case (0x6000):			
			Op_Code_6xkk(Vx);
			break;
		case (0x7000):			
			Op_Code_7xkk(Vx);
			break;
		case (0x8000):
			switch(op_code & 0x000F)
			{
				case(0x0000):
					Op_Code_8xy0(Vx, Vy);
					break;
				case(0x0001):
					Op_Code_8xy1(Vx, Vy);
					break;
				case(0x0002):
					Op_Code_8xy2(Vx, Vy);
					break;
				case(0x0003):
					Op_Code_8xy3(Vx, Vy);
					break;
				case(0x0004):
					Op_Code_8xy4(Vx, Vy);
					break;
				case(0x0005):
					Op_Code_8xy5(Vx, Vy);
					break;
				case(0x0006):
					Op_Code_8xy6(Vx, Vy);
					break;
				case(0x0007):
					Op_Code_8xy7(Vx, Vy);
					break;
				case(0x000E):
					Op_Code_8xyE(Vx, Vy);
					break;			
			}
			break;
		case(0x9000):
			Op_Code_9xy0(Vx, Vy);
			break;
		
		case (0xA000):
			Op_Code_Annn();	
			break;

		case (0xB000):
			Op_Code_Bnnn();
			break;

		case(0xC000):
			Op_Code_Cxkk(Vx);
			break;

		case (0xD000):			
			Op_Code_Dxyn(Vx, Vy);	
			break;

		case(0xE000):
			switch(op_code & 0x00FF)
			{	
				case(0x009E):
					Op_Code_Ex9E(Vx);
					break;	

				case(0x00A1):		
					Op_Code_ExA1(Vx);
					break;	
			}
			break;			
		
		case(0xF000):				
			switch (op_code & 0x00FF)
			{
				case (0x0007):
					Op_Code_Fx07(Vx);
					break;

				case (0x000A):
					Op_Code_Fx0A(Vx);
					break;

				case (0x0015):
					Op_Code_Fx15(Vx);
					break;

				case (0x0018):
					Op_Code_Fx18(Vx);
					break;

				case (0x001E):
					Op_Code_Fx1E(Vx);
					break;

				case (0x0029):
					Op_Code_Fx29(Vx);
					break;

				case (0x0033):
					Op_Code_Fx33(Vx);
					break;					
				
				case (0x0055):
					Op_Code_Fx55(Vx);
					break;	
				
				case (0x0065):
					Op_Code_Fx65(Vx);
					break;					
			}	
			break;
		default:
			throw std::runtime_error("Invalid Opcode found");				
	}	
}

/*	Clears the display by setting the display array to all 0.	*/
void Chip8::Op_Code_00E0() 
{	
	std::fill_n (display_array, PIXEL_COUNT, 0);		
}

/*	Return from a subroutine. Sets program counter to the address at top of the stack.	*/
void Chip8::Op_Code_00EE() 
{	
	--stack_pointer;
	program_counter = stack[stack_pointer];
}

/*	Jump to memory location nnn.	*/ 
void Chip8::Op_Code_1nnn() 
{	
	program_counter = (op_code & 0x0FFF);
}

/*	Call subroutine at memory location nnn.	*/
void Chip8::Op_Code_2nnn()
{		
	stack[stack_pointer] = program_counter;
	++stack_pointer;
	program_counter = (op_code & 0x0FFF);
}

/*	Skip next instruction if what is stored in V_Registers[x] is equal to kk.	*/
void Chip8::Op_Code_3xkk(u8 Vx)
{
	if (V_registers[Vx]  == (op_code & 0x00FF))	
		program_counter += 2;	
}

/*	Skip next instruction if what is stored in V_Registers[x] is NOT equal to kk. */
void Chip8::Op_Code_4xkk(u8 Vx)
{
	if (V_registers[Vx] != (op_code & 0x00FF) )	
		program_counter += 2;	
}

/*	Skip next instruction if what is stored in V_Registers[x] is equal to what is stored in V_Registers[y].	*/
void Chip8::Op_Code_5xy0(u8 Vx, u8 Vy)
{
	if (V_registers[Vx] == V_registers[Vy])	
		program_counter += 2;	
}

/*	Set what is in V_register[x] to kk. */
void Chip8::Op_Code_6xkk(u8 Vx) 
{			
	V_registers[Vx] = op_code & 0x00FF;
}

/*	Set what is in V_Registers[x] to V_Registers[x] + kk.	*/
void Chip8::Op_Code_7xkk(u8 Vx) 
{	
	V_registers[Vx] += (op_code & 0x00FF);
}

/*	Set V_Registers[x] to equal V_Registers[y] */
void Chip8::Op_Code_8xy0(u8 Vx, u8 Vy) 
{	
	V_registers[Vx] = V_registers[Vy];
}

/*	Set V_Registers[x] to bitwise V_Registers[x] OR V_Registers[y] 	*/
void Chip8::Op_Code_8xy1(u8 Vx, u8 Vy) 
{	
	V_registers[Vx] |= V_registers[Vy];
}

/*	Set V_Registers[x] to bitwise V_Registers[x] AND V_Registers[y]	*/
void Chip8::Op_Code_8xy2(u8 Vx, u8 Vy) 
{
	V_registers[Vx] &= V_registers[Vy];
}

/*	Set V_Registers[x] to bitwise V_Registers[x] XOR V_Registers[y]	*/
void Chip8::Op_Code_8xy3(u8 Vx, u8 Vy) 
{
	V_registers[Vx] ^= V_registers[Vy];
}

/*	Total is equal to V_registers[x] + V_Reigsters[y].
	Set V_Registers[0xF] to carry IF total is greater than a byte.
	Store the last 8 bits (0x00FF) in V_Registers[x]. 	*/
void Chip8::Op_Code_8xy4(u8 Vx, u8 Vy) 
{		
	u16 total = V_registers[Vx] + V_registers[Vy];

	if (total > 0xFF)	
		V_registers[0xF] = 1;	
	else 	
		V_registers[0xF] = 0;

	V_registers[Vx] = total & 0x00FF;
}

/*	If V_registers[x] > V_registers[y], set V_Registers[0xF] to carry.
	Set V_Registers[x] to V_Registers[x] - V_Registers[y].	 */
void Chip8::Op_Code_8xy5(u8 Vx, u8 Vy) 
{	
	if (V_registers[Vx] > V_registers[Vy])	
		V_registers[0xF] = 1;	
	else	
		V_registers[0xF] = 0;	

	V_registers[Vx] -= V_registers[Vy];
}

/*	If the least-significant bit of V_registers[Vx] is 1, then V_Registers[0xF] is set to 1, otherwise 0. 
	V_registers[Vx] is divided by 2. */
void Chip8::Op_Code_8xy6(u8 Vx, u8 Vy) 
{	
	V_registers[0xF] = (V_registers[Vx] & 0x0001);
	V_registers[Vx] /= 2;
}

/*	If V_registers[Vy] > V_registers[Vx], VF is set to 1, otherwise 0. 
	V_registers[Vx] is set to V_registers[Vy] - V_registers[Vx] */
void Chip8::Op_Code_8xy7(u8 Vx, u8 Vy) 
{	
	if (V_registers[Vy] > V_registers[Vx])	
		V_registers[0xF] = 1;	
	else	
		V_registers[0xF] = 0;	

	V_registers[Vx] = V_registers[Vy] - V_registers[Vx];

}

/* If the most-significant bit of V_registers[Vx] is 1, then V_registers[0xF] is set to 1, otherwise to 0. 
	V_registers[Vx] is multiplied by 2  */
void Chip8::Op_Code_8xyE(u8 Vx, u8 Vy) 
{		
	V_registers[0xF] = (V_registers[Vx] & 0x80) >> 7;
	V_registers[Vx] *= 2;	
}

/*	Skip next instruction if V_registers[Vx] != V_registers[Vy] */
void Chip8::Op_Code_9xy0(u8 Vx, u8 Vy) 
{
	if (V_registers[Vx] != V_registers[Vy])	
		program_counter += 2;	
}

/*	Set index register to nnn	*/
void Chip8::Op_Code_Annn() 
{
	index_register = op_code & 0x0FFF;	
}

/*	Jump to location nnn + V_registers[V0]	*/
void Chip8::Op_Code_Bnnn() 
{
	program_counter = (op_code & 0x0FFF) + V_registers[0];
}

/*	Generate a random number between 0 and 255 which is then bitwise &'d with kk
	Store the outcome of the &'ing in V_registers[Vx]	*/
void Chip8::Op_Code_Cxkk(u8 Vx) 
{
	u8 low_number  = 0;
    u8 high_number = 255;    
	u8 random_number = low_number + std::rand() % ( high_number - low_number );			
	V_registers[Vx] = (op_code & 0x00FF) & random_number;	
	
};

/*	Draws a sprite at coordinates from the V[Vx] and V[Vy] registers.
	The width will always be 8, and the height is taken from the n in opcode.
	Sprites are XORed onto the display. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0.
	If the sprite is goes outside the display range, it wraps around to the opposite side of the screen.
	
	This was the most difficult opcode to implement so high level explanation is: the sprite data begins at location of the index register. 
	It is always 8 bits wide, and the height is gotten from the n part of the opcode. The x and y coords are checked for wrapping by the modulo.
	(e.g if something is to be drawn at 65 on the x axis of the display it will be 65%64, so a value of 1 will flip it to the other side of the screen).
	The y_row variable is the height of the sprite in memory. The x_column then moves along each y_line and gets the value (either on or off, 1 or 0). 
	display_array[(y_coord + y_row) * X_RESOLUTION + (x_coord + x_column)] will get the current pixel of the display.
								*/
void Chip8::Op_Code_Dxyn(u8 Vx, u8 Vy) 
{	
	u8 sprite_height = op_code & 0x000F;
	
	int x_coord = V_registers[Vx] % X_RESOLUTION;
	int y_coord = V_registers[Vy] % Y_RESOLUTION;
	V_registers[0xF] = 0;	

	for (int y_row = 0; y_row < sprite_height; ++y_row)
	{
		int sprite_data = memory[index_register + y_row];
		
		for (int x_column = 0; x_column < 8; ++x_column)
		{						
			if (sprite_data & (0x80 >> x_column))
			{				
				if ( display_array[(y_coord + y_row) * X_RESOLUTION + (x_coord + x_column)]== 0xFFFFFFFF)		
				{
					V_registers[0xF] = 1;	
				}
				display_array[(y_coord + y_row) * X_RESOLUTION + (x_coord + x_column)] ^= 0xFFFFFFFF;
			}				
		}			
	}
}

/*	If the key with the value of V_registers Vx is currently being pressed (down position), increase program counter by 2.	*/
void Chip8::Op_Code_Ex9E(u8 Vx) 
{		
	if (keyboard_controls[V_registers[Vx]])	
		program_counter += 2;

}

/*	If the key with the value of V_registers Vx is NOT currently being pressed (up position), increase program counter by 2.	*/
void Chip8::Op_Code_ExA1(u8 Vx) 
{		
	if (!keyboard_controls[V_registers[Vx]])	
		program_counter += 2;	
}

/* Set V_registers[Vx] to the value of the delay_timer */
void Chip8::Op_Code_Fx07(u8 Vx) 
{
	V_registers[Vx] = delay_timer;
}

/*	Stops all execution until a key is pressed (down position). 
	If no key press is found it will decrement the program counter by 2 to stay at this opcode	*/
void Chip8::Op_Code_Fx0A(u8 Vx) 
{	
	if (keyboard_controls[0])	
		V_registers[Vx] = 0;	
	else if (keyboard_controls[1])	
		V_registers[Vx] = 1;	
	else if (keyboard_controls[2])	
		V_registers[Vx] = 2;	
	else if (keyboard_controls[3])	
		V_registers[Vx] = 3;	
	else if (keyboard_controls[4])	
		V_registers[Vx] = 4;	
	else if (keyboard_controls[5])	
		V_registers[Vx] = 5;	
	else if (keyboard_controls[6])	
		V_registers[Vx] = 6;	
	else if (keyboard_controls[7])	
		V_registers[Vx] = 7;	
	else if (keyboard_controls[8])	
		V_registers[Vx] = 8;	
	else if (keyboard_controls[9])	
		V_registers[Vx] = 9;	
	else if (keyboard_controls[10])	
		V_registers[Vx] = 10;	
	else if (keyboard_controls[11])	
		V_registers[Vx] = 11;	
	else if (keyboard_controls[12])	
		V_registers[Vx] = 12;	
	else if (keyboard_controls[13])	
		V_registers[Vx] = 13;	
	else if (keyboard_controls[14])	
		V_registers[Vx] = 14;	
	else if (keyboard_controls[15])	
		V_registers[Vx] = 15;	
	else	
		program_counter -= 2;
}

/*	Set delay timer to the value store in V_registers[Vx]	*/
void Chip8::Op_Code_Fx15(u8 Vx) 
{	
	delay_timer = V_registers[Vx];
}

/*	Set sound timer to the value store in V_registers[Vx]	*/
void Chip8::Op_Code_Fx18(u8 Vx) 
{	
	sound_timer = V_registers[Vx];
}

/*	Add the value in V_registers[Vx] to the index register.	*/
void Chip8::Op_Code_Fx1E(u8 Vx) 
{	
	index_register += V_registers[Vx];
}

/*	The value in the index_register is set to the location for the sprite corresponding to the value of Vx.  */
void Chip8::Op_Code_Fx29(u8 Vx) 
{	
	index_register = FONT_MEMORY_START_ADDRESS + (5 * V_registers[Vx]);
}

/*	Takes the decimal value of Vx, and places the hundreds digit in memory at location in the index_register,
	the tens digit at location index_register +1, and the ones digit at location index_register+2 */
void Chip8::Op_Code_Fx33(u8 Vx) 
{	
	memory[index_register] = V_registers[Vx] / 100;
	memory[index_register + 1] = (V_registers[Vx]/10) % 10;	
	memory[index_register + 2] = V_registers[Vx] % 10;  
}

/*	Copy the values of index_register through Vx in to memory, starting at the address in the index register	*/
void Chip8::Op_Code_Fx55(u8 Vx) 
{	
	for (u8 i = 0; i <= Vx; ++i)	
		memory[index_register + i] = V_registers[i];
}

/*	Read values from memory starting at location i into registers V0 through Vx	*/
void Chip8::Op_Code_Fx65(u8 Vx) 
{	
	for (u8 i = 0; i <= Vx; ++i)	
		V_registers[i] = memory[index_register + i];	
}
