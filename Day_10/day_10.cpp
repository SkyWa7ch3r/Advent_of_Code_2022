#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct CPU {
    int registerX{1};
    int cycles{0};
};

int checkSignalStrength(CPU cpu) {
    switch (cpu.cycles)
    {
    case 20:
        [[fallthrough]];
    case 60:
        [[fallthrough]];
    case 100:
        [[fallthrough]];
    case 140:
        [[fallthrough]];
    case 180:
        [[fallthrough]];
    case 220:
        return cpu.cycles * cpu.registerX;
    default:
        return 0;
    }
}

std::vector<std::string> getSignalStrength(std::ifstream& file) {
    std::string instruction;
    std::vector<std::string> instructions;
    int signalStrength = 0;
    CPU simpleCPU;
    while(!file.eof()) {
        std::getline(file, instruction);
        instructions.push_back(instruction);
        if (instruction == "noop") {
            simpleCPU.cycles++;
            signalStrength += checkSignalStrength(simpleCPU);
        } else {
            // The other instruction is addx which takes two cycles.
            int toAdd = std::stoi(instruction.substr(instruction.find(' ')));
            for (int i = 0; i < 2; i++) {
                simpleCPU.cycles++;
                if (checkSignalStrength(simpleCPU) != 0) {
                    std::cout << "CPU - Cycles: " << simpleCPU.cycles << " Register: " << simpleCPU.registerX << " Signal Strength: " << checkSignalStrength(simpleCPU) << "\n";
                }
                signalStrength += checkSignalStrength(simpleCPU);
            }
            // Apply toAdd
            simpleCPU.registerX += toAdd;
        }
    }
    std::cout << "CPU - Cycles: " << simpleCPU.cycles << " Register: " << simpleCPU.registerX << " Signal Strength: " << signalStrength << "\n";
    return instructions;
}

void lightUpPixel(std::vector<char>& CRT, CPU cpu) {
    int crtPosition = cpu.cycles - 1;
    // Each 40 Cycles contributes to a new row, work out which row we're on
    // This will adjust the register value appropriately
    int rowOffset = ((cpu.cycles / 40)) * 40;
    int sprite = cpu.registerX + rowOffset;
    if((crtPosition == sprite) || (crtPosition == sprite - 1) || (crtPosition == sprite + 1)) {
        CRT[crtPosition] = '#';
    }
}

void printCRT(std::vector<std::string>& instructions) {
    CPU cpu;
    const int crtSize = 40 * 6;
    // Initialize the CRT
    std::vector<char> CRT(crtSize, '.');
    for(auto instruction : instructions) {
        if(instruction == "noop") {
            cpu.cycles++;
            lightUpPixel(CRT, cpu);
        } else {
            // The other instruction is addx which takes two cycles.
            int toAdd = std::stoi(instruction.substr(instruction.find(' ')));
            for (int i = 0; i < 2; i++) {
                cpu.cycles++;
                lightUpPixel(CRT, cpu);
            }
            // Apply toAdd
            cpu.registerX += toAdd;
        }
    }
    // Print the CRT
    for (int i = 0; i < crtSize; i++) {
        if (i % 40 == 0) {
            std::cout << "\n" << CRT[i];
        } else {
            std::cout << CRT[i];
        }
    }
    std::cout << "\n\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "No Input\n";
        return 1;
    }
    std::ifstream input{argv[1]};
    if (!input) {
        std::cerr << "File Couldn't be opened\n";
    }

    std::vector<std::string> instructions = getSignalStrength(input);

    printCRT(instructions);

    return 0;
}