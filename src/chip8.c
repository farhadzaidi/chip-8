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

void read_rom(Chip8 *c, char *file_path) {
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

// Fetch the current instruction and increment the PC to point to the next one
uint16_t fetch_instr(Chip8 *c) {
	uint16_t instr = (c->mem[c->PC] << 8) | c->mem[c->PC + 1];
	c->PC += 2;
	return instr;
}

void decd_and_exec_instr(Chip8 *c, uint16_t instr) {
	uint16_t opcode = instr & 0xF000;
	uint16_t n = instr & 0x000F;
	uint16_t nn = instr & 0x00FF;

	switch(opcode) {
		case 0x0000:
			switch(n) {
				case 0x0000:
					cls(c, instr);
					break;
				case 0x000E:
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
				case 0x0000:
					ld_Vx_Vy(c, instr);
					break;
				case 0x0001:
					or(c, instr);
					break;
				case 0x0002:
					and(c, instr);
					break;
				case 0x0003:
					xor(c, instr);
					break;
				case 0x0004:
					add_Vx_Vy(c, instr);
					break;
				case 0x0005:
					sub(c, instr);
					break;
				case 0x0006:
					shr(c, instr);
					break;
				case 0x0007:
					subn(c, instr);
					break;
				case 0x000E:
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
				case 0x009E:
					skp(c, instr);
					break;
				case 0x00A1:
					skpn(c, instr);
					break;
				default:
					printf("Invalid instruction 0x%04x.\n", instr);
					exit(EXIT_FAILURE);
			}
			break;
		case 0xF000:
			switch(nn) {
				case 0x0007:
					ld_Vx_DT(c, instr);
					break;
				case 0x000A:
					ld_Vx_k(c, instr);
					break;
				case 0x0015:
					ld_DT_Vx(c, instr);
					break;
				case 0x0018:
					ld_ST_Vx(c, instr);
					break;
				case 0x001E:
					add_I_Vx(c, instr);
					break;
				case 0x0029:
					ld_I_f(c, instr);
					break;
				case 0x0033:
					ld_I_b(c, instr);
					break;
				case 0x0055:
					ld_I_from_reg(c, instr);
					break;
				case 0x0065:
					ld_V_from_mem(c, instr);
					break;
				default:
					printf("Invalid instruction 0x%04x.\n", instr);
					exit(EXIT_FAILURE);
			}
			break;
	}
}

void st_push(Chip8 *c, uint16_t val) {
	c->SP++;
	if (c->SP >= 16) {
		printf("ERROR: Stack overflow.");
		exit(EXIT_FAILURE);
	}

	c->stack[c->SP] = val;
}

uint16_t st_pop(Chip8 *c) {
	if (c->SP < 0) {
		printf("ERROR: Cannot pop from empty stack.");
		exit(EXIT_FAILURE);
	}

	uint16_t res = c->stack[c->SP];
	c->SP--;
	return res;
}

// DEBUG

void show_mem(Chip8 *c) {
	show_mem_snip(c, MEM_START_ADDR, MEM_END_ADDR);
}

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

	printf("\n\n");
}
