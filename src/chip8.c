#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "chip8.h"
#include "instructions.h"

void init_sys(Chip8 *c) {
	c->DT = 0;
	c->ST = 0;
	c->I = RAM_START_ADDR;
	c->PC = RAM_START_ADDR;
	c->SP = STACK_START_ADDR;

	for (int i = 0; i < NUM_V_REGISTERS; i++) {
		c->V[i] = 0;
	}

	for (int i = MEM_START_ADDR; i <= MEM_END_ADDR; i++) {
		c->mem[i] = 0;
	}

	// Load font into memory
	for (int i = 0; i < FONTSET_SIZE; i++) {
		c->mem[i + FONTSET_START_ADDR] = FONTSET[i];
	}

	srand(time(NULL));

	c->is_running = 1;
	c->key_down = -1;
	c->start_wait = 0;
	c->end_wait = 0;
}

void load_rom(Chip8 *c, char *file_path) {
	FILE *f = fopen(file_path, "rb");
	if (f == NULL) {
		printf("ERROR: Unable to open file with path '%s'.\n", file_path);
		exit(EXIT_FAILURE);
	}

	size_t bytes_read = fread(&c->mem[RAM_START_ADDR], 1, 
		MEM_SIZE - RAM_START_ADDR, f);
	if (bytes_read == 0) {
		printf("ERROR:Unable to read file.\n");
		exit(EXIT_FAILURE);
	}

	fclose(f);
}

uint16_t fetch_instr(Chip8 *c) {
	// Combine byte at PC and PC + 1 (MSB first) to form 16-bit instruction
	return (c->mem[c->PC] << 8) | c->mem[c->PC + 1];
}

void decd_and_exec_instr(Chip8 *c, uint16_t instr) {
	uint16_t opcode = instr & OPCODE_MASK;
	uint8_t n = get_n(instr);
	uint8_t nn = get_nn(instr);
	c->PC += 2;

	switch(opcode) {
		case 0x0000:
			switch(nn) {
				case 0xE0:
					cls(c);
					break;
				case 0xEE:
					ret(c);
					break;
				default:
					printf("ERROR: Invalid instruction 0x%04x.\n", instr);
					exit(EXIT_FAILURE);
			}
			break;
		case 0x1000:
			jmp_nnn(c, instr);
			break;
		case 0x2000:
			call_nnn(c, instr);
			break;
		case 0x3000:
			se_Vx_nn(c, instr);
			break;
		case 0x4000:
			sne_Vx_nn(c, instr);
			break;
		case 0x5000:
			se_Vx_Vy(c, instr);
			break;
		case 0x6000:
			ld_Vx_nn(c, instr);
			break;
		case 0x7000:
			add_Vx_nn(c, instr);
			break;
		case 0x8000:
			switch(n) {
				case 0x0:
					ld_Vx_Vy(c, instr);
					break;
				case 0x1:
					bor(c, instr);
					break;
				case 0x2:
					band(c, instr);
					break;
				case 0x3:
					bxor(c, instr);
					break;
				case 0x4:
					add_Vx_Vy(c, instr);
					break;
				case 0x5:
					sub(c, instr);
					break;
				case 0x6:
					shr(c, instr);
					break;
				case 0x7:
					subn(c, instr);
					break;
				case 0xE:
					shl(c, instr);
					break;
				default:
					printf("ERROR: Invalid instruction 0x%04x.\n", instr);
					exit(EXIT_FAILURE);
			}
			break;
		case 0x9000:
			sne_Vx_Vy(c, instr);
			break;
		case 0xA000:
			ld_I_nnn(c, instr);
			break;
		case 0xB000:
			jmp_V0_nnn(c, instr);
			break;
		case 0xC000:
			rnd(c, instr);
			break;
		case 0xD000:
			drw(c, instr);
			break;
		case 0xE000:
			switch(nn) {
				case 0x9E:
					skp(c, instr);
					break;
				case 0xA1:
					skpn(c, instr);
					break;
				default:
					printf("ERROR: Invalid instruction 0x%04x.\n", instr);
					exit(EXIT_FAILURE);
			}
			break;
		case 0xF000:
			switch(nn) {
				case 0x07:
					ld_Vx_DT(c, instr);
					break;
				case 0x0A:
					ld_Vx_k(c, instr);
					break;
				case 0x15:
					ld_DT_Vx(c, instr);
					break;
				case 0x18:
					ld_ST_Vx(c, instr);
					break;
				case 0x1E:
					add_I_Vx(c, instr);
					break;
				case 0x29:
					ld_I_f(c, instr);
					break;
				case 0x33:
					ld_I_b(c, instr);
					break;
				case 0x55:
					ld_I_from_reg(c, instr);
					break;
				case 0x65:
					ld_V_from_mem(c, instr);
					break;
				default:
					printf("ERROR: Invalid instruction 0x%04x.\n", instr);
					exit(EXIT_FAILURE);
			}
			break;
	}

	// Ensure that the key press is reset
	c->key_down = -1;
}

// Layout:
// 1  2  3  C
// 4  5  6  D
// 7  8  9  E
// A  0  B  F
int get_key_from_scancode(int sc) {
	switch(sc) {
		case 30:
			return 0x1;
		case 31:
			return 0x2;
		case 32:
			return 0x3;
		case 33:
			return 0xC;
		case 20:
			return 0x4;
		case 26:
			return 0x5;
		case 8:
			return 0x6;
		case 21:
			return 0xD;
		case 4:
			return 0x7;
		case 22:
			return 0x8;
		case 7:
			return 0x9;
		case 9:
			return 0xE;
		case 29:
			return 0xA;
		case 27:
			return 0x0;
		case 6:
			return 0xB;
		case 25:
			return 0xF;
		default:
			return -1;
	}
}

// The following are a set of functions that display the state of the emulator
// in the console for debugging.

void show_registers(Chip8 *c) {
	for (int i = 0; i < NUM_V_REGISTERS; i++) {
		printf("\nV%c: %hhu", HEX[i], c->V[i]);
	}

	printf("\n\nDT: %hhu\n", c->DT);
	printf("ST: %hhu\n", c->ST);

	printf("\nI:  0x%03x\n", c->I);
	printf("PC: 0x%03x\n", c->PC);
	printf("SP: 0x%03x\n", c->SP);
}

// Displays the contents of memory in the range [start_addr, end_addr]
void show_mem(Chip8 *c, uint16_t start_addr, uint16_t end_addr, int chunk_size) {
	if (end_addr < start_addr) {
		printf("ERROR: Unable to display memory. start_addr (%0x) > end_addr (%u).\n",
			start_addr, end_addr);
		exit(EXIT_FAILURE);
	} else if(chunk_size < 1 || chunk_size > 256) {
		printf("ERROR: Chunk size must be in range [1, 256].\n");
		exit(EXIT_FAILURE);
	}

	int cnt = 0;
	for (int i = start_addr; i <= end_addr; i++) {
		if (cnt % (16 * chunk_size) == 0) {
			printf("\n\n    ");
			for (int i = 0; i < 16; i++) {
				printf("    %c", HEX[i]);
			}
		}

		if (cnt % 16 == 0) {
			printf("\n0x%03x: ", i);
		}

		printf("0x%02x ", c->mem[i]);
		cnt++;
	}

	printf("\n");
}
