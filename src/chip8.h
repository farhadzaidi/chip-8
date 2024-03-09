#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#include "instructions.h"

#define MEM_START_ADDR 0x000
#define MEM_END_ADDR 0xFFF

#define ROM_START_ADDR 0x000
#define ROM_END_ADDR 0x1FF

#define FONTSET_START_ADDR 0x000
#define FONTSET_END_ADDR 0x04F

#define RAM_START_ADDR 0x200
#define RAM_END_ADDR 0xFFF

#define FRAME_BUFFER_START_ADDR 0xF00
#define FRAME_BUFFER_END_ADDR 0xFFF

#define FONTSET_SIZE 80
#define NUM_V_REGISTERS 16
#define MEM_SIZE 4096
#define STACK_SIZE 16

// Used to set speed of emulation
#define INSTR_PER_SEC 700

#define OPCODE_MASK 0xF000
#define X_MASK 0x0F00
#define Y_MASK 0x00F0
#define N_MASK 0x000F
#define NN_MASK 0x00FF
#define NNN_MASK 0x0FFF

#define MSB_8 0x80
#define MSB_16 0x8000

typedef struct Chip8 Chip8;

static const char HEX[] = "0123456789ABCDEF";

static const uint8_t FONTSET[FONTSET_SIZE] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

struct Chip8 {
	uint8_t V[NUM_V_REGISTERS];
	uint8_t DT;
	uint8_t ST;
	uint16_t PC;
	uint16_t I;
	uint16_t SP;

	uint8_t mem[MEM_SIZE];
	uint16_t stack[STACK_SIZE];
};

void init_sys(Chip8 *c);
void load_rom(Chip8 *c, char *file_path);
uint16_t fetch_instr(Chip8 *c);
void decd_and_exec_instr(Chip8 *c, uint16_t instr);

// void st_push(Chip8 *c, uint16_t val);
// uint16_t st_pop(Chip8 *c);

void show_registers(Chip8 *c);
void show_mem(Chip8 *c);
void show_mem_snip(Chip8 *c, uint16_t start_addr, uint16_t end_addr);
void show_stack(Chip8 *c);

#endif