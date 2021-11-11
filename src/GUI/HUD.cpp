#include "GUI/HUD.hpp"

using namespace chip8;

HUD::HUD(CPU& cpu) : cpu(cpu) { }

void HUD::draw() {
    ImGui::Begin("Debug", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGui::BeginGroup();
    if (ImGui::Button("Open ROM")) { this->openFileBrowser = true; }
    ImGui::SameLine();
    if (ImGui::Button("Reset")) { this->cpu.reset(); }
    ImGui::SameLine();
    if (ImGui::Button("CPU Tick")) { this->cpu.cpuTick(true); }
    ImGui::SameLine();
    if (ImGui::Button("Timers Tick")) { this->cpu.timersTick(true); }
    ImGui::SameLine();
    ImGui::Checkbox("IsHalted", &this->cpu.isHalted);
    ImGui::EndGroup();

    ImGui::Separator();

    if (ImGui::BeginTabBar("chip8_tabbar", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("Registers")) {
            if (ImGui::BeginTable("registers", 2, ImGuiTableFlags_RowBg)) {
                // PC
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("PC");
                ImGui::TableNextColumn();
                ImGui::TextColored(HUD_TEXT_COLOR(this->cpu.PC), "0x%03X", this->cpu.PC);
                // I
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("I");
                ImGui::TableNextColumn();
                ImGui::TextColored(HUD_TEXT_COLOR(this->cpu.I), "0x%08X", this->cpu.I);
                // SP
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("SP");
                ImGui::TableNextColumn();
                ImGui::TextColored(HUD_TEXT_COLOR(this->cpu.SP), "0x%02X", this->cpu.SP);
                // DT
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("DT");
                ImGui::TableNextColumn();
                ImGui::TextColored(HUD_TEXT_COLOR(this->cpu.DT), "0x%02X", this->cpu.DT);
                // ST
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("ST");
                ImGui::TableNextColumn();
                ImGui::TextColored(HUD_TEXT_COLOR(this->cpu.ST), "0x%02X", this->cpu.ST);
                // V
                for (int n = 0; n < CPU::V_REGISTER_SIZE; n++) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("V%X", n);
                    ImGui::TableNextColumn();
                    ImGui::TextColored(HUD_TEXT_COLOR(this->cpu.V[n]), "0x%02X", this->cpu.V[n]);
                }
                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Stack")) {
            if (ImGui::BeginTable("stack", 2, ImGuiTableFlags_RowBg)) {
                // S
                for (int n = 0; n < CPU::STACK_SIZE; n++) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", n);
                    ImGui::TableNextColumn();
                    ImGui::TextColored(HUD_TEXT_COLOR(this->cpu.S[n]), "0x%03X", this->cpu.S[n]);
                }
                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Memory Editor")) {
            this->memoryEditor.DrawContents(this->cpu.memory, CPU::MEMORY_SIZE);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();

    if (this->openFileBrowser) {
        ImGui::OpenPopup("Open File");
        this->openFileBrowser = false;
    }

    if (this->fileBrowser.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 500), ".ch8")) {
        this->cpu.reset();
        this->cpu.loadRomFromFile(fileBrowser.selected_path.c_str());
    }
}