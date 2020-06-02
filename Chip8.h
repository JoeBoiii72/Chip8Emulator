#include <stdint.h>

class Chip8
{
    // chip8 has 4k memory
    uint8_t memory[4096];

    // registers v0-vf
    uint8_t V[16];

    // frame buffer
    uint8_t  gfx[64 * 32];

    // program counter
    uint16_t pc;

    // current opcode
    uint16_t opcode;

    // index register
    uint16_t I;

    // delay timer
    uint8_t delay_timer;

    // stack 16 levels
    uint16_t stack[16];

    // stack pointer
    uint16_t sp;

    // key states
    uint8_t key[16];

    // sound timer
    uint8_t sound_timer;

    // intialize shit
    void initialize();

    // emulate a cycle
    void emulateCylce();
};