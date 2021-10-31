#include <cstdio>
#include "Emulator.hpp"

int main() {
    chip8::Emulator emulator;
    emulator.Run();
    return 0;
}