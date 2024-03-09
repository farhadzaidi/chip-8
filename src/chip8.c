#include <stdio.h>
#include <stdlib.h>

#include "chip8.h"
#include "instructions.h"

void init_sys(Chip8 *c) {
	// Reset system (set all registers/memory locations to 0)	
	c->DT = 0;
	c->ST = 0;
	c->PC = 0;
	c->I = 0;
	c->SP = 0;

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

	// Initialize PC and SP
	c->PC = RAM_START_ADDR;
	c->SP = -1;
}

void load_rom(Chip8 *c, char *file_path) {
	FILE *f = fopen(file_path, "rb");
	if (f == NULL) {
		printf("ERROR: Unable to open file with path '%s'.", file_path);
		exit(EXIT_FAILURE);
	}

	size_t bytes_read = fread(&c->mem[RAM_START_ADDR], 1, 
		MEM_SIZE - RAM_START_ADDR, f);
	if (bytes_read == 0) {
		printf("ERROR:Unable to read file.");
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
				case 0x00:
					cls(c, instr);
					break;
				case 0xE0:
					ret(c, instr);
					break;
				default:
					printf("Invalid instruction 0x%04x.\n", instr);
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
					or(c, instr);
					break;
				case 0x2:
					and(c, instr);
					break;
				case 0x3:
					xor(c, instr);
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
					printf("Invalid instruction 0x%04x.\n", instr);
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
					printf("Invalid instruction 0x%04x.\n", instr);
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
					printf("Invalid instruction 0x%04x.\n", instr);
					exit(EXIT_FAILURE);
			}
			break;
	}
}

// void st_push(Chip8 *c, uint16_t val) {
// 	c->SP++;
// 	if (c->SP >= 16) {
// 		printf("ERROR: Stack overflow.");
// 		exit(EXIT_FAILURE);
// 	}

// 	c->stack[c->SP] = val;
// }

// uint16_t st_pop(Chip8 *c) {
// 	if (c->SP < 0) {
// 		printf("ERROR: Cannot pop from empty stack.");
// 		exit(EXIT_FAILURE);
// 	}

// 	uint16_t res = c->stack[c->SP];
// 	c->SP--;
// 	return res;
// }


// The following are a set of functions that display the state of the emulator
// in the console for debugging.

void show_registers(Chip8 *c) {
	for (int i = 0; i < NUM_V_REGISTERS; i++) {
		printf("\nV%c: %hhu", HEX[i], c->V[i]);
	}

	printf("\n\nDT: %hhu\n", c->DT);
	printf("ST: %hhu\n", c->ST);

	printf("\nI:  0x%03x\n", c->I);
	printf("PC: 0x%03x\n\n", c->PC);
}

void show_mem(Chip8 *c) {
	show_mem_snip(c, MEM_START_ADDR, MEM_END_ADDR);
}

// Displays the contents of memory in the range [start_addr, end_addr]
void show_mem_snip(Chip8 *c, uint16_t start_addr, uint16_t end_addr) {
	if (end_addr < start_addr) {
		printf("ERROR: Unable to display memory. start_addr (%0x) > end_addr (%u).\n",
			start_addr, end_addr);
		exit(EXIT_FAILURE);
	}

	int cnt = 0;
	for (int i = start_addr; i <= end_addr; i++) {
		if (cnt % 16 == 0) {
			printf("\n0x%03x: ", i);
		}

		printf("0x%02x ", c->mem[i]);
		cnt++;
	}

	printf("\n");
}
