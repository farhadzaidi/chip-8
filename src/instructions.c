#include <stdio.h>

#include "instructions.h"
#include "chip8.h"


// GETTERS


uint8_t get_x(uint16_t instr) {
    return (instr & X_MASK) >> 8;
}

uint8_t get_y(uint16_t instr) {
    return (instr & Y_MASK) >> 4;
}

uint8_t get_n(uint16_t instr) {
    return instr & N_MASK;
}

uint8_t get_nn(uint16_t instr) {
    return instr & NN_MASK;
}

uint16_t get_nnn(uint16_t instr) {
    return instr & NNN_MASK;
}


// INSTRUCTIONS


// Clear screen
void cls(Chip8 *c, uint16_t instr) {
    for (int i = FRAME_BUFFER_START_ADDR; i <= FRAME_BUFFER_END_ADDR; i++) {
        c->mem[i] = 0;
    }
}

// Return from subroutine
void ret(Chip8 *c, uint16_t instr) {

}

// Jump to memory address nnn
void jmp_nnn(Chip8 *c, uint16_t instr) {
    uint16_t nnn = get_nnn(instr);
    c->PC = nnn;
}

// Call subroutine at address nnn
void call_nnn(Chip8 *c, uint16_t instr) {

}

// Skip next instruction if Vx = nn
void se_Vx_nn(Chip8 *c, uint16_t instr) {

}

// Skip next instruction if Vx != nn
void sne_Vx_nn(Chip8 *c, uint16_t instr) {

}

// Skip next instruction if Vx = Vy
void se_Vx_Vy(Chip8 *c, uint16_t instr) {

}

// Load nn into Vx
void ld_Vx_nn(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    uint8_t nn = get_nn(instr);
    c->V[x] = nn;
}

// Load Vx + nn into Vx
void add_Vx_nn(Chip8 *c, uint16_t instr) {
    uint8_t x = (instr & X_MASK) >> 8;
    uint8_t nn = (instr & NN_MASK);
    c->V[x] += nn;
}

// Load Vy into Vx
void ld_Vx_Vy(Chip8 *c, uint16_t instr) {

}

// Load Vx OR Vy into Vx
void or(Chip8 *c, uint16_t instr) {

}

// Load Vx AND Vy into Vx
void and(Chip8 *c, uint16_t instr) {

}

// Load Vx XOR Vy into Vx
void xor(Chip8 *c, uint16_t instr) {

}

// Load Vx + Vy into Vx and load the carry bit into VF
void add_Vx_Vy(Chip8 *c, uint16_t instr) {

}

// Load Vx - Vy into Vx 
// If Vx > Vy, load 1 into VF, else 0
void sub(Chip8 *c, uint16_t instr) {

}

// Load Vx >> 1 into Vx
void shr(Chip8 *c, uint16_t instr) {

}

// Load Vy - Vx into Vx
// If Vx > Vy, load 1 into VF, else 0
void subn(Chip8 *c, uint16_t instr) {
    
}

// Load Vx << 1 into Vx
void shl(Chip8 *c, uint16_t instr) {

}

// Skip next instruction if Vx != Vy
void sne_Vx_Vy(Chip8 *c, uint16_t instr) {

}

// Load nnn into I
void ld_I_nnn(Chip8* c, uint16_t instr) {
    uint16_t nnn = get_nnn(instr);
    c->I = nnn;
}

// Jump to memory address V0 + nnn
void jmp_V0_nnn(Chip8 *c, uint16_t instr) {

}

// Load random byte & nn into Vx
void rnd(Chip8 *c, uint16_t instr) {

}

// TODO
void drw(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    uint8_t y = get_y(instr);
    uint8_t n = get_n(instr);

    int x_coord = c->V[x];
    int y_coord = c->V[y];

    for (int i = 0; i < n; i++) {
        uint8_t sprite = c->mem[c->I + i];
        uint16_t y_offset = (y_coord + i) * 8; // byte offset
        uint16_t x_byte_offset = x_coord / 8;
        uint16_t x_pixel_offset = x_coord % 8;
        uint16_t byte_addr = FRAME_BUFFER_START_ADDR + y_offset + x_byte_offset;

        uint8_t sprite_mask = sprite >> x_pixel_offset;
        uint8_t *frame_byte = &c->mem[byte_addr];
        for (int j = 0; j < 8; j++) {
            if (sprite_mask & MSB_8) {
                *frame_byte ^= MSB_8 >> j;
            }
            sprite_mask <<= 1;
        }

        sprite_mask = sprite << (8 - x_pixel_offset);
        frame_byte += 1;
        for (int j = 0; j < 8; j++) {
            if (sprite_mask & MSB_8) {
                *frame_byte ^= MSB_8 >> j;
            }
            sprite_mask <<= 1;
        }
    }

}

// Skip next instruction if key with the value Vx is pressed
void skp(Chip8 *c, uint16_t instr) {

}

// Skip next instruction if key with the value Vx is not pressed
void skpn(Chip8 *c, uint16_t instr) {

}

// Load DT into Vx
void ld_Vx_DT(Chip8 *c, uint16_t instr) {

}

// Load the value of the next key press into Vx
void ld_Vx_k(Chip8 *c, uint16_t instr) {

}

// Load Vx into DT
void ld_DT_Vx(Chip8 *c, uint16_t instr) {

}

// Load Vx into ST
void ld_ST_Vx(Chip8 *c, uint16_t instr) {

}

// Load I + Vx into I
void add_I_Vx(Chip8 *c, uint16_t instr) {

}

// Load the memory address of the sprite that represents f into I
void ld_I_f(Chip8 *c, uint16_t instr) {

}

// TODO
void ld_I_b(Chip8 *c, uint16_t instr) {

}

// Load all general purpose registers (V0-VF) into memory starting at address I
// I, I + 1, I + 2, ... = V0, V1, V2, ...
void ld_I_from_reg(Chip8 *c, uint16_t instr) {

}

// Load all values in memory starting at address I into each general purpose register
// V0, V1, V2, ... = I, I + 1, I + 2
void ld_V_from_mem(Chip8 *c, uint16_t instr) {

}

