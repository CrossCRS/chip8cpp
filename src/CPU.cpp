#include "CPU.hpp"

using namespace chip8;

CPU::CPU() : m_random_device(), m_rng(m_random_device()), m_rng_dist255(0, 255) {
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
    printf("PC: 0x%03X\nOPCODE: 0x%04X\nNNN: 0x%03X\nNN: 0x%02X\nN: 0x%01X\nX: 0x%01X\nY: 0x%01X\n\n", this->PC, opcode.opcode, opcode.NNN, opcode.NN, opcode.N, opcode.X, opcode.Y);
    
    this->PC += 2;

    switch (opcode.opcode & 0xF000) {
        case 0x0000:
            switch (opcode.opcode) {
                case 0x00E0: // CLS - clear screen
                    break;
                case 0x00EE: // RET - return from subroutine
                    if (this->SP < 0) {
                        // Stack underflow
                        // TODO: Error handling
                        this->IsHalted = true;
                    } else {
                        this->PC = this->S[this->SP];
                        this->SP--;
                    }
                    break;
            }
            break;
        case 0x1000: // JP NNN - jump to address NNN
            this->PC = opcode.NNN;
            break;
        case 0x2000: // CALL NNN - call subroutine at NNN
            if (this->SP >= STACK_SIZE - 1) {
                // Stack overflow
                // TODO: Error handling
                this->IsHalted = true;
            } else {
                this->SP++; // Increment stack pointer
                this->S[this->SP] = this->PC; // Store current program counter on the stack
                this->PC = opcode.NNN; // Set new program counter
            }
            break;
        case 0x3000: // SE Vx NN - Skip next instruction if Vx == NN
            if (this->V[opcode.X] == opcode.NN) {
                this->PC += 2;
            }
            break;
        case 0x4000: // SNE Vx NN - Skip next instruction if Vx != NN
            if (this->V[opcode.X] != opcode.NN) {
                this->PC += 2;
            }
            break;
        case 0x5000: // SE Vx Vy - Skip next instruction if Vx == Vy
            if (this->V[opcode.X] == this->V[opcode.Y]) {
                this->PC += 2;
            }
            break;
        case 0x6000: // LD Vx NN - Set Vx to NN
            this->V[opcode.X] = opcode.NN;
            break;
        case 0x7000: // ADD Vx NN - Set Vx to Vx + NN
            this->V[opcode.X] += opcode.NN;
            break;
        case 0x8000:
            switch (opcode.opcode & 0xF00F) {
                case 0x8000:
                    this->V[opcode.X] = this->V[opcode.Y];
                    break;
                case 0x8001: // Vx = Vx OR Vy
                    this->V[opcode.X] = (this->V[opcode.X] | this->V[opcode.Y]);
                    break;
                case 0x8002: // Vx = Vx AND Vy
                    this->V[opcode.X] = (this->V[opcode.X] & this->V[opcode.Y]);
                    break;
                case 0x8003: // Vx = Vx XOR Vy
                    this->V[opcode.X] = (this->V[opcode.X] ^ this->V[opcode.Y]);
                    break;
                case 0x8004: // Vx = Vx + Vy, VF = carry
                    this->V[0xF] = (this->V[opcode.X] + this->V[opcode.Y] > 255);
                    this->V[opcode.X] += this->V[opcode.Y];
                    break;
                case 0x8005: // Vx = Vx - Vy, VF = NOT borrow
                    this->V[0xF] = (this->V[opcode.X] > this->V[opcode.Y]);
                    this->V[opcode.X] -= this->V[opcode.Y];
                    break;
                case 0x8006: // Vx = Vx SHR 1
                    this->V[0xF] = (this->V[opcode.X] & 1);
                    this->V[opcode.X] >>= 1;
                    break;
                case 0x8007: // Set Vx = Vy - Vx, VF = NOT borrow.
                    this->V[0xF] = (this->V[opcode.Y] > this->V[opcode.X]);
                    this->V[opcode.X] = this->V[opcode.Y] - this->V[opcode.X];
                    break;
                case 0x800E: // Vx = Vx SHL 1
                    this->V[0xF] = (this->V[opcode.X] >> 7);
                    this->V[opcode.X] <<= 1;
                    break;
                default: // Illegal opcode
                    break;
            }
            break;
        case 0x9000: // SNE Vx, Vy - skip next instruction if Vx != Vy
            if (this->V[opcode.X] != this->V[opcode.Y])
                this->PC += 2;
            break;
        case 0xA000: // LD I, addr - I = NNN
            this->I = opcode.NNN;
            break;
        case 0xB000: // JP V0, addr - Jump to V0 + NNN
            this->PC = this->V[0x0] + opcode.NNN;
            break;
        case 0xC000: // Vx = rand() & NN
            this->V[opcode.X] = (this->m_rng_dist255(this->m_rng)) & opcode.NN;
            break;
        case 0xD000: // Draw at Vx, Vn
            // TODO: Implement
            break;
        case 0xE000:
            switch (opcode.opcode & 0xF0FF) {
                case 0xE09E: // Skip next instruction if key[Vx] is pressed
                    // TODO: Implement
                    break;
                case 0xE0A1: // Skip next instruction if key[Vx] is NOT pressed
                    // TODO: Implement
                    break;
                default: // Illegal opcode
                    break;
            }
            break;
        case 0xF000:
            switch (opcode.opcode & 0xF0FF) {
                case 0xF007: // Set Vx to current delay timer
                    // TODO: Implement
                    break;
                case 0xF00A: // Wait for key press and assign result to Vx
                    // TODO: Implement
                    break;
                case 0xF015: // Set delay timer to Vx
                    // TODO: Implement
                    break;
                case 0xF018: // Set sound timer to Vx
                    // TODO: Implement
                    break;
                case 0xF01E: // I += Vx
                    this->I += this->V[opcode.X];
                    break;
                case 0xF029: // Set I to location of sprite for character in Vx
                    // TODO: Implement
                    break;
                case 0xF033: { // Store BCD of Vx in memory at address I
                    uint8_t value = this->V[opcode.X];
                    for (uint8_t offset = 2; offset >= 0; offset--) {
                        this->Memory[this->I + offset] = value % 10;
                        value /= 10;
                    }
                    break;
                }
                case 0xF055: { // Store V0 to Vx in memory starting at address I
                    for (uint8_t i; i <= opcode.X; i++) {
                        this->Memory[this->I + i] = this->V[i];
                    }
                    break;
                }
                case 0xF065: { // Fill V0 to Vx with values from memory starting at address I
                    for (uint8_t i; i <= opcode.X; i++) {
                        this->V[i] = this->Memory[this->I + i];
                    }
                    break;
                }
                default: // Illegal opcode
                    break;
            }
            break;
        default: // Illegal opcode
            break;
    }
}