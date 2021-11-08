#pragma once
#include "CPU.hpp"
#include "imgui_memory_editor.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics.hpp>

namespace chip8 {
    class HUD {
        public:
            HUD(CPU& cpu);

            void draw();
        private:
            CPU& cpu;
            MemoryEditor memoryEditor;
    };
} // namespace chip8
