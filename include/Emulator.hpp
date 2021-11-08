#pragma once
#include "GUI/HUD.hpp"
#include "CPU.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics.hpp>

#include <cstdio>

namespace chip8 {
    class Emulator {
        public:
            Emulator();
            
            void run();
        private:
            sf::RenderWindow window;
            CPU cpu;
            HUD hud;

            float cpuTimer;
            float timersTimer;
            float cpuClock;
            float timersClock;

            void update(float delta);
    };
} // namespace chip8
