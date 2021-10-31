#include <cstdio>
#include "CPU.hpp"

using namespace chip8;

int main() {
    CPU *cpu = new CPU();
    printf("PRESS ENTER TO STEP.\n");

    while(getchar()) {
        cpu->CPUTick();
    }
    return 0;
}