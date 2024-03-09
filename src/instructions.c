#include <stdio.h>

#include "instructions.h"
#include "chip8.h"

void cls(Chip8 *c, uint16_t instr) {
    for (int i = FRAME_BUFFER_START_ADDR; i <= FRAME_BUFFER_END_ADDR; i++) {
        c->mem[i] = 0;
    }
}

void ret(Chip8 *c, uint16_t instr) {

}

void jmp_nnn(Chip8 *c, uint16_t instr) {
    uint16_t nnn = instr & 0x0FFF;
    c->PC = nnn;
}

void call_nnn(Chip8 *c, uint16_t instr) {

}

void se_Vx_nn(Chip8 *c, uint16_t instr) {

}

void sne_Vx_nn(Chip8 *c, uint16_t instr) {

}

void se_Vx_Vy(Chip8 *c, uint16_t instr) {

}

void ld_Vx_nn(Chip8 *c, uint16_t instr) {
    uint8_t x = (instr & 0x0F00) >> 2;
    uint8_t nn = (instr & 0x00FF);
    c->V[x] = nn;
}

void add_Vx_nn(Chip8 *c, uint16_t instr) {
    uint8_t x = (instr & 0x0F00) >> 2;
    uint8_t nn = (instr & 0x00FF);
    c->V[x] += nn;
}

void ld_Vx_Vy(Chip8 *c, uint16_t instr) {

}

void or(Chip8 *c, uint16_t instr) {

}

void and(Chip8 *c, uint16_t instr) {

}

void xor(Chip8 *c, uint16_t instr) {

}

void add_Vx_Vy(Chip8 *c, uint16_t instr) {

}

void sub(Chip8 *c, uint16_t instr) {

}

void shr(Chip8 *c, uint16_t instr) {

}

void subn(Chip8 *c, uint16_t instr) {
    
}

void shl(Chip8 *c, uint16_t instr) {

}

void sne_Vx_Vy(Chip8 *c, uint16_t instr) {

}

void ld_I_nnn(Chip8* c, uint16_t instr) {
    uint16_t nnn = (instr & 0x0FFF);
    c->I = nnn;
}

void jmp_V0_nnn(Chip8 *c, uint16_t instr) {

}

void rnd(Chip8 *c, uint16_t instr) {

}

void drw(Chip8 *c, uint16_t instr) {
    uint8_t x = (instr & 0x0F00) >> 2;
    uint8_t y = (instr & 0x00F0) >> 1;
    uint8_t n = (instr & 0x000F);

    int x_coord = c->V[x];
    int y_coord = c->V[y];

    for (int i = 0; i < n; i++) {
        int byte_offset = (int) (x_coord / 8) * ((y_coord + n) * 8);
        int pixel_offset = x_coord % 8;
        uint8_t sprite = c->mem[c->I + n];

        uint8_t *frame_byte_ptr = &c->mem[FRAME_BUFFER_START_ADDR + byte_offset];
        uint8_t tmp1 = *frame_byte_ptr;
        uint8_t tmp2 = *(frame_byte_ptr + 1);

        *frame_byte_ptr ^= sprite >> pixel_offset;
        *(frame_byte_ptr + 1) ^= sprite << (8 - pixel_offset);

        if (*frame_byte_ptr < tmp1 || *(frame_byte_ptr + 1) < tmp2) {
            c->V[0xF] = 1;
        } else {
            c->V[0xF] = 0;
        }
    }
    
}  

void skp(Chip8 *c, uint16_t instr) {

}

void skpn(Chip8 *c, uint16_t instr) {

}

void ld_Vx_DT(Chip8 *c, uint16_t instr) {

}

void ld_Vx_k(Chip8 *c, uint16_t instr) {

}

void ld_DT_Vx(Chip8 *c, uint16_t instr) {

}

void ld_ST_Vx(Chip8 *c, uint16_t instr) {

}

void add_I_Vx(Chip8 *c, uint16_t instr) {

}

void ld_I_f(Chip8 *c, uint16_t instr) {

}

void ld_I_b(Chip8 *c, uint16_t instr) {

}

void ld_I_from_reg(Chip8 *c, uint16_t instr) {

}

void ld_V_from_mem(Chip8 *c, uint16_t instr) {

}

