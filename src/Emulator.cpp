#include "Emulator.hpp"

namespace chip8 {
    Emulator::Emulator() : window(sf::VideoMode(1280, 640), "Chip8-CPP", sf::Style::Titlebar | sf::Style::Close), cpu(), hud(cpu) {
        window.setFramerateLimit(60);
        ImGui::SFML::Init(window);

        this->cpuTimer = 0;
        this->timersTimer = 0;

        this->cpuClock = 500; // 500Hz, configurable
        this->timersClock = 60; // 60Hz
    }

    void Emulator::run() {
        sf::Clock clock;

        while (window.isOpen()) {
            sf::Event event;

            while (window.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(event);

                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            sf::Time elapsed = clock.restart();

            ImGui::SFML::Update(window, elapsed);
            this->update(elapsed.asSeconds());

            window.clear();
            window.draw(this->cpu.getVideoSprite());

            hud.draw();
            ImGui::SFML::Render(window);
            window.display();
        }
        ImGui::SFML::Shutdown();
    }

    void Emulator::update(float delta) {
        this->cpuTimer += delta;
        this->timersTimer += delta;

        while (this->cpuTimer > 1.0F / this->cpuClock) {
            this->cpu.cpuTick();
            this->cpuTimer -= 1.0F / this->cpuClock;
        }

        while (this->timersTimer > 1.0F / this->timersClock) {
            this->cpu.timersTick();
            this->timersTimer -= 1.0F / this->timersClock;
        }
    }
} // namespace chip8