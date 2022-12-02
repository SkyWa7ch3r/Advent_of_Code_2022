#include <iostream>
#include <fstream>
#include <string>
#include <map>

// A map of all the result combinations for Rock Paper Scissors
std::map<std::string, std::string> GameCombos {
    // Opponent chooses Rock
    {"A X", "DRAW"},
    {"A Y", "WIN"},
    {"A Z", "LOSE"},
    // Opponent chooses Paper
    {"B X", "LOSE"},
    {"B Y", "DRAW"},
    {"B Z", "WIN"},
    // Opponent chooses Scissors
    {"C X", "WIN"},
    {"C Y", "LOSE"},
    {"C Z", "DRAW"}
};

// For the second puzzle we got told the second character was the strategy
std::map<char, std::string> Strategy {
    {'X', "LOSE"},
    {'Y', "DRAW"},
    {'Z', "WIN"},
};

// Match the scores
std::map<char, int> ChoiceScore {
    {'X', 1}, // Rock
    {'Y', 2}, // Paper
    {'Z', 3}, // Scissors
};

// Match the scores to result
std::map<std::string, int> ResultScore {
    {"WIN", 6}, 
    {"DRAW", 3}, 
    {"LOSE", 0}, 
};

/**
 * Finds the combo that receives a strategy and opponents move, and finds the desired move combination for us
*/
std::string findCombo(const std::map<std::string, std::string> & combos, std::string_view strategy, char move) {
    for (auto pair: combos) {
        if (pair.first.find(move) != std::string::npos && pair.second == strategy) {
            return pair.first;
        }
    }
    return "Not found"; // Should never occur
}

int main(int argc, char* argv[]) {
    if (argc < 1) {
        std::cerr << "No Input\n";
        return 1;
    }
    std::ifstream input{argv[1]};
    if (!input) {
        std::cerr << "File Couldn't be opened";
    }
    int first_puzzle_score = 0;
    int second_puzzle_score = 0;
    // Need to do eof otherwise it goes to it finds the EOF character which gets counted as a line
    while (!input.eof()) {
        std::string round;
        std::getline(input, round);
        first_puzzle_score += ResultScore[GameCombos[round]] + ChoiceScore[round[2]];
        std::string strat {Strategy[round[2]]};
        std::string desiredCombo {findCombo(GameCombos, strat, round[0])};
        second_puzzle_score += ResultScore[strat] + ChoiceScore[desiredCombo[2]];
    }
    
    std::cout << "Your first strategy gave you a score of " << first_puzzle_score << "\n";
    std::cout << "Your second strategy gave you a score of " << second_puzzle_score << "\n";

    return 0;
}