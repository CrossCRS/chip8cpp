// https://en.wikipedia.org/wiki/CHIP-8
// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.2
#pragma once
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <random>
#include <SFML/Graphics.hpp>
#include "Opcode.hpp"

namespace chip8 {
    class CPU {
        static const uint STACK_SIZE = 16;
        static const uint MEMORY_SIZE = 4096;
        static const uint MEMORY_PROGRAM_START_ADDRESS = 0x200;
        static const uint MEMORY_FONT_START_ADDRESS = 0x50;

        static const uint VIDEO_WIDTH = 64;
        static const uint VIDEO_HEIGHT = 32;

        public:
            uint8_t V[16]; // V0-VF registers
            uint16_t I; // 16-bit address register
            
            uint16_t PC; // 16-bit program counter register
            uint16_t S[STACK_SIZE]; // 16-bit stack
            uint8_t SP; // 8-bit stack pointer register

            uint8_t DT; // 8-bit delay timer register
            uint8_t ST; // 8-bit sound timer register

            uint8_t Memory[MEMORY_SIZE]; // 4KB of 8-bit RAM (BIG ENDIAN)

            uint8_t DelayTimer;
            uint8_t SoundTimer;

            uint8_t Video[VIDEO_WIDTH * VIDEO_HEIGHT * 4];

            bool IsHalted;

            CPU();
            ~CPU();

            void Reset();
            Opcode DecodeOpcode();
            void CPUTick(); // Around 500Hz, should be configurable
            void TimersTick(); // Always 60Hz

            sf::Sprite& GetVideoSprite();
        private:
            std::random_device m_random_device;
            std::mt19937 m_rng;
            std::uniform_int_distribution<> m_rng_dist255;

            sf::Texture m_video_texture;
            sf::Sprite m_sprite;

            void LoadFontset();
    };
}
