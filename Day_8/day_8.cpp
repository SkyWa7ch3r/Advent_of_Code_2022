#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<std::vector<int>> readGrid(std::ifstream& file) {
    std::string line;
    std::vector<std::vector<int>> grid;
    std::vector<int> currentLine;
    while(!file.eof()) {
        std::getline(file, line);
        for (auto num: line) {
            // 1 in ASCII is 49
            currentLine.push_back(int(num) - 48);
        }
        grid.push_back(currentLine);
        currentLine.clear();
    }
    return grid;
}

int countVisibleTrees(std::vector<std::vector<int>>& grid) {
    int visibleTrees = 0;
    for(int r = 1; r < grid.size() - 1; r++) {
        for(int c = 1; c < grid[0].size() - 1; c++) {
            int tree = grid[r][c];
            // Get Row
            std::vector<int> row = grid[r];
            // Get Column
            std::vector<int> col;
            for(int rc = 0; rc < grid.size(); rc++) {
                col.push_back(grid[rc][c]);
            }
            bool visible = false;
            // Check Left
            visible = tree > *std::max_element(row.begin(), row.begin() + c);
            // Check Right
            visible = visible ||  (tree > *std::max_element(row.begin()+ c + 1, row.end()));
            // Check Above
            visible = visible || (tree > *std::max_element(col.begin(), col.begin() + r));
            // Check Below
            visible = visible || (tree > *std::max_element(col.begin() + r + 1, col.end()));
            // Visible?
            visibleTrees += visible;
        }
    }
    return visibleTrees + (grid[0].size() - 2) * 2 + grid.size() * 2;
}

int getBestTree(std::vector<std::vector<int>>& grid) {
    int maxScenicScore = 0;
    for(int r = 1; r < grid.size() - 1; r++) {
        for(int c = 1; c < grid[0].size() - 1; c++) {
            int tree = grid[r][c];
            std::cout << "Doing Tree [" << r << "][" << c << "]\n";
            // Get Row
            std::vector<int> row = grid[r];
            // Get Column
            std::vector<int> col;
            for(int rc = 0; rc < grid.size(); rc++) {
                col.push_back(grid[rc][c]);
            }
            int leftCounter = 0;
            int rightCounter = 0;
            int aboveCounter = 0;
            int belowCounter = 0;
            // Check Left (Begining to first)
            for (int i = c - 1; i > -1; i--) {
                leftCounter++;
                if (row[i] >= tree) {
                    break;
                }
            }
            // Check Right
            for(int i = c + 1; i < row.size(); i++) {
                rightCounter++;
                if (row[i] >= tree) {
                    break;
                }
            }
            // Check Above
            for (int i = r - 1; i > -1; i--) {
                aboveCounter++;
                if (col[i] >= tree) {
                    break;
                }
            }
            // Check Below
            for(int i = r + 1; i < col.size(); i++) {
                belowCounter++;
                if (col[i] >= tree) {
                    break;
                }
            }
            int scenicScore = leftCounter * rightCounter * belowCounter * aboveCounter;
            // Check against Max Scenic Score
            maxScenicScore = scenicScore > maxScenicScore ? scenicScore : maxScenicScore;
        }
    }

    return maxScenicScore;
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

    std::vector<std::vector<int>> grid = readGrid(input);

    int visibleTrees = countVisibleTrees(grid);

    std::cout << "The Number of trees visible are: " << visibleTrees << "\n";

    int maxScenicScore = getBestTree(grid);

    std::cout << "The Best Scenic Score is: " << maxScenicScore << "\n";
    
    return 0;
}