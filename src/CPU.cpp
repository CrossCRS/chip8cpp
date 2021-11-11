#include "CPU.hpp"

using namespace chip8;

CPU::CPU() : rng(randomDevice()), rngDist255(0, 255) {
    this->videoTexture.create(VIDEO_WIDTH, VIDEO_HEIGHT);
    this->sprite.setTexture(this->videoTexture);
    this->sprite.setScale(20, 20);

    this->reset();

    this->isHalted = true;
}

sf::Sprite& CPU::getVideoSprite() {
    return this->sprite;
}

void CPU::reset() {
    this->V.fill(0);
    this->I = 0;

    this->PC = 0x200; // Programs start at 0x200
    this->S.fill(0);
    this->SP = 0;

    this->DT = 0;
    this->ST = 0;

    memset(this->memory, 0, sizeof(this->memory));
    memset(this->videoMemory, 0, sizeof(this->videoMemory));

    this->delayTimer = 0;
    this->soundTimer = 0;

    this->loadFontset();

    this->isHalted = false;

}

Opcode CPU::decodeOpcode() {
    // Get two opcode bytes from memory
    uint16_t opcode = (this->memory[this->PC] << 8) | this->memory[this->PC+1];
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

void CPU::loadFontset() {
    const uint8_t FONTSET_SIZE = 80;

    std::array<uint8_t, FONTSET_SIZE> fontset =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    for (uint8_t i = 0; i < FONTSET_SIZE; i++) {
        this->memory[MEMORY_FONT_START_ADDRESS + i] = fontset[i];
    }
}

void CPU::loadRomFromFile(const char* filename) {
    this->isHalted = true;

	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (file.is_open()) {
		std::streampos size = file.tellg();
		char* buffer = new char[size];

		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		for (unsigned int i = 0; i < size; i++)	{
			this->memory[this->MEMORY_PROGRAM_START_ADDRESS + i] = buffer[i];
		}

		delete[] buffer;
    }

    this->isHalted = false;
}

void CPU::cpuTick(bool ignoreHalted) {
    if (this->isHalted && !ignoreHalted) {
        return;
    }

    Opcode opcode = this->decodeOpcode();

    this->PC += 2;

    switch (opcode.opcode & 0xF000) {
        case 0x0000:
            switch (opcode.opcode) {
                case 0x00E0: // CLS - clear screen
                    memset(this->videoMemory, 0, sizeof(this->videoMemory));
                    this->videoTexture.update(this->videoMemory);
                    break;
                case 0x00EE: // RET - return from subroutine
                    if (this->SP == 0) {
                        // Stack underflow
                        // TODO: Error handling
                        this->isHalted = true;
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
                this->isHalted = true;
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
            if (this->V[opcode.X] != this->V[opcode.Y]) {
                this->PC += 2;
            }
            break;
        case 0xA000: // LD I, addr - I = NNN
            this->I = opcode.NNN;
            break;
        case 0xB000: // JP V0, addr - Jump to V0 + NNN
            this->PC = this->V[0x0] + opcode.NNN;
            break;
        case 0xC000: // Vx = rand() & NN
            this->V[opcode.X] = (this->rngDist255(this->rng)) & opcode.NN;
            break;
        case 0xD000: { // Draw at Vx, Vn
            uint8_t x = this->V[opcode.X];
            uint8_t y = this->V[opcode.Y];

            this->V[0xF] = 0; // Reset the "collision" flag

            for (uint8_t row = 0; row < opcode.N; row++) {
                uint8_t sprite_byte = this->memory[this->I + row];

                for (uint8_t col = 0; col < 8; col++) {
                    uint8_t sprite_pixel = sprite_byte & (0x80 >> col);
                    // Cast every 4 uint8_t to 1 uint32_t RGBA pixel
                    auto screen_pixel = (uint32_t*)&this->videoMemory[((y + row) * VIDEO_WIDTH + (x + col)) * 4];

                    if (sprite_pixel) {
                        if (*screen_pixel == 0xFFFFFFFF) {
                            this->V[0xF] = 1; // Set VF for collision detection
                        }

                        *screen_pixel ^= 0xFFFFFFFF; // XOR with 0xFFFFFFFF because we're only using monochrome
                    }
                }
            }

            this->videoTexture.update(this->videoMemory);
            break;
        }
        case 0xE000:
            switch (opcode.opcode & 0xF0FF) {
                case 0xE09E: // Skip next instruction if key[Vx] is pressed
                    if (sf::Keyboard::isKeyPressed(this->KEYMAP[this->V[opcode.X]])) {
                        this->PC += 2;
                    }
                    break;
                case 0xE0A1: // Skip next instruction if key[Vx] is NOT pressed
                    if (!sf::Keyboard::isKeyPressed(this->KEYMAP[this->V[opcode.X]])) {
                        this->PC += 2;
                    }
                    break;
                default: // Illegal opcode
                    break;
            }
            break;
        case 0xF000:
            switch (opcode.opcode & 0xF0FF) {
                case 0xF007: // Set Vx to current delay timer
                    this->V[opcode.X] = this->delayTimer;
                    break;
                case 0xF00A: {// Wait for key press and assign result to Vx
                    bool pressed = false;
                    for (int i = 0; i <= this->KEYPAD_SIZE; i++) {
                        if (sf::Keyboard::isKeyPressed(this->KEYMAP[i])) {
                            this->V[opcode.X] = i;
                            pressed = true;
                        }
                    }

                    if (!pressed) {
                        this->PC -= 2; // Repeat the instruction
                    }
                    break;
                }
                case 0xF015: // Set delay timer to Vx
                    this->delayTimer = this->V[opcode.X];
                    break;
                case 0xF018: // Set sound timer to Vx
                    this->soundTimer = this->V[opcode.X];
                    break;
                case 0xF01E: // I += Vx
                    this->I += this->V[opcode.X];
                    break;
                case 0xF029: // Set I to location of sprite for character in Vx
                    this->I = MEMORY_FONT_START_ADDRESS + (this->V[opcode.X] * 5);
                    break;
                case 0xF033: { // Store BCD of Vx in memory at address I
                    uint8_t value = this->V[opcode.X];
                    for (int offset = 2; offset >= 0; offset--) {
                        this->memory[this->I + offset] = value % 10;
                        value /= 10;
                    }
                    break;
                }
                case 0xF055: { // Store V0 to Vx in memory starting at address I
                    for (int i = 0; i <= opcode.X; i++) {
                        this->memory[this->I + i] = this->V[i];
                    }
                    break;
                }
                case 0xF065: { // Fill V0 to Vx with values from memory starting at address I
                    for (int i = 0; i <= opcode.X; i++) {
                        this->V[i] = this->memory[this->I + i];
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

void CPU::timersTick(bool ignoreHalted) {
    if (this->isHalted && !ignoreHalted) {
        return;
    }
    
    if (this->delayTimer > 0) { 
        this->delayTimer--;
    }

    if (this->soundTimer > 0) {
        // TODO: Sound
        this->soundTimer--;
    }
}