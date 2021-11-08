#include "Emulator.hpp"
#include <cstdio>

int main() {
    chip8::Emulator emulator;
    emulator.run();
    return 0;
}