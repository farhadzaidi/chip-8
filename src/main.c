#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#include "chip8.h"
#include "instructions.h"
#include "screen.h"

const int U_DELAY = 1 / INSTR_PER_SEC * 1000000;

int main() {
	Chip8 c;
	init_sys(&c);
	read_rom(&c, "../roms/ibm_logo.ch8");

	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_CreateWindowAndRenderer(WINDOW_WIDTH * WINDOW_SCALE,
		WINDOW_HEIGHT * WINDOW_SCALE, 0, &window, &renderer);
	SDL_RenderSetScale(renderer, WINDOW_SCALE, WINDOW_SCALE);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	c.mem[FRAME_BUFFER_START_ADDR] = 0xFF;
	c.mem[FRAME_BUFFER_START_ADDR + 1] = 0x0F;
	c.mem[FRAME_BUFFER_END_ADDR] = 0b10101010;

	SDL_Event event;
	int is_running = 1;
	while (is_running) {
		while(SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				is_running = 0;
			}
		}

		// uint16_t instr = fetch_instr(&c);
		// decd_and_exec_instr(&c, instr);
		// printf("Executed 0x%04x\n", instr);

		update_screen(&c, renderer);
		// usleep(U_DELAY);
	}

	close_screen(window, renderer);
	return EXIT_SUCCESS;

}