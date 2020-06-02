#include <stdint.h>

class Chip8
{
    // chip8 has 4k memory
    uint8_t memory[4096];

    // registers v0-vf
    uint8_t V[16];

    // frame buffer
    uint8_t  frameBuffer[64 * 32];

    // program counter
    uint16_t pc;

    // current opcode
    uint16_t opcode;

    // index register
    uint16_t I;

    // delay timer
    uint8_t delay_timer;

    // sound timer
    uint8_t sound_timer;

    // stack 16 levels
    uint16_t stack[16];

    // stack pointer
    uint16_t sp;

    // key states
    uint8_t key[16];

    // intialize shit
    void initialize();

    // emulate a cycle
    void emulateCylce();

    // op code functions
    // un used
    //void call_0NNN   ();
    void display_00E0();
    void flow_00EE   ();
    void flow_1NNN   ();
    void flow_2NNN   ();
    void cond_3XNN   ();
    void cond_4XNN   ();
    void cond_5XY0   ();
    void const_6XNN  ();
    void const_7XNN  ();
    void assign_8XY0 ();
    void bitOp_8XY1  ();
    void bitOP_8xY2  ();
    void bitOP_8xY3  ();
    void math_8XY4   ();
    void math_8xY5   ();
    void bitOP_8xY6  ();
    void math_8XY7   ();
    void bitOp_8XYE  ();
    void cond_9XY0   ();
    void mem_ANNN    ();
    void flow_BNNN   ();
    void rand_CXNN   ();
    void disp_DXYN   ();
    void keyOp_EX9E  ();
    void keyOp_EXA1  ();
    void timer_FX07  ();
    void keyOp_FX0A  ();
    void timer_FX15  ();
    void sound_FX18  ();
    void mem_FX1E    ();
    void mem_FX29    ();
    void bcd_FX33    ();
    void mem_FX55    ();
    void mem_FX65    ();



};