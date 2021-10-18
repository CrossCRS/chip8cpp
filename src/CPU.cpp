#include "CPU.hpp"

using namespace chip8;

CPU::CPU() {
    this->Reset();
}

void CPU::Reset() {
    memset(this->V, 0, sizeof(this->V));
    this->I = 0;

    this->PC = 0x200; // Programs start at 0x200
    memset(this->S, 0, sizeof(this->S));
    this->SP = -1;

    this->DT = 0;
    this->ST = 0;

    memset(this->Memory, 0, sizeof(this->Memory));

    this->IsHalted = false;

    // Insert testing stuff...
    this->Memory[0x200] = 0x00;
    this->Memory[0x201] = 0xE0; // CLS
    this->Memory[0x202] = 0x12;
    this->Memory[0x203] = 0x00; // JMP 0x200

    printf("READY.\n");
}

Opcode CPU::DecodeOpcode() {
    // Get two opcode bytes from memory
    uint16_t opcode = (this->Memory[this->PC] << 8) | this->Memory[this->PC+1];
    Opcode decoded_opcode;
    
    decoded_opcode.opcode = opcode;
    // TODO: Calculate them as necessary based on current opcode?
    decoded_opcode.NNN = opcode & 0x0FFF;
    decoded_opcode.NN = opcode & 0x00FF;
    decoded_opcode.N = opcode & 0x000F;
    decoded_opcode.X = (opcode & 0x0F00) >> 8;
    decoded_opcode.Y = (opcode & 0x00F0) >> 4;

    return decoded_opcode;
}

void CPU::Step() {
    Opcode opcode = this->DecodeOpcode();
    printf("PC: 0x%x\nOPCODE: 0x%x\nNNN: 0x%x\nNN: 0x%x\nN: 0x%x\nX: 0x%x\nY: 0x%x\n\n", this->PC, opcode.opcode, opcode.NNN, opcode.NN, opcode.N, opcode.X, opcode.Y);
    
    this->PC += 2;

    switch (opcode.opcode & 0xF000) {
        // TODO: Implement opcodes here
    }
}