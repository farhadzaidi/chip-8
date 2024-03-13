#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#include "chip8.h"
#include "instructions.h"
#include "screen.h"

#define CPU_CLOCK_SPEED 540 // cycles per second (hz)
#define TIMER_UPDATE_CYCLES 9 // decrement timers every 9 cycles

int main() {
	// Emulator delay in microseconds as a function of cpu clock speed.
	// This is important since the computers that ran the CHIP-8 interpreter
	// were significantly slower than modern computers, and so we need to slow
	// down the execution of instructions to match their speed.
	const int EMU_DELAY = 1. / CPU_CLOCK_SPEED * 1000000;
	const int DEBUG = 1;

	// Initialize system and load ROM
	Chip8 c;
	init_sys(&c);
	load_rom(&c, "../roms/ibm-logo.ch8");

	// Initialize display
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	init_screen(&window, &renderer);

	SDL_Event e;
	SDL_Scancode sc;
	int cycles_since_timer_update = 0;

	// Main loop
	while (c.is_running) {
		// Event handler
		while(SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				c.is_running = 0;
			} else if (e.type == SDL_KEYDOWN) {
				sc = e.key.keysym.scancode;
				c.key_down = get_key_from_scancode(sc);
				if (c.start_wait && c.key_down != -1) {
					c.end_wait = 1;
				}
			}
		}

		// Execute instructions, and update the display as long as there is no 
		// wait period.

		uint16_t instr = 0;
		if (!c.start_wait || c.end_wait) {
			instr = fetch_instr(&c);
			decd_and_exec_instr(&c, instr);
			update_screen(&c, &renderer);
		}

		// Add delay and decrement timers if applicable.
		usleep(EMU_DELAY);
		cycles_since_timer_update++;
		if (cycles_since_timer_update == TIMER_UPDATE_CYCLES) {
			if (c.DT > 0) {
				c.DT--;
			}

			if (c.ST > 0) {
				c.ST--;
			}

			cycles_since_timer_update = 0;
		}

		if (DEBUG && instr) {
			printf("Executed 0x%03x\n", instr);
		}
	}

	close_screen(&window, &renderer);
	return EXIT_SUCCESS;
}