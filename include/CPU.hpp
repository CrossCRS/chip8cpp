// https://en.wikipedia.org/wiki/CHIP-8
// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.2
#pragma once
#include "Opcode.hpp"

#include <SFML/Graphics.hpp>

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <random>

namespace chip8 {
    class CPU {
        public:
            static const int STACK_SIZE = 16;
            static const int V_REGISTER_SIZE = 16;
            static const int MEMORY_SIZE = 4096;
            static const int MEMORY_PROGRAM_START_ADDRESS = 0x200;
            static const int MEMORY_FONT_START_ADDRESS = 0x50;

            static const int VIDEO_WIDTH = 64;
            static const int VIDEO_HEIGHT = 32;

            static const int KEYPAD_SIZE = 16;

            const sf::Keyboard::Key KEYMAP[KEYPAD_SIZE] = {
                sf::Keyboard::X,
                sf::Keyboard::Num1,
                sf::Keyboard::Num2,
                sf::Keyboard::Num3,
                sf::Keyboard::Q,
                sf::Keyboard::W,
                sf::Keyboard::E,
                sf::Keyboard::A,
                sf::Keyboard::S,
                sf::Keyboard::D,
                sf::Keyboard::Z,
                sf::Keyboard::C,
                sf::Keyboard::Num4,
                sf::Keyboard::R,
                sf::Keyboard::F,
                sf::Keyboard::V,
            };

            std::array<uint8_t, V_REGISTER_SIZE> V; // V0-VF registers
            uint16_t I; // 16-bit address register
            
            uint16_t PC; // 16-bit program counter register
            std::array<uint16_t, STACK_SIZE> S; // 16-bit stack
            uint8_t SP; // 8-bit stack pointer register

            uint8_t DT; // 8-bit delay timer register
            uint8_t ST; // 8-bit sound timer register

            // Need to be C-style arrays to work with SFML and Imgui Mem Editor
            uint8_t memory[MEMORY_SIZE]; // 4KB of 8-bit RAM (BIG ENDIAN)
            uint8_t videoMemory[VIDEO_WIDTH * VIDEO_HEIGHT * 4]; // 64px x 32px video buffer

            uint8_t delayTimer;
            uint8_t soundTimer;

            bool isHalted;

            CPU();

            void loadRomFromFile(const char* filename);
            void reset();
            Opcode decodeOpcode();
            void cpuTick(bool ignoreHalted = false); // Around 500Hz, should be configurable
            void timersTick(bool ignoreHalted = false); // Always 60Hz

            sf::Sprite& getVideoSprite();
        private:
            std::random_device randomDevice;
            std::mt19937 rng;
            std::uniform_int_distribution<> rngDist255;

            sf::Texture videoTexture;
            sf::Sprite sprite;

            void loadFontset();
    };
} // namespace chip8
