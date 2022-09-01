#include <SDL2/SDL.h>
#include <iostream>
#include "display.hpp"

using std::cout;

Display::Display()
{

}

void Display::begin_Display(int x_resolution, int y_resolution)
{
  if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        cout << "Couldn't initialise SDL library\n";
       
    }

    SDL_Window *window = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x_resolution, y_resolution, 0);

    if(!window)
    {
        std::cout << "Couldn't create display.\n";
        
    }

    SDL_Surface *window_surface = SDL_GetWindowSurface(window);

    if(!window_surface)
    {
        std::cout << "Couldn't get the surface from the window\n";
        
    }

    SDL_UpdateWindowSurface(window);
    SDL_Delay(1000);

}