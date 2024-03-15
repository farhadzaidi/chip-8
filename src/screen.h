#include <SDL2/SDL.h>

#include "chip8.h"

void init_screen(SDL_Window **window, SDL_Renderer **renderer);
void update_screen(Chip8 *c, SDL_Renderer **renderer);
void close_screen(SDL_Window **window, SDL_Renderer **renderer);