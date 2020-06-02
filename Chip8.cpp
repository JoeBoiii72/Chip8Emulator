#include "Chip8.h"
#include <random>
#include "time.h"

#define WIDTH 64
#define HEIGHT 32

unsigned char fontSet[80] = 
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
    pc     = 0x200;
    opcode = 0;
    I      = 0;
    sp     = 0;
    width  = 64;
    height = 32;


    // clear da memories
    for (int i = 0; i < 4096; ++i)
        memory[i] = 0;

    // font set needs to go somewhere
    for (int i = 0; i < 80; ++i)
        memory[i] = fontSet[i];

    // Clear the display
    for (int i = 0; i < HEIGHT*WIDTH; ++i)
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

    // how else will we get random shit
    srand (time(NULL));
}

bool Chip8::loadROM(const char* file)
{
    initialize();

    printf("[*]loading %s\n", file);

    // Open ROM file
    FILE* rom = fopen(file, "rb");

    if (rom == NULL)
    {
        printf("[!]Oh no! I couldn't open the ROM\n");
        return false;
    }

    // Get file size
    fseek(rom, 0, SEEK_END);

    long rom_size = ftell(rom);

    rewind(rom);

    // Allocate memory to store rom
    char* rom_buffer = (char*) malloc(sizeof(char) * rom_size);
    if (rom_buffer == NULL)
    {
        printf("[!]Failed to allocate memory for ROM\n");
        return false;
    }

    // Copy ROM into buffer
    size_t result = fread(rom_buffer, sizeof(char), (size_t)rom_size, rom);
    if (result != rom_size)
    {
        printf("[!]Failed to read ROM\n");
        return false;
    }

    // Copy buffer to memory
    if ((4096-512) > rom_size)
    {
        for (int i = 0; i < rom_size; ++i)
        {
            memory[i + 512] = (uint8_t)rom_buffer[i];   
            // Load into memory starting at 0x200 (=512)
        }
    }
    else
    {
        printf("[!]ROM too large to fit in memory\n");
        return false;
    }

    // Clean up
    fclose(rom);
    free(rom_buffer);

    printf("[*]ROM loaded successfully!\n");
    return true;
}

void Chip8::emulateCylce()
{
    // get the opcode, this is two bytes so yeah!
    opcode = memory[pc] << 8 | memory[pc + 1];

    switch(opcode & 0xF000)
    {
        case 0x0000:

            switch (opcode & 0x000F)
            {
                case 0x0000:
                    display_00E0();
                    break;

                case 0x000E:
                    flow_00EE();
                    break;

                default:
                    unKnownOpcode();
            }
            break;

        case 0x1000:
            flow_1NNN();
            break;

        case 0x2000:
            flow_2NNN();
            break;

        case 0x3000:
            cond_3XNN();
            break;

        case 0x4000:
            cond_4XNN();
            break;

        case 0x5000:
            cond_5XY0();
            break;

        case 0x6000:
            const_6XNN();
            break;

        case 0x7000:
            const_7XNN();
            break;

        case 0x8000:
            switch (opcode & 0x000F)
            {

                case 0x0000:
                    assign_8XY0();
                    break;

                case 0x0001:
                    bitOp_8XY1();
                    break;

                case 0x0002:
                    bitOp_8XY2();
                    break;

                case 0x0003:
                    bitOp_8XY3();
                    break;

                case 0x0004:
                    math_8XY4();
                    break;

                case 0x0005:
                    math_8XY5();
                    break;

                case 0x0006:
                    bitOp_8XY6();
                    break;

                case 0x0007:
                    math_8XY7();
                    break;

                case 0x000E:
                    bitOp_8XYE();
                    break;

                default:
                    unKnownOpcode();
            }
            break;

        case 0x9000:
            cond_9XY0();
            break;

        case 0xA000:
            mem_ANNN();
            break;

        case 0xB000:
            flow_BNNN();
            break;

        case 0xC000:
            rand_CXNN();
            break;

        case 0xD000:
            disp_DXYN();
            break;

        case 0xE000:
            switch (opcode & 0x00FF)
            {
                case 0x009E:
                    keyOp_EX9E();
                    break;

                case 0x00A1:
                    keyOp_EXA1();
                    break;

                default:
                    unKnownOpcode();
            }
            break;

        case 0xF000:
            switch(opcode & 0x00FF)
            {
                case 0x0007:
                    timer_FX07();
                    break;

                case 0x000A:
                    keyOp_FX0A();
                    break;

                case 0x0015:
                    timer_FX15();
                    break;

                case 0x0018:
                    sound_FX18();
                    break;

                case 0x001E:
                    mem_FX1E();
                    break;

                case 0x0029:
                    I = V[(opcode & 0x0F00) >> 8] * 0x5;
                    pc += 2;
                    break;

                case 0x0033:
                    bcd_FX33();
                    break;

                case 0x0055:
                    mem_FX55();
                    break;

                case 0x0065:
                    mem_FX65();
                    break;

                default:
                    unKnownOpcode();
            }
            break;

        default:
            unKnownOpcode();
    }


    // Update timers
    if (delay_timer > 0)
        --delay_timer;

    if (sound_timer > 0)
        if(sound_timer == 1);
            // TODO: Implement sound
        --sound_timer;

}

// error time
void Chip8::unKnownOpcode()
{
    printf("[!]What the fuck, this isn't an opcode: %.4X\n", opcode);
    exit(1);
}

// clears the screen
void Chip8::display_00E0()
{
    for (int i = 0; i < WIDTH*HEIGHT; ++i)
        frameBuffer[i] = 0;

    drawFlag = true;
    pc = pc + 2;
}

// returns from subroutine
void Chip8::flow_00EE()
{   
    --sp;
    pc = stack[sp];
    pc = pc + 2;
}

// jumps to the address at NNN
void Chip8::flow_1NNN()
{
    pc = opcode & 0x0FFF;
}

/*
 calls subroutine at NNN
*/
void Chip8::flow_2NNN()
{
    // get address
    uint16_t subroutine_address = opcode & 0x0FFF;

    // store program counter in stack;
    stack[sp] = pc;

    ++sp;

    // change program counter
    pc = subroutine_address;

    // no program counter incrementing
}

/*
Skips the next instruction if VX 
equals NN. (Usually the next instruction 
is a jump to skip a code block) 
*/
void Chip8::cond_3XNN()
{
    uint8_t  X  = (opcode & 0x0F00) >> 8;
    uint16_t NN =  opcode & 0x00FF;
    pc = (V[X] == NN) ? pc + 4 : pc + 2;
}

/*
Skips the next instruction if 
VX doesn't equal NN. (Usually the 
next instruction is a jump to skip a code block) 
*/
void Chip8::cond_4XNN()
{
    uint8_t  X  = (opcode & 0x0F00) >> 8;
    uint16_t NN =  opcode & 0x00FF;
    pc = (V[X] != NN) ? pc + 4 : pc + 2;
}

/*
Skips the next instruction if VX equals VY. 
(Usually the next instruction is a jump to skip a code block) 
*/
void Chip8::cond_5XY0()
{
    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t Y = (opcode & 0x00F0) >> 4;
    pc = (V[X] == V[Y]) ? pc + 4 : pc + 2;
}

// Sets VX to NN.
void Chip8::const_6XNN()
{
    uint8_t  X  = (opcode & 0x0F00) >> 8;
    uint16_t NN =  opcode & 0x00FF;
    
    V[X] = NN;

    pc = pc + 2;
}

// Adds NN to VX. (Carry flag is not changed) 
void Chip8::const_7XNN()
{
    uint8_t  X  = (opcode & 0x0F00) >> 8;
    uint16_t NN =  opcode & 0x00FF;

    V[X] = V[X] + NN;

    pc = pc + 2;
}

// Sets VX to the value of VY. 
void Chip8::assign_8XY0()
{
    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t Y = (opcode & 0x00F0) >> 4;
    V[X] = V[Y];
    pc = pc + 2;
}

// Sets VX to VX or VY. (Bitwise OR operation) 
void Chip8::bitOp_8XY1()
{
    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t Y = (opcode & 0x00F0) >> 4;
    V[X] = V[X] | V[Y];
    pc = pc + 2;
}

// Sets VX to VX and VY. (Bitwise AND operation) 
void Chip8::bitOp_8XY2()
{
    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t Y = (opcode & 0x00F0) >> 4;
    V[X] = V[X] & V[Y];
    pc = pc + 2;
}

// Sets VX to VX xor VY. 
void Chip8::bitOp_8XY3()
{
    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t Y = (opcode & 0x00F0) >> 4;
    V[X] = V[X] ^ V[Y];
    pc = pc + 2;
}

/*
Adds VY to VX. VF is set to 1 when there's 
a carry, and to 0 when there isn't. 
*/
void Chip8::math_8XY4()
{
    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t Y = (opcode & 0x00F0) >> 4;
    V[0XF] = (V[Y] > (0xFF - V[X])) ? 1 : 0;    
    V[X] = V[X] + V[Y];
    pc = pc + 2;
}

/*
VY is subtracted from VX.
VF is set to 0 when there's a 
borrow, and 1 when there isn't. 
*/
void Chip8::math_8XY5()
{
    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t Y = (opcode & 0x00F0) >> 4;
    V[0XF]    = (V[Y] > V[X]) ? 0 : 1;
    V[X]      = V[X] - V[Y];
    pc = pc + 2;
}

/*
Stores the least significant bit of VX in VF 
and then shifts VX to the right by 1.
*/
void Chip8::bitOp_8XY6()
{
    uint8_t X = (opcode & 0x0F00) >> 8;
    V[0xF] = V[X] & 1;
    V[X]   = V[X] >> 1;
    pc = pc + 2;
}

/*
Sets VX to VY minus VX.
VF is set to 0 when there's a 
borrow, and 1 when there isn't. 
*/
void Chip8::math_8XY7()
{
    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t Y = (opcode & 0x00F0) >> 4;

    // check if borrow used
    // 1 if there is
    // 0 if not
    V[0XF] = (V[Y] > V[Y]) ? 0 : 1;

    // Vx = Vy - Vx
    V[X] = V[Y] - V[X];

    pc = pc + 2;
}

/*
VF = VX's most significant bit
VX is shifted left by one
*/
void Chip8::bitOp_8XYE()
{
    uint8_t X = (opcode & 0x0F00) >> 8;
    V[0xF] = V[X] >> 7;  
    V[X]   = V[X] << 1;
    pc = pc + 2;
}

/*
Skips the next instruction if VX doesn't equal VY. 
(Usually the next instruction 
is a jump to skip a code block) 
*/
void Chip8::cond_9XY0()
{
    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t Y = (opcode & 0x00F0) >> 4;

    pc = (V[X] != V[Y]) ? pc + 4: pc + 2;
}

/*
Sets I as the last 12 bits the opcode.
These 12 bits are usually an address.
*/ 
void Chip8::mem_ANNN()
{
    uint16_t addressNNN = opcode & 0x0FFF;
    I = addressNNN;
    pc += 2;   
}

/*
jump to NNN
*/
void Chip8::flow_BNNN()
{
    uint16_t NNN = (opcode & 0x0FFF) >> 0;
    pc = V[0] + NNN;
}

/*
assign V[x] with random number masked with NN
*/
void Chip8::rand_CXNN()
{
    uint8_t X       = (opcode & 0x0F00) >> 8;
    uint8_t NN      = (opcode & 0x00FF) >> 0;

    // random number masked by NN
    uint8_t newValue = (rand()%(255 + 1)) & NN;

    V[X] = newValue;

    pc = pc + 2;
}

/*
Draws a sprite using a given x and y coordinates.
The sprite that is drawn is caculated using "I"s value.
This means it will draw the sprite at "memory[I]".
*/
void Chip8::disp_DXYN()
{
    // get Vx, Vy and N(height)
    uint8_t Vx = V[(opcode & 0x0F00) >> 8];
    uint8_t Vy = V[(opcode & 0x00F0) >> 4];
    uint8_t h  =   (opcode & 0x000F) >> 0;
    uint16_t pixel;

    // restet VF register
    V[0xF] = 0;

    for(int y = 0; y < h; y++)
    {
        // get pixel from sprite in memory
        pixel = memory[I + y];

        // sprite is always 8 bits wide
        for(int x = 0; x < 8; x++)
        {
            // check if sprites pixel is set
            // other wise we don't need to draw shit
            if((pixel & (0x80 >> x)) != 0)
            {
                // index of pixel in frame buffer
                int frame_index = (Vx + x + ((Vy + y) * WIDTH));

                // if pixel in frame buffer is set, set V15 register
                if(frameBuffer[frame_index] == 1)
                    V[0xF] = 1;   

                // set pixel
                frameBuffer[frame_index] ^= 1;
            }
        }
    }

    drawFlag = true;
    pc = pc + 2;
}

// skip instruction if key in VX is pressed
void Chip8::keyOp_EX9E()
{
    if (key[V[(opcode & 0x0F00) >> 8]] != 0)
        pc +=  4;
    else
        pc += 2;
}

// skip instruction if key in VX is not pressed
void Chip8::keyOp_EXA1()
{
    if (key[V[(opcode & 0x0F00) >> 8]] == 0)
        pc +=  4;
    else
        pc += 2;
}

/*
Sets Vx to the value of the delay timer.
*/
void Chip8::timer_FX07()
{
    uint8_t X = (opcode & 0x0F00) >> 8;
    V[X] = delay_timer;
    pc = pc + 2;
}

/*
A key press is awaited, and then stored in VX. 
(Blocking Operation. All instruction 
halted until next key event) 
*/
void Chip8::keyOp_FX0A()
{
    uint8_t X = (opcode & 0x0F00) >> 8;

    // iterate through keys
    bool pressed = false;
    for(int i = 0; i < 16; ++i)
    {
        // if key pressed
        if(key[i] != 0)
        {      
            // store key in VX
            V[X] = i;
            pressed = true;
        }
    }

    if(!pressed)
        return;

    pc = pc + 2;
}

// Sets the delay timer to VX. 
void Chip8::timer_FX15()
{
    uint8_t X = (opcode & 0x0F00) >> 8;
    delay_timer = V[X];
    pc = pc + 2;   
}

// Sets the sound timer to VX. 
void Chip8::sound_FX18()
{
    uint8_t X = (opcode & 0x0F00) >> 8;
    sound_timer = V[X];
    pc = pc + 2;   
}
/*
Adds VX to I. VF is set to 1 when there 
is a range overflow (I+VX>0xFFF), and 
to 0 when there isn't
*/
void Chip8::mem_FX1E()
{
    uint8_t X = (opcode & 0x0F00) >> 8;

    // range overflow
    V[0xF] = ((I+V[X]) > 0xFFF) ? 1 : 0;

    I = I + V[X];

    pc = pc + 2;    
}

/*
Sets I to the location of the sprite 
for the character in VX. Characters 
0-F (in hexadecimal) are represented by a 4x5 font. 
*/
void Chip8::mem_FX29()
{
    uint8_t X = (opcode & 0x0F00) >> 8;
    I  = V[X] * 5;
    pc = pc + 2;
}

/*
Stores the binary-coded decimal representation of VX, with the 
most significant of three digits at the address in I, the middle 
digit at I plus 1, and the least significant digit at I plus 2. 
*/
void Chip8::bcd_FX33()
{
    uint8_t X     = (opcode & 0x0F00) >> 8;
    memory[I]     =   V[X] / 100       ;
    memory[I + 1] = ( V[X] / 10  ) % 10;
    memory[I + 2] = ( V[X] % 100 ) % 10;
    pc += 2;
}

/*
Stores V0 to VX (including VX) in memory starting at address I. 
The offset from I is increased by 1 for each value written, but 
I itself is left unmodified.
*/
void Chip8::mem_FX55()
{
    uint8_t X = (opcode & 0x0F00) >> 8;
    for(int i = 0; i <= X; ++i)
    {
        memory[I + i] = V[i];
    }

    // the orignal iterpreter does this
    // a p p a r e n t l y
    I  = I  + X + 1;
    pc = pc + 2;
}

/*
Fills V0 to VX (including VX) with values from memory 
starting at address I. The offset from I is increased 
by 1 for each value written, but I itself is left unmodified.
*/
void Chip8::mem_FX65()
{
    uint8_t X = (opcode & 0x0F00) >> 8;
    for(int i = 0; i <= X; ++i)
    {
        V[i] = memory[I + i];
    }

    // the orignal iterpreter does this
    // a p p a r e n t l y
    I  = I  + X + 1;
    pc = pc + 2;
}