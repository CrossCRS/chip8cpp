#include "Emulator.hpp"

using namespace chip8;

Emulator::Emulator() : m_window(sf::VideoMode(1280, 640), "Chip8-CPP", sf::Style::Titlebar | sf::Style::Close), m_cpu(), m_hud(m_cpu) {
    m_window.setFramerateLimit(60);
    ImGui::SFML::Init(m_window);

    this->m_cpu_timer = 0;
    this->m_timers_timer = 0;

    this->m_cpu_clock = 500; // 500Hz, configurable
    this->m_timers_clock = 60; // 60Hz
}

Emulator::~Emulator() = default;

void Emulator::Run() {
    sf::Clock clock;

    while (m_window.isOpen()) {
        sf::Event event;

        while (m_window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                m_window.close();
            }
        }

        sf::Time elapsed = clock.restart();

        ImGui::SFML::Update(m_window, elapsed);
        this->Update(elapsed.asSeconds());

        m_window.clear();
        m_window.draw(this->m_cpu.GetVideoSprite());

        m_hud.draw();
        ImGui::SFML::Render(m_window);
        m_window.display();
    }
    ImGui::SFML::Shutdown();
}

void Emulator::Update(float delta) {
    this->m_cpu_timer += delta;
    this->m_timers_timer += delta;

    while (this->m_cpu_timer > 1.0/this->m_cpu_clock) {
        this->m_cpu.CPUTick();
        this->m_cpu_timer -= 1.0/this->m_cpu_clock;
    }

    while (this->m_timers_timer > 1.0/this->m_timers_clock) {
        this->m_cpu.TimersTick();
        this->m_timers_timer -= 1.0/this->m_timers_clock;
    }
}