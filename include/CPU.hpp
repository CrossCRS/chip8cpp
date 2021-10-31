// https://en.wikipedia.org/wiki/CHIP-8
// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.2
#pragma once
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <random>
#include "Opcode.hpp"

#define STACK_SIZE 16
#define RAM_SIZE 4096

namespace chip8 {
    class CPU {
        public:
            uint8_t V[16]; // V0-VF registers
            uint16_t I; // 16-bit address register
            
            uint16_t PC; // 16-bit program counter register
            uint16_t S[STACK_SIZE]; // 16-bit stack
            uint8_t SP; // 8-bit stack pointer register

            uint8_t DT; // 8-bit delay timer register
            uint8_t ST; // 8-bit sound timer register

            uint8_t Memory[RAM_SIZE]; // 4KB of 8-bit RAM (BIG ENDIAN)

            bool IsHalted;

            CPU();
            ~CPU();

            void Reset();
            Opcode DecodeOpcode();
            void Step();
        private:
            std::random_device m_random_device;
            std::mt19937 m_rng;
            std::uniform_int_distribution<> m_rng_dist255;
    };
}
