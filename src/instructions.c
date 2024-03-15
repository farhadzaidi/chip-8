#include <stdio.h>
#include <stdlib.h>

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
void cls(Chip8 *c) {
    for (int i = FRAME_BUFFER_START_ADDR; i <= FRAME_BUFFER_END_ADDR; i++) {
        c->mem[i] = 0;
    }
}

// Return from subroutine
void ret(Chip8 *c) {
    if (c->SP == STACK_START_ADDR) {
        printf("ERROR: Stack underflow.\n");
        exit(EXIT_FAILURE);
    }

    c->SP -= 2;
    uint8_t msb = c->mem[c->SP];
    uint8_t lsb = c->mem[c->SP + 1];
    c->PC = (msb << 8) | lsb;
}

// Jump to memory address nnn
void jmp_nnn(Chip8 *c, uint16_t instr) {
    uint16_t nnn = get_nnn(instr);
    c->PC = nnn;
}

// Call subroutine at address nnn
void call_nnn(Chip8 *c, uint16_t instr) {
    if (c->SP > STACK_END_ADDR) {
        printf("ERROR: Stack Overflow.\n");
        exit(EXIT_FAILURE);
    }

    uint8_t msb = (c->PC & 0xFF00) >> 8;
    uint8_t lsb = (c->PC & 0x00FF);
    
    c->mem[c->SP] = msb;
    c->mem[c->SP + 1] = lsb;
    c->SP += 2;
    c->PC = get_nnn(instr);
}

// Skip next instruction if Vx = nn
void se_Vx_nn(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    uint8_t nn = get_nn(instr);

    if (c->V[x] == nn) {
        c->PC += 2;
    }
}

// Skip next instruction if Vx != nn
void sne_Vx_nn(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    uint8_t nn = get_nn(instr);

    if (c->V[x] != nn) {
        c->PC += 2;
    }
}

// Skip next instruction if Vx = Vy
void se_Vx_Vy(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    uint8_t y = get_y(instr);

    if (c->V[x] == c->V[y]) {
        c->PC += 2;
    }
}

// Load nn into Vx
void ld_Vx_nn(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    uint8_t nn = get_nn(instr);
    c->V[x] = nn;
}

// Load Vx + nn into Vx
void add_Vx_nn(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    uint8_t nn = get_nn(instr);
    c->V[x] += nn;
}

// Load Vy into Vx
void ld_Vx_Vy(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    uint8_t y = get_y(instr);
    c->V[x] = c->V[y];
}

// Load Vx OR Vy into Vx
void bor(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    uint8_t y = get_y(instr);

    c->V[x] |= c->V[y];
}

// Load Vx AND Vy into Vx
void band(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    uint8_t y = get_y(instr);

    c->V[x] &= c->V[y];
}

// Load Vx XOR Vy into Vx
void bxor(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    uint8_t y = get_y(instr);

    c->V[x] ^= c->V[y];
}

// Load Vx + Vy into Vx and load the carry bit into VF
void add_Vx_Vy(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    uint8_t y = get_y(instr);

    uint16_t res = c->V[x] + c->V[y];
    c->V[x] = res;
    c->V[0xF] = res > 255 ? 1 : 0;
}

// Load Vx - Vy into Vx 
// If Vx > Vy, load 1 into VF, else 0
void sub(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    uint8_t y = get_y(instr);

    uint8_t flag = c->V[x] > c->V[y] ? 1 : 0;
    c->V[x] -= c->V[y];
    c->V[0xF] = flag;
}

// Load Vx >> 1 into Vx
void shr(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    c->V[x] >>= 1;
}

// Load Vy - Vx into Vx
// If Vy > Vx, load 1 into VF, else 0
void subn(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    uint8_t y = get_y(instr);

    uint8_t flag = c->V[y] > c->V[x] ? 1 : 0;
    c->V[x] = c->V[y] - c->V[x];
    c->V[0xF] = flag;
}

// Load Vx << 1 into Vx
void shl(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    c->V[x] <<= 1;
}

// Skip next instruction if Vx != Vy
void sne_Vx_Vy(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    uint8_t y = get_y(instr);

    if (c->V[x] != c->V[y]) {
        c->PC += 2;
    }
}

// Load nnn into I
void ld_I_nnn(Chip8* c, uint16_t instr) {
    uint16_t nnn = get_nnn(instr);
    c->I = nnn;
}

// Jump to memory address V0 + nnn
void jmp_V0_nnn(Chip8 *c, uint16_t instr) {
    uint16_t nnn = get_nnn(instr);
    c->PC = c->V[0] + nnn;
}

// Load random byte & nn into Vx
void rnd(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    uint8_t nn = get_nn(instr);

    uint8_t rnd_byte = rand();
    c->V[x] = rnd_byte & nn;
}

// Draw n-byte sprite at (Vx, Vy)
void drw(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    uint8_t y = get_y(instr);
    uint8_t n = get_n(instr);

    c->V[0xF] = 0;

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
            if (sprite_mask & 0x80) {
                *frame_byte ^= 0x80 >> j;
                int new = *frame_byte & (0x80 >> j);
                if (!new) {
                    c->V[0xF] = 1;
                }

            }

            sprite_mask <<= 1;
        }

        sprite_mask = sprite << (8 - x_pixel_offset);
        frame_byte += 1;
        for (int j = 0; j < 8; j++) {
            if (sprite_mask & 0x80) {
                *frame_byte ^= 0x80 >> j;
                int new = *frame_byte & (0x80 >> j);
                if (!new) {
                    c->V[0xF] = 1;
                }
            }

            sprite_mask <<= 1;
        }
    }

    c->update_screen = 1;
}

// Skip next instruction if key with the value Vx is pressed
void skp(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    if (c->key_down == c->V[x]) {
        c->PC += 2;
    }
}

// Skip next instruction if key with the value Vx is not pressed
void skpn(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    if (c->key_down != c->V[x]) {
        c->PC += 2;
    }
}

// Load DT into Vx
void ld_Vx_DT(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    c->V[x] = c->DT;
}

// Load the value of the next key press into Vx
void ld_Vx_k(Chip8 *c, uint16_t instr) {
    // This instruction will be executed twice, the first time to start the
    // wait period and the second time when a key is pressed (wait period ends)

    // If we haven't started the wait period, then this is the first time this
    // instruction is called and so we start the wait period (for the key press)
    // PC is decremented by 2 to set it back to this instruction since we 
    // incremented it by 2 at the start of decd_and_exec_instr

    // If we have ended the wait period, then this is the second time this 
    // instruction is executed and we can go ahead with the operation (since we 
    // now have our valid key press)

    if (!c->start_wait) {
        c->start_wait = 1;
        c->PC -= 2;
    } else if (c->end_wait) { 
        uint8_t x = get_x(instr);
        c->V[x] = c->key_down;

        c->start_wait = 0;
        c->end_wait = 0;
    }
}

// Load Vx into DT
void ld_DT_Vx(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    c->DT = c->V[x];
}

// Load Vx into ST
void ld_ST_Vx(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    c->ST = c->V[x];
}

// Load I + Vx into I
void add_I_Vx(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    c->I += c->V[x];
}

// Load the memory address of the sprite that represents f into I
void ld_I_f(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    c->I = FONTSET_START_ADDR + 5 * c->V[x];
}

// Load:
    // the hundreds digit of x into location at I
    // the tens digit of x into location at I + 1
    // the ones digit of x into location at I + 2
void ld_I_b(Chip8 *c, uint16_t instr) {
    uint8_t Vx = c->V[get_x(instr)];
    c->mem[c->I] = Vx / 100;
    Vx %= 100;
    c->mem[c->I + 1] = Vx / 10;
    Vx %= 10;
    c->mem[c->I + 2] = Vx;
}

void ld_I_from_reg(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    for (int i = 0; i <= x; i++) {
        c->mem[c->I + i] = c->V[i];
    }
}

void ld_V_from_mem(Chip8 *c, uint16_t instr) {
    uint8_t x = get_x(instr);
    for (int i = 0; i <= x; i++) {
        c->V[i] = c->mem[c->I + i];
    }
}

