#include "screen.h"

#define WINDOW_WIDTH 64
#define WINDOW_HEIGHT 32
#define WINDOW_SCALE 10

void init_screen(SDL_Window **window, SDL_Renderer **renderer) {
		SDL_Init(SDL_INIT_VIDEO);

		SDL_CreateWindowAndRenderer(WINDOW_WIDTH * WINDOW_SCALE,
			WINDOW_HEIGHT * WINDOW_SCALE, 0, window, renderer);
		SDL_RenderSetScale(*renderer, WINDOW_SCALE, WINDOW_SCALE);

		SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
		SDL_RenderClear(*renderer);
}

// Updates the SDL window/renderer based on the contents of the frame buffer.
// This is done by computing the relative x and y coordinates of each byte
// in the buffer, and then computing the exact x coordinate for each bit/pixel
// in that byte. For example, FRAME_BUFFER_START_ADDR should contain pixels
// in the range (0, 0) to (7, 0), and FRAME_BUFFER_END_ADDR should contain
// pixels in the range (56, 31) to (63, 31).
void update_screen(Chip8 *c, SDL_Renderer **renderer) {
	for (int i = FRAME_BUFFER_START_ADDR; i <= FRAME_BUFFER_END_ADDR; i++) {
		int index = i - FRAME_BUFFER_START_ADDR;
		int row = index / 8; // Get the row (y) of the current byte
		int col = index % 8; // Get the col (x) of the current byte

		// For each bit in the current byte, paint the corresponding pixel
		// if is set, else white
		uint8_t byte = c->mem[i];
		for (int j = 0; j < 8; j++) {
			// We can iterate through each bit in the byte by isolating the 
			// most significant bit. Then, to get the next bit, we can left 
			// shift the byte and repeat.
			int pixel = (byte & MS_BIT_8) >> 7;
			byte <<= 1;

			if (pixel) {
				SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
			} else {
				SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
			}

			// The exact column of the bit/pixel can be computed as follows
			int pixel_col = col * 8 + j;
			SDL_RenderDrawPoint(*renderer, pixel_col, row);
		}
	}
	SDL_RenderPresent(*renderer);
}

void close_screen(SDL_Window **window, SDL_Renderer **renderer) {
	SDL_DestroyRenderer(*renderer);
	SDL_DestroyWindow(*window);
	SDL_Quit();
}