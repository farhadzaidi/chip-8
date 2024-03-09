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
	load_rom(&c, "../roms/ibm_logo.ch8");

	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	init_screen(&window, &renderer);

	SDL_Event event;
	int is_running = 1;
	while (is_running) {
		while(SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				is_running = 0;
			}
		}

		uint16_t instr = fetch_instr(&c);
		decd_and_exec_instr(&c, instr);

		update_screen(&c, &renderer);
		usleep(U_DELAY);
	}

	close_screen(&window, &renderer);
	return EXIT_SUCCESS;

}