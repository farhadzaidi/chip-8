#include <SDL2/SDL.h>

#include "chip8.h"

#define WINDOW_WIDTH 64
#define WINDOW_HEIGHT 32
#define WINDOW_SCALE 10

void init_screen(SDL_Window *window, SDL_Renderer *renderer);
void update_screen(Chip8 *c, SDL_Renderer *renderer);
void close_screen(SDL_Window *window, SDL_Renderer *renderer);