#include "GUI/HUD.hpp"

using namespace chip8;

HUD::HUD(CPU& cpu) : cpu(cpu) { }

void HUD::draw() {
    ImGui::Begin("CPU");

    // Display contents in a scrolling region
    ImGui::TextColored(ImVec4(1,1,0,1), "Registers");
    if (ImGui::BeginTable("registers", 2)) {
        // PC
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("PC");
        ImGui::TableNextColumn();
        ImGui::Text("0x%03X", this->cpu.PC);
        // I
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("I");
        ImGui::TableNextColumn();
        ImGui::Text("0x%08X", this->cpu.I);
        for (int n = 0; n < CPU::V_REGISTER_SIZE; n++) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("V%X", n);
            ImGui::TableNextColumn();
            ImGui::Text("0x%02X", this->cpu.V[n]);
        }
        ImGui::EndTable();
    }
    ImGui::End();

    this->memoryEditor.DrawWindow("Memory", this->cpu.memory, CPU::MEMORY_SIZE);
}