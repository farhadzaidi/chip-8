#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>

typedef struct Chip8 Chip8;

uint8_t get_x(uint16_t instr);
uint8_t get_y(uint16_t instr);
uint8_t get_n(uint16_t instr);
uint8_t get_nn(uint16_t instr);
uint16_t get_nnn(uint16_t instr);

void cls(Chip8 *c, uint16_t instr);
void ret(Chip8 *c, uint16_t instr);
void jmp_nnn(Chip8 *c, uint16_t instr);
void call_nnn(Chip8 *c, uint16_t instr);
void se_Vx_nn(Chip8 *c, uint16_t instr);
void sne_Vx_nn(Chip8 *c, uint16_t instr);
void se_Vx_Vy(Chip8 *c, uint16_t instr);
void ld_Vx_nn(Chip8 *c, uint16_t instr);
void add_Vx_nn(Chip8 *c, uint16_t instr);
void ld_Vx_Vy(Chip8 *c, uint16_t instr);
void or(Chip8 *c, uint16_t instr);
void and(Chip8 *c, uint16_t instr);
void xor(Chip8 *c, uint16_t instr);
void add_Vx_Vy(Chip8 *c, uint16_t instr);
void sub(Chip8 *c, uint16_t instr);
void shr(Chip8 *c, uint16_t instr);
void subn(Chip8 *c, uint16_t instr);
void shl(Chip8 *c, uint16_t instr);
void sne_Vx_Vy(Chip8 *c, uint16_t instr);
void ld_I_nnn(Chip8* c, uint16_t instr);
void jmp_V0_nnn(Chip8 *c, uint16_t instr);
void rnd(Chip8 *c, uint16_t instr);
void drw(Chip8 *c, uint16_t instr);
void skp(Chip8 *c, uint16_t instr);
void skpn(Chip8 *c, uint16_t instr);
void ld_Vx_DT(Chip8 *c, uint16_t instr);
void ld_Vx_k(Chip8 *c, uint16_t instr);
void ld_DT_Vx(Chip8 *c, uint16_t instr);
void ld_ST_Vx(Chip8 *c, uint16_t instr);
void add_I_Vx(Chip8 *c, uint16_t instr);
void ld_I_f(Chip8 *c, uint16_t instr);
void ld_I_b(Chip8 *c, uint16_t instr);
void ld_I_from_reg(Chip8 *c, uint16_t instr);
void ld_V_from_mem(Chip8 *c, uint16_t instr);





#endif