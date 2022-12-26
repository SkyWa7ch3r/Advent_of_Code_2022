#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <gmpxx.h>
#include <math.h>
#include <climits>

// Compile with g++ -lgmpxx -lgmp -I<conda-include> -L<conda-lib> day_11.cpp -o day_11.o
// Takes forever still, gotta apply Lowest Commnon Denominator from all

// const 

class Monkey {
    // Need to change all int to types that can handle large numbers
    // long long uint wasn't large enough, got overflows, now using the GMP library
    std::vector<mpz_class> items;
    int opt;
    bool multiply;
    int test;
    int trueMonkeyIdx{0};
    int falseMonkeyIdx{1};
    int itemsInspected{0};
    int worryDivider{3};
    mpz_class LCM{1};
    public:
        Monkey(std::vector<mpz_class> newItems, int newOpt, bool newMultiply, int newTest, int newTrueM, int newFalseM) :
            items{ newItems }, opt{ newOpt }, multiply{ newMultiply }, test{ newTest }, trueMonkeyIdx{ newTrueM }, falseMonkeyIdx{ newFalseM } {}
        
        int getItemsInspected() {
            return itemsInspected;
        }

        int getTest() {
            return test;
        }

        void setWorryDivider(int newDiv) {
            worryDivider = newDiv;
        }

        // Add item to the stack of items
        void addItem(mpz_class item) {
            items.push_back(item);
        }

        void setLCM(mpz_class newLCM) {
            LCM = newLCM;
        }

        // Adjust the item's worry
        mpz_class inspect(mpz_class item) {
            itemsInspected++;
            if(multiply) {
                // Multiply item by opt
                // Doing this in a ternary gave syntax error?
                if (opt > 0) {
                    return item * opt;
                } else {
                    return item * item;
                }
            } else {
                // We add opt to item
                if (opt > 0) {
                    return item + opt;
                } else {
                    return item + item;
                }
            }
        }

        void inspectAndTestItems(std::vector<Monkey>& monkeys) {
            while(!items.empty()) {
                // Grab the item
                mpz_class currentItem = items.back();
                // Inspect the item
                currentItem = inspect(currentItem);
                if (currentItem > __INT64_MAX__) {
                    currentItem = currentItem % LCM;
                }
                //  Decrease worry level
                currentItem = currentItem / worryDivider;
                // Test the item and pass to another monkey
                (currentItem % test) == 0 ? monkeys[trueMonkeyIdx].addItem(currentItem) : monkeys[falseMonkeyIdx].addItem(currentItem);
                // Remove item after inspection
                items.pop_back();
            }
        }

        void print() {
            std::cout << items.size() <<" Items: " ;
            for(mpz_class item: items) {
                std::cout << item << (item == items.back() ? "\n" : ", ");
            }
            std::cout << "Operator: new = old " << (multiply ? " * " : " + ") << opt << "\n";
            std::cout << "Worry Divider: " << worryDivider << "\n";
            std::cout << "Test: Divide By " << test << "\n";
            std::cout << "Test Pass: Item to Monkey " << trueMonkeyIdx << "\n";
            std::cout << "Test Fail: Item to Monkey " << falseMonkeyIdx << "\n";
            std::cout << "Items Inspected: " << itemsInspected << "\n\n";
        }
};

std::vector<Monkey> readMonkeyFile(std::ifstream& file) {
    std::string line;
    std::vector<Monkey> monkeys;
    std::getline(file, line);
    mpz_class LCM{1};
    while(!file.eof()) {
        // Get Item Line
        std::getline(file, line);
        // Get the items
        std::string itemString = line.substr(line.find(':') + 2);
        std::vector<mpz_class> items;
        while(!itemString.empty()) {
            if (itemString.find(',') == itemString.npos) {
                mpz_class item{itemString};
                items.push_back(item);
                itemString.clear();
            } else {
                // Get the item worry level and push it onto the item stack
                mpz_class item{itemString.substr(itemString.rfind(',') + 2)};
                items.push_back(item);
                // Erase the item from the string
                itemString.erase(itemString.begin() + itemString.rfind(','), itemString.end());
            }
        }
        // Get Operation Line
        std::getline(file, line);
        bool multiply;
        int opt{0};
        if(line.find('*') == line.npos) {
            multiply = false;
            if(line.substr(line.find('*') + 2) == "old") {
                opt = -1;
            } else {
                opt = std::stoi(line.substr(line.find('+') + 2));
            }
            
        } else {
            multiply = true;
            if(line.substr(line.find('*') + 2) == "old") {
                opt = -1;
            } else {
                opt = std::stoi(line.substr(line.find('*') + 2));
            }
            
        }
        // Get Test Line
        std::getline(file, line);
        int test = std::stoi(line.substr(line.find("by") + 3));
        // Multiply LCM by test, since all tests are prime
        LCM *= test;
        // Get True Monkey Line
        std::getline(file, line);
        int trueMonkey = std::stoi(line.substr(line.rfind(' ') + 1));
        // Get False Monkey Line
        std::getline(file, line);
        int falseMonkey = std::stoi(line.substr(line.rfind(' ') + 1));
        // Create Monkey Instance and add it to list of Monkeys
        Monkey newMonkey{items, opt, multiply, test, trueMonkey, falseMonkey};
        monkeys.push_back(newMonkey);
        newMonkey.print();
        // Set the line marker to start of next Monkey
        std::getline(file, line);
        std::getline(file, line);
    }

    for (Monkey &monkey: monkeys) {
        monkey.setLCM(LCM);
    }

    return monkeys;
}

void getTotalInspections(std::vector<Monkey>& monkeys, int rounds = 20) {
    for (int i = 0; i < rounds; i++) {
        if(i % 100 == 0) {
            std::cout << "Round " << i << "\n";
        }
        for(int j = 0; j < monkeys.size(); j++) {
            // if((i == 20) || ((i % 1000) == 0)) {
            //     std::cout << "Monkey " << j << "\n";
            //     monkeys[j].print();
            // }
            monkeys[j].inspectAndTestItems(monkeys);
        }
    };
    std::sort(monkeys.begin(), monkeys.end(), [](Monkey a, Monkey b) { return a.getItemsInspected() > b.getItemsInspected(); });
    mpz_class monkey1{monkeys[0].getItemsInspected()};
    mpz_class monkey2{monkeys[1].getItemsInspected()};
    mpz_class result{monkey1 * monkey2};
    std::cout << "The top two Monkeys were: " << monkeys[0].getItemsInspected() << " & " << monkeys[1].getItemsInspected() << ".\nThe total monkey business is: " <<  result << "\n";
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

    std::vector<Monkey> monkeys = readMonkeyFile(input);

    input.close();

    std::vector<Monkey> moreWorrisomeMonkeys;

    for(Monkey monkey: monkeys) {
        moreWorrisomeMonkeys.push_back(monkey);
        moreWorrisomeMonkeys.back().setWorryDivider(1);
    }

    getTotalInspections(monkeys);

    getTotalInspections(moreWorrisomeMonkeys, 10000);

    return 0;
}