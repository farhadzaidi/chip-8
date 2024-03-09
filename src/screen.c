#include "screen.h"

#define WINDOW_WIDTH 64
#define WINDOW_HEIGHT 32
#define WINDOW_SCALE 10

void init_screen(SDL_Window *window, SDL_Renderer *renderer) {
		SDL_Init(SDL_INIT_VIDEO);
		SDL_CreateWindowAndRenderer(WINDOW_WIDTH * WINDOW_SCALE,
			WINDOW_HEIGHT * WINDOW_SCALE, 0, &window, &renderer);
}

void update_screen(Chip8 *c, SDL_Renderer *renderer) {
	for (int i = FRAME_BUFFER_START_ADDR; i <= FRAME_BUFFER_END_ADDR; i++) {
		int index = i - FRAME_BUFFER_START_ADDR;
		int row = index / 8; // byte/bit row
		int col = index % 8; // byte column

		uint8_t byte = c->mem[i];
		for (int j = 0; j < 8; j++) {
			int pixel = (byte & 0x80) >> 7;
			if (pixel) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			} else {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			}

			int pixel_col = col * 8 + j; // bit column
			SDL_RenderDrawPoint(renderer, pixel_col, row);
			byte <<= 1;
		}
	}
	SDL_RenderPresent(renderer);
}

void close_screen(SDL_Window *window, SDL_Renderer *renderer) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}