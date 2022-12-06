#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stack>

struct Move {
    int numberCrates{1};
    int stackFrom{0};
    int stackTo{1};
};


// Create a vector of stack data structures depending on the number of rows in the top 
// Using vector instead of stack so we put in the elements in the correct order
// We should be able to pop_back and push_back for popping and pushing during instructions
std::vector<std::vector<char>> createCrateStacks(std::ifstream& file) {
    std::string newLine;
    std::getline(file, newLine);
    std::vector<std::vector<char>> crates;
    // Resize the crates by the number of stacks we need
    crates.resize(newLine.length() / 4 + 1);
    while(newLine.length() != 0) {
        // The only easy way I can think to extract the stupid input is takling advantage of the fact the characters are always 4 characters apart
        // This can also probably be done with regex match
        for(int index = 1; index < newLine.length(); index = index + 4) {
            // If it's a capital character (>= 65 in ASCII)
            if (int(newLine[index]) >= 65) {
                // We grab the index / 4, and because insert needs an iterator we tell it where the beginning is
                crates[index / 4].insert(crates[index / 4].begin(), newLine[index]);
            }
        }
        std::getline(file, newLine);
    }

    return crates;
}

// Then follow the proceduces -1 and move items using pop and push to stacks.
// At the end pop all the stacks and get the final crates on top as a string
void craneTheCrates(std::vector<std::vector<char>>& crates, std::ifstream& file, bool craneMaster9001 = false) {
    std::string instruction;
    while(!file.eof()) {
        std::getline(file, instruction);
        Move currentMove{};
        // Extract the moves from instructions
        std::string to = instruction.substr(instruction.rfind(' '), instruction.rfind(' '));
        std::string from = instruction.substr(instruction.rfind("from"), instruction.rfind("from"));
        std::string move = instruction.substr(instruction.find("move"), instruction.find("from"));
        currentMove.numberCrates = stoi(move.substr(move.find(' ') + 1)); 
        currentMove.stackFrom = stoi(from.substr(from.find(' ') + 1)) - 1;
        currentMove.stackTo = stoi(to.substr(to.find(' ') + 1)) - 1;
        // Move every required crate based on instructions
        if (craneMaster9001) {
            int oldFromLength = crates[currentMove.stackFrom].size();
            for (int crate = oldFromLength - currentMove.numberCrates; crate < oldFromLength; crate++) {
                crates[currentMove.stackTo].push_back(crates[currentMove.stackFrom][crate]);
            }
            crates[currentMove.stackFrom].resize(oldFromLength - currentMove.numberCrates);
        } else {
            for (int crate = 0; crate < currentMove.numberCrates; crate++) {
                char popped = crates[currentMove.stackFrom][crates[currentMove.stackFrom].size() - 1];
                crates[currentMove.stackTo].push_back(popped);
                crates[currentMove.stackFrom].pop_back();
            }
        }
    }
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
    std::vector<std::vector<char>> crates = createCrateStacks(input);
    if (argc > 2) {
        craneTheCrates(crates, input, true);
    } else {
        craneTheCrates(crates, input);
    }
    std::cout << "Result: ";
    for (auto crateStack : crates) {
        std::cout << crateStack.back();
    }
    std::cout << "\n";

    return 0;
}