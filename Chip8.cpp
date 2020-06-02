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

    // Decode Opcode
    // keep only first 4 bits
    switch(opcode & 0xF000)
    {
        case 0x0000:
        {
            // check last eight bits
            switch(opcode & 0x00FF)
            {  
                case 0x00E0:
                    display_00E0();
                    break;

                case 0x00EE:
                    flow_00EE();
                    break;          
            }
            break;
        }

        case 0x1000:
        {
            flow_1NNN();
            break;
        }

        case 0x2000:
        {
            flow_2NNN();
            break;
        }

        case 0x3000:
        {
            cond_3XNN();
            break;
        }

        case 0x4000:
        {
            cond_4XNN();
            break;
        }

        case 0x5000:
        {
            cond_5XY0();
            break;
        }

        case 0x6000:
        {
            const_6XNN();
            break;
        }

        case 0x7000:
        {
            const_7XNN();
            break;
        }

        case 0x8000:
        {
            // check last four bits
            switch(opcode & 0x000F)
            {  
                case 0x0000:
                    assign_8XY0();
                    break;

                case 0x0001:
                    bitOp_8XY1();
                    break;   

                case 0x0002:
                    bitOP_8xY2();
                    break;   

                case 0x0003:
                    bitOP_8xY3();
                    break;   

                case 0x0004:
                    math_8XY4();
                    break;  

                case 0x0005:
                    math_8xY5();
                    break;   

                case 0x0006:
                    bitOP_8xY6();
                    break;   

                case 0x0007:
                    math_8XY7();
                    break;  

                case 0x000E:
                    bitOp_8XYE();
                    break;         
            }
            break;
        }

        case 0x9000:
        {
            cond_9XY0();
            break;
        }

        case 0xA000:
        {
            mem_ANNN();
            break;
        }

        case 0xB000:
        {
            flow_BNNN();
            break;
        }
        
        case 0xC000:
        {
            rand_CXNN();
        }

        case 0xD000:
        {
            disp_DXYN();
            break;
        }

        case 0xE000:
        {
            // check last eight bits
            switch(opcode & 0x00FF)
            {
                case 0x009E:
                {
                    keyOp_EX9E();
                    break;
                }

                case 0x00A1:
                {
                    keyOp_EXA1();
                    break;
                }
            }
        }

        case 0xF000:
        {
            // check last eight bits
            switch(opcode & 0x00FF)
            {
                case 0x0007:
                {
                    timer_FX07();
                    break;
                }

                case 0x000A:
                {
                    keyOp_FX0A();
                    break;
                }

                case 0x0015:
                {
                    timer_FX15();
                    break;
                }

                case 0x0018:
                {
                    sound_FX18();
                    break;
                }

                case 0x001E:
                {
                    mem_FX1E();
                    break;
                }

                case 0x0029:
                {
                    mem_FX29();
                    break;
                }

                case 0x0033:
                {
                    bcd_FX33();
                    break;
                }

                case 0x0055:
                {
                    mem_FX55();
                    break;
                }

                case 0x0065:
                {
                    mem_FX65();
                    break;
                }
            }
        }

        
    }
    // Execute Opcode

    // Update timers
}