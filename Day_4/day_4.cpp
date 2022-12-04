#include <fstream>
#include <iostream>
#include <string>
#include <array>

std::array<int, 2> getSectionRange(const std::string elf) {
    int dashIndex = elf.find('-');
    std::array<int, 2> sections;
    sections[0] = std::stoi(elf.substr(0, 0 + dashIndex));
    sections[1] = std::stoi(elf.substr(dashIndex + 1));
    return sections;
}

int findDuplicateWork(std::ifstream& file, bool noOverlap = false) {
    int sectionPairs = 0;
    while (!file.eof()) {
        std::string sections;
        std::getline(file, sections);
        std::string elf1 = sections.substr(0, sections.find(','));
        std::string elf2 = sections.substr(sections.find(',') + 1);
        std::array<int, 2> elf1Range = getSectionRange(elf1);
        std::array<int, 2> elf2Range = getSectionRange(elf2);
        // // If either elf has a range higher than the other, add to section pairs
        if (!noOverlap) {
            if((elf1Range[0] <= elf2Range[0] && elf1Range[1] >= elf2Range[1]) || (elf2Range[0] <= elf1Range[0] && elf2Range[1] >= elf1Range[1])) {
                sectionPairs++;
            }
        } else {
            if(
                // Check if either elf has a greater minimum and a lesser maximum
                (
                    (elf1Range[0] >= elf2Range[0] && elf1Range[1] <= elf2Range[1]) ||
                    (elf2Range[0] >= elf1Range[0] && elf2Range[1] <= elf1Range[1])
                ) 
                    || //OR
                // If an elf has a lesser maximum than the other minimum while still having less maximum than the other maximum
                (
                    (elf1Range[1] >= elf2Range[0] && elf1Range[1] <= elf2Range[1]) ||
                    (elf2Range[1] >= elf1Range[0] && elf2Range[1] <= elf1Range[1])
                )
            ) {
                sectionPairs++;
            }
        }
    }

    return sectionPairs;
}

int main(int argc, char* argv[]) {
    if (argc < 1) {
        std::cerr << "No Input\n";
        return 1;
    }
    std::ifstream puzzle1{argv[1]};
    if (!puzzle1) {
        std::cerr << "File Couldn't be opened\n";
    }
    int result1 = findDuplicateWork(puzzle1);
    std::cout << "The number of pairs where one Elf's work is completely contained by the other is: " << result1 << "\n";
    std::ifstream puzzle2{argv[1]};
    if (!puzzle2) {
        std::cerr << "File Couldn't be opened\n";
    }
    int result2 = findDuplicateWork(puzzle2, true);
    std::cout << "The number of pairs where one Elf's work is completely contained by the other with any overlap is: " << result2 << "\n";

    return 0;
}

