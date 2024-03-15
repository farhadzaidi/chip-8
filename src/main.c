#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "chip8.h"
#include "instructions.h"
#include "screen.h"
#include "sound.h"

int main() {
	// The user can pick from one of 6 included ROMS or load a custom ROM.
	printf("\n------ CHIP-8 EMULATOR ------\n\n");
	printf("Select a game from the following (1-6) or load a custom ROM (7):\n");
	printf("1. Breakout\n");
	printf("2. Connect 4\n");
	printf("3. Pong\n");
	printf("4. Space Invaders\n");
	printf("5. Tetris\n");
	printf("6. Tic Tac Toe\n");
	printf("7. Custom ROM\n\n");

	int rom;
	printf("Selection: ");
	scanf("%d", &rom);

	// Initialize system
	Chip8 c;
	init_sys(&c);

	// Load ROM based on user selection.
	// The chip-8 specification doesn't list the speed at which the processor
	// should run, and since different games run best at different speeds, the
	// speed of the processor will be set differently based on the selected game.
	// Alternatively, if the user chooses to select a custom ROM, then they
	// must set the processor speed.
	char rom_path[500];
	int cpu_clock_rate;
	switch(rom) {
		case 1:
			strcpy(rom_path, "../roms/breakout.ch8");
			cpu_clock_rate = 720;
			break;
		case 2:
			strcpy(rom_path, "../roms/connect-4.ch8");
			cpu_clock_rate = 120;
			break;
		case 3:
			strcpy(rom_path, "../roms/pong.ch8");
			cpu_clock_rate = 720;
			break;
		case 4:
			strcpy(rom_path, "../roms/space-invaders.ch8");
			cpu_clock_rate = 1080;
			break;
		case 5:
			strcpy(rom_path, "../roms/tetris.ch8");
			cpu_clock_rate = 720;
			break;
		case 6:
			strcpy(rom_path, "../roms/tic-tac-toe.ch8");
			cpu_clock_rate = 1080;
			break;
		case 7:
			printf("Enter relative or absolute path to ROM: ");
			scanf("%s", rom_path);
			printf("\nSet CPU clock rate: ");
			scanf("%d", &cpu_clock_rate);
			break;
		default:
			printf("ERROR: Invalid selection.\n");
			exit(EXIT_FAILURE);
	}

	// The emulator delay (in microseconds) is used to slow down the execution 
	// of the main loop to emulate the processor clock speed.
	const int EMU_DELAY = 1. / cpu_clock_rate * 1000000;
	
	// This is the number of cycles it should take to decrement the timers by 1.
	const int TIMER_UPDATE_CYCLES = cpu_clock_rate / 60;

	load_rom(&c, rom_path);

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
					load_rom(&c, rom_path);
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