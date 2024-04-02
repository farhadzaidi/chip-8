#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "chip8.h"
#include "instructions.h"
#include "screen.h"
#include "sound.h"

int main(int argc, char *argv[]) {
	// The program requires two inputs as command line arguments:
		// 1. The absolute or relative path to the ROM
		// 2. The clock rate (in Hz) at which the emulator should run

	// Note: the clock rate is required to be inputted by the user (as opposed
	// to a fixed value), because the original CHIP-8 specification does not
	// mention a specific clock rate and different games run better at different
	// clock rates.

	if (argc < 3) {
		printf("ERROR: Missing command line arguments.\n");
		return EXIT_FAILURE;
	}

	// The emulator delay (in microseconds) is used to slow down the execution 
	// of the main loop to emulate the processor clock speed.
	const int EMU_DELAY = 1. / atoi(argv[2]) * 1000000;
	
	// This is the number of cycles it should take to decrement the timers by 1.
	const int TIMER_UPDATE_CYCLES = atoi(argv[2]) / 60;

	// Initialize the emulator and load the ROM
	Chip8 c;
	init_sys(&c);
	load_rom(&c, argv[1]);

	// Initialize display and sound system
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	init_screen(&window, &renderer);
	init_sound();

	SDL_Event e;
	SDL_Scancode sc;
	int cycles_since_timer_update = 0;

	// Main loop
	printf("\nRunning emulator... (Press [ESC] to reset)\n");
	while (c.is_running) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				c.is_running = 0;
			} else if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					init_sys(&c);
					load_rom(&c, argv[1]);
					update_screen(&c, &renderer);
				}
			}
		}

		if (c.ST > 0) {
			play_sound();
		} else {
			pause_sound();
		}

		// Get the currently pressed key
		c.key_down = -1;
		const uint8_t *keyboard_state = SDL_GetKeyboardState(NULL);
		for (int sc = 0; sc < SDL_NUM_SCANCODES; sc++) {
			if (keyboard_state[sc]) {
				c.key_down = get_key_from_scancode(sc);
				// If there is a wait period (for a key press), we can set the
				// end wait flag to signal the end of the wait period (since we
				// recieved a key press).
				if (c.start_wait && c.key_down != -1) {
					c.end_wait = 1;
				}
				break;
			}
		}

		// Execute instructions as long as there is no wait period
		// A wait period can occur if the "wait until key press" instruction
		// is executed.
		if (!c.start_wait || c.end_wait) {
			uint16_t instr = fetch_instr(&c);
			decd_and_exec_instr(&c, instr);
		}

		usleep(EMU_DELAY);

		// Decrement timers and update the screen 60 times per second.
		cycles_since_timer_update++;
		if (cycles_since_timer_update == TIMER_UPDATE_CYCLES) {
			if (c.DT > 0) {
				c.DT--;
			}

			if (c.ST > 0) {
				c.ST--;
			}

			// The screen also has a refresh rate of 60 Hz; however, we only
			// update the screen if the update screen flag is set (i.e. a draw
			// instruction was executed).
			if (c.update_screen) {
				update_screen(&c, &renderer);
				c.update_screen = 0;
			}

			cycles_since_timer_update = 0;
		}
	}

	// Clean up
	close_screen(&window, &renderer);
	close_sound();
	SDL_Quit();

	return EXIT_SUCCESS;
}