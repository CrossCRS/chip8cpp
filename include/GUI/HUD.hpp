#pragma once
#include "CPU.hpp"
#include "imgui_file_browser.hpp"
#include "imgui_memory_editor.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics.hpp>

#define HUD_TEXT_COLOR(val) (val ? ImVec4(1.0, 1.0, 1.0, 1.0) : ImVec4(0.5, 0.5, 0.5, 1.0))

namespace chip8 {
    class HUD {
        public:
            HUD(CPU& cpu);

            void draw();
        private:
            CPU& cpu;
            MemoryEditor memoryEditor;
            imgui_addons::ImGuiFileBrowser fileBrowser;

            bool openFileBrowser = false;
    };
} // namespace chip8
