#include <iostream>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include "SDL2\include\SDL2\SDL.h"
#endif

#ifdef __linux__ 
#include <SDL2/SDL.h>  
#endif

#include "display.hpp"

void Display_and_input::begin_display(char const* file_title)
{
	char window_title[256];	
	//strcpy(window_title, "CHIP8 Interpreter -  ");
	//strcat(window_title, file_title);

	window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 25*X_RESOLUTION, 25*Y_RESOLUTION, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, X_RESOLUTION,Y_RESOLUTION);

  	if(SDL_Init(SDL_INIT_VIDEO) != 0)    
        cout << "Couldn't initialise SDL library\n";       
    if(!window)    
        cout << "Couldn't create display.\n"; 
 
    SDL_UpdateWindowSurface(window);       
}


void Display_and_input::update_display(void const* pixels, int pitch)
{
	
    SDL_RenderClear(renderer);	
	SDL_UpdateTexture(texture, nullptr, pixels, pitch);		      
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);	
	SDL_RenderPresent(renderer);
}

bool Display_and_input::get_key_press(u8* keys)
{    
    SDL_Event event;
    while (SDL_PollEvent (&event))
    {
        switch (event.type)
			{
				case SDL_QUIT:				                    
					quit = true;
				    break;
				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:						
                            cout << "Terminating chip8 program.\n";
							quit = true;
						    break;
						case SDLK_x:						
							keys[0] = 1;                            
						    break;
						case SDLK_1:						
							keys[1] = 1;                           
						    break;
						case SDLK_2:	                            				
							keys[2] = 1;
						    break;
						case SDLK_3:						
							keys[3] = 1;
						    break;
						case SDLK_q:	
                            keys[4] = 1;
						    break;
						case SDLK_w:						
							keys[5] = 1;
						    break;
						case SDLK_e:						
							keys[6] = 1;
						    break;
						case SDLK_a:						
							keys[7] = 1;
						    break;
						case SDLK_s:						
							keys[8] = 1;
						    break;
						case SDLK_d:						
							keys[9] = 1;
						    break;
						case SDLK_z:						
							keys[10] = 1;
						    break;
						case SDLK_c:						
							keys[11] = 1;
						    break;
						case SDLK_4:						
							keys[12] = 1;
						    break;
						case SDLK_r:						
							keys[13] = 1;
						    break;
						case SDLK_f:						
							keys[14] = 1;
						    break;
						case SDLK_v:						
							keys[15] = 1;
						    break;
					}
				} break;

				case SDL_KEYUP:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_x:						
							keys[0] = 0;                            
						    break;
						case SDLK_1:						
							keys[1] = 0;                           
						    break;
						case SDLK_2:	                            				
							keys[2] = 0;
						    break;
						case SDLK_3:						
							keys[3] = 0;
						    break;
						case SDLK_q:	
                            keys[4] = 0;
						    break;
						case SDLK_w:						
							keys[5] = 0;
						    break;
						case SDLK_e:						
							keys[6] = 0;
						    break;
						case SDLK_a:						
							keys[7] = 0;
						    break;
						case SDLK_s:						
							keys[8] = 0;
						    break;
						case SDLK_d:						
							keys[9] = 0;
						    break;
						case SDLK_z:						
							keys[10] = 0;
						    break;
						case SDLK_c:						
							keys[11] = 0;
						    break;
						case SDLK_4:						
							keys[12] = 0;
						    break;
						case SDLK_r:						
							keys[13] = 0;
						    break;
						case SDLK_f:						
							keys[14] = 0;
						    break;
						case SDLK_v:						
							keys[15] = 0;
						    break;
					}
				}   break;
			}
		}        
    return quit;		
}




