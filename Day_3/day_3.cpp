#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

int getCharIndex(char character) {
    // 1 to 26 == a to z
    // 27 to 52 == A to Z
    // Chars are ASCII integers
    int capitalToIndex = 38; // A - 38 = 27
    int lowerToIndex = 96; // a - 96 = 1
    if (character <= 90) {
        return character - capitalToIndex;
    } else {
        return character - lowerToIndex;
    }
}

int getPrioritySum(std::ifstream& input) {
    int prioritySum = 0;
    while (!input.eof()) {
        std::string rucksack;
        std::getline(input, rucksack);
        std::string compartment1 = rucksack.substr(0, rucksack.length() / 2);
        std::string compartment2 = rucksack.substr(rucksack.length() / 2);
        // set_intersection needs to the arrays to be sorted, might be faster to load into a set
        std::sort(compartment1.begin(), compartment1.end());
        std::sort(compartment2.begin(), compartment2.end());
        // Initialize the vector to store items, run set_intersection
        std::vector<char> common;
        std::set_intersection(compartment1.begin(), compartment1.end(), 
                              compartment2.begin(), compartment2.end(), 
                              std::back_inserter(common));
        // Add the common char
        prioritySum += getCharIndex(common[0]);
    }

    return prioritySum;
}

int findGroupPriority(std::ifstream& input) {
    int groupIndex = 0;
    std::string group[3];
    int prioritySum = 0;
    while(input) {
        std::string rucksack;
        std::getline(input, rucksack);
        // Get the common item type
        if (groupIndex == 3) {
            std::vector<char> common;
            std::vector<char> badge;
            // Get the common between the first two sets
            std::set_intersection(group[0].begin(), group[0].end(), 
                                  group[1].begin(), group[1].end(), 
                                  std::back_inserter(common));
            // Get the badge through intersection between this common array and last rucksack
            std::set_intersection(common.begin(), common.end(), 
                                  group[2].begin(), group[2].end(), 
                                  std::back_inserter(badge));
            // Add to priority sum based off badges
            prioritySum += getCharIndex(badge[0]);
            // Reset the group array
            groupIndex = 0;
            std::fill_n(group, 3, "");
        }
        // Sort ready for set_interaction
        std::sort(rucksack.begin(), rucksack.end());
        group[groupIndex] = rucksack;
        groupIndex++;
    }

    return prioritySum;
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
    int priority1 = getPrioritySum(puzzle1);

    std::cout << "Your priority sum for puzzle 1 was " << priority1 << "\n";

    std::ifstream puzzle2{argv[1]};
    if (!puzzle2) {
        std::cerr << "File Couldn't be opened\n";
    }
    int priority2 = findGroupPriority(puzzle2);

    std::cout << "Your priority sum for puzzle 2 was " << priority2 << "\n";

    return 0;
}