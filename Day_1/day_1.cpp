#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

struct Elf
{
    int sum {0};
    int id {0};
};

bool greater(Elf a, Elf b) {
    return (a.sum > b.sum);
}

// Get sorted vector of elves
void getTopFourElves(std::ifstream& file, std::vector<Elf>& elves) {
    int currentCalories = 0;
    int elfID = 0;
    while (file) {
        std::string food_item;
        std::getline(file, food_item);
        if (food_item != "") {
            currentCalories += std::stoi(food_item);
        } else {
            elves[3].id = elfID;
            elves[3].sum = currentCalories;
            std::sort(elves.begin(), elves.end(), greater);
            currentCalories = 0;
            elfID++;
        }
    }
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
    std::vector<Elf> elves(4);
    getTopFourElves(input, elves);
    std::cout << "The Elf with the most calories was Elf Number " << elves[0].id << " with " << elves[0].sum << " calories.\n";
    int topThreeSum = 0;
    for (int i = 0; i < 3; i++) {
        topThreeSum += elves[i].sum;
    }
    std::cout << "The sum of the calories from the top 3 elves was " << topThreeSum << " calories.\n";
    
}