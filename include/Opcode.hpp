#pragma once
#include <cstdint>

namespace chip8 {
    struct Opcode {
        uint16_t opcode;
        uint16_t NNN; // Address
        uint8_t NN; // 8-bit constant
        uint8_t N; // 4-bit constant
        uint8_t X, Y; // 4-bit register identifiers
    };
} // namespace chip8
