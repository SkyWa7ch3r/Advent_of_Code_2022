#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stack>

// Create a vector of stack data structures depending on the number of rows in the top 
// Then follow the proceduces -1 and move items using pop and push to stacks.
// At the end pop all the stacks and get the final crates on top as a string

int main(int argc, char* argv[]) {
    if (argc < 1) {
        std::cerr << "No Input\n";
        return 1;
    }
    std::ifstream puzzle1{argv[1]};
    if (!puzzle1) {
        std::cerr << "File Couldn't be opened\n";
    }

    return 0;
}