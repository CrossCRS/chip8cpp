#include <cstdio>
#include <SFML/Graphics.hpp>

#include "CPU.hpp"

namespace chip8 {
    class Emulator {
        public:
            Emulator();
            ~Emulator();
            
            void Run();
        private:
            sf::RenderWindow m_window;
            CPU m_cpu;

            float m_cpu_timer;
            float m_timers_timer;
            float m_cpu_clock;
            float m_timers_clock;

            void Update(float delta);
    };
}
