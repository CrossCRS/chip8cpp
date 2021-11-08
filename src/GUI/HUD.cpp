#include "GUI/HUD.hpp"

using namespace chip8;

HUD::HUD(CPU& cpu) : m_cpu(cpu) { }

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
        ImGui::Text("0x%03X", this->m_cpu.PC);
        // I
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("I");
        ImGui::TableNextColumn();
        ImGui::Text("0x%08X", this->m_cpu.I);
        for (int n = 0; n < CPU::V_REGISTER_SIZE; n++) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("V%X", n);
            ImGui::TableNextColumn();
            ImGui::Text("0x%02X", this->m_cpu.V[n]);
        }
        ImGui::EndTable();
    }
    ImGui::End();

    m_memeditor.DrawWindow("Memory", this->m_cpu.Memory, CPU::MEMORY_SIZE);
}