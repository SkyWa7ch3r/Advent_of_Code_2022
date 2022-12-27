#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>

struct Coordinate {
    int row{0};
    int col{0};
};

struct Path {
    Coordinate current;
    std::vector<Coordinate> path;
};

std::vector<std::vector<char>> readTheGrid(std::ifstream& file, Coordinate& start, Coordinate& end) {
    std::string line;
    std::vector<std::vector<char>> grid;
    int row = 0;
    while(!file.eof()) {
        std::getline(file, line);
        std::vector<char> chars;
        for(int col = 0; col < line.length(); col++) {
            switch (line[col])
            {
                case 'S':
                    chars.push_back('a');
                    start.row = row;
                    start.col = col;
                    break;
                case 'E':
                    chars.push_back('z');
                    end.row = row;
                    end.col = col;
                    break;
                default:
                    chars.push_back(line[col]);
                    break;
            }
        }
        grid.push_back(chars);
        row++;
    }
    return grid;
}

bool checkNextPath(std::vector<std::vector<char>>& grid, std::deque<Path>& paths, Path currentPath, int rowOffset, int colOffset, Coordinate end) {
    if (
        (currentPath.current.row + rowOffset) >= 0 && 
        (currentPath.current.row + rowOffset) < grid.size() &&
        (currentPath.current.col + colOffset) >= 0 && 
        (currentPath.current.col + colOffset) < grid[0].size() && 
        !std::any_of(currentPath.path.begin(), currentPath.path.end(), [currentPath, rowOffset, colOffset](Coordinate prev) { 
            return ((currentPath.current.row + rowOffset) == prev.row && (currentPath.current.col + colOffset) == prev.col);
        })
    ) {
        // Get the difference between current and potential new part of the path
        int diff = grid[currentPath.current.row + rowOffset][currentPath.current.col + colOffset] - grid[currentPath.current.row][currentPath.current.col];
        std::cout << "New Diff: " << diff << "\n";
        if (diff == 1 || diff == 0) {
            std::cout << currentPath.current.row + rowOffset << " " << currentPath.current.col + colOffset;
            std::cout << " " << grid[currentPath.current.row + rowOffset][currentPath.current.col + colOffset] << "\n";
            // Create new Path
            Path newPath;
            Coordinate newCurrent;
            newCurrent.row = currentPath.current.row + rowOffset;
            newCurrent.col = currentPath.current.col + colOffset;
            newPath.current = newCurrent;
            // Update newPaths path
            newPath.path.push_back(currentPath.current);
            // Add the path to the paths queue
            paths.push_back(newPath);
            if(newPath.current.row == end.row && newPath.current.col == end.col) {
                // If we got to the end, break the loop by sending a true back
                return true;
            } else {
                return false;
            }
        }
       
    }
    return false;
}

// Execute a Breadth-First Search (Essentially a BFS, treat each new char as a node)
void getShortestPathToSignal(std::vector<std::vector<char>>& grid, Coordinate& start, Coordinate& end) {
    std::deque<Path> paths;
    std::vector<Coordinate> startPaths{start};
    Path startPath{start, startPaths};
    paths.push_back(startPath);
    while(paths.size()) {
        Path current = paths.front();
        // Check Above (rowOffset = -1, colOffset = 0)
        std::cout << "Checking Above\n";
        bool above = checkNextPath(grid, paths, current, -1, 0, end);
        if (above) {
            break;
        }
        // Check Below (rowOffset = 1, colOffset = 0)
        std::cout << "Checking Below\n";
        bool below = checkNextPath(grid, paths, current, 1, 0, end);
        if (below) {
            break;
        }
        // Check Left (rowOffset = 0, colOffset = -1)
        std::cout << "Checking Left\n";
        bool left = checkNextPath(grid, paths, current, 0, -1, end);
        if (left) {
            break;
        }
        // Check Right (rowOffset = 0, colOffset = 1)
        std::cout << "Checking Right\n";
        bool right = checkNextPath(grid, paths, current, 0, 1, end);
        if (right) { 
            break;
        }
        std::cout << "Number of paths to check: " << paths.size() << "\n";
        // Remove the path from queue of Paths
        paths.pop_front();
        
    }
    std::cout << "The lowest path to the goal was done in " << paths.back().path.size() << "\n";
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

    Coordinate start;
    Coordinate end;

    std::vector<std::vector<char>> grid = readTheGrid(input, start, end);

    getShortestPathToSignal(grid, start, end);

    return 0;
}