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
std::vector<std::stack<char>> createCrateStacks(std::ifstream& file) {
    std::vector<std::stack<char>> crates;
    std::string newLine;
    std::getline(file, newLine);
    while(newLine.length() != 0) {
        std::cout << newLine + " " << int(newLine.length()) << "\n";
        std::getline(file, newLine);
    }
    std::cout << "Done with top section Now the instructions\n"; 
    while (!file.eof()) {
        std::getline(file, newLine);
        std::cout << newLine + " " << int(newLine.length()) << "\n";
    }

    return crates;
}


// Then follow the proceduces -1 and move items using pop and push to stacks.
// At the end pop all the stacks and get the final crates on top as a string
std::string getTopCrates(std::vector<std::stack<char>> crates) {

    return "Crates be here rrrr";
}

int main(int argc, char* argv[]) {
    if (argc < 1) {
        std::cerr << "No Input\n";
        return 1;
    }
    std::ifstream input{argv[1]};
    if (!input) {
        std::cerr << "File Couldn't be opened\n";
    }

    createCrateStacks(input);

    return 0;
}