#include "Chip8.h"

const char fontSet[80] = 
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

void Chip8::initialize()
{
    // roms starts at 512
    pc     = 512;
    opcode = 0;
    I      = 0;
    sp     = 0;


    // clear da memories
    for (int i = 0; i < 4096; ++i)
        memory[i] = 0;

    // font set needs to go somewhere
    for (int i = 0; i < 80; ++i)
        memory[i] = fontSet[i];

    // Clear the display
    for (int i = 0; i < 32*64; ++i)
        frameBuffer[i] = 0;

    // clear registers and shit
    for (int i = 0; i < 16; ++i)
    {
        stack[i]    = 0;
        key[i]      = 0;
        V[i]        = 0;
    }

    // Reset timers
    delay_timer = 0;

    // if 0 no sound
    sound_timer = 0;

}

void Chip8::emulateCylce()
{
    // Fetch Opcode
    // opcode is two bytes, so get next two bytes
    opcode = memory[pc] << 8 | memory[pc + 1];

    pc += 2;

    // Decode Opcode
    switch(opcode & 0x000F)
    {
        case 0x0000:
            switch(opcode & 0x000F)
            {
                case 0x0000:
                break;

                case 0x000E:
                break;          
            }
            break;

        case 0x1000:;

        case 0x2000:;

        case 0x3000:;

        case 0x4000:;

        case 0x5000:;

        case 0x6000:;

        case 0x7000:;

        case 0x8000:;

        case 0x9000:;

        //     ANNN
        case 0xA000:;

        case 0xB000:;
        
        //     CNNNN
        case 0xC000:;

        case 0xD000:
        
        case 0xE000:;

        case 0xF000:;

        
    }
    // Execute Opcode

    // Update timers
}