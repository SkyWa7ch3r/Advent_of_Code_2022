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

bool checkNextPath(std::vector<std::vector<char>>& grid, std::deque<Path>& paths, Path& currentPath, int rowOffset, int colOffset, Coordinate& end, std::vector<Coordinate>& visited) {
    int newRow = currentPath.current.row + rowOffset;
    int newCol = currentPath.current.col + colOffset;
    if (
        newRow >= 0 && 
        newRow < grid.size() &&
        newCol >= 0 && 
        newCol < grid[0].size() &&
        !std::any_of(visited.begin(), visited.end(), [newRow, newCol](Coordinate prev) { 
            return (newRow == prev.row && newCol == prev.col);
        })
    ) {
        // Get the difference between current and potential new part of the path
        int diff = grid[newRow][newCol] - grid[currentPath.current.row][currentPath.current.col];
        if (diff <= 1) {
            // Create new Path
            Path newPath;
            Coordinate newCurrent;
            newCurrent.row = newRow;
            newCurrent.col = newCol;
            newPath.current = newCurrent;
            // Update newPaths path
            newPath.path = currentPath.path;
            newPath.path.push_back(currentPath.current);
            // Add the path to the paths queue
            paths.push_back(newPath);
            // Also add coordinate to visited
            visited.push_back(newPath.current);
            if((newPath.current.row == end.row) && (newPath.current.col == end.col)) {
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
std::vector<Coordinate> getShortestPathToSignal(std::vector<std::vector<char>>& grid, Coordinate& start, Coordinate& end) {
    std::deque<Path> paths;
    std::vector<Coordinate> startPaths{};
    Path startPath{start, startPaths};
    std::vector<Coordinate> visited{start};
    paths.push_back(startPath);
    while(paths.size()) {
        Path current = paths.front();
        paths.pop_front();
        // Check Above (rowOffset = -1, colOffset = 0)
        bool above = checkNextPath(grid, paths, current, -1, 0, end, visited);
        if (above) {
            return paths.back().path;
        }
        // Check Below (rowOffset = 1, colOffset = 0)
        bool below = checkNextPath(grid, paths, current, 1, 0, end, visited);
        if (below) {
            return paths.back().path;
        }
        // Check Left (rowOffset = 0, colOffset = -1)
        bool left = checkNextPath(grid, paths, current, 0, -1, end, visited);
        if (left) {
            return paths.back().path;
        }
        // Check Right (rowOffset = 0, colOffset = 1)
        bool right = checkNextPath(grid, paths, current, 0, 1, end, visited);
        if (right) { 
            return paths.back().path;
        }
    }
    return startPaths;
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

    std::cout << "Start X: " << start.col << " Y: " << start.row << "\n";
    std::cout << "End X: " << end.col << " Y: " << end.row << "\n";

    std::vector<Coordinate> shortestPath = getShortestPathToSignal(grid, start, end);

    std::cout << "The lowest path from the designated start to the goal was done in " << shortestPath.size() << "\n";

    int minShortestPath = grid.size() * grid[0].size() + 1;
    for (int row = 0; row < grid.size(); row++) {
        for (int col = 0; col < grid[0].size(); col++) {
            if(grid[row][col] == 'a') {
                Coordinate newStart{row, col};
                std::vector<Coordinate> shortestPath = getShortestPathToSignal(grid, newStart, end);
                if (shortestPath.size() < minShortestPath && shortestPath.size() != 0) {
                    std::cout << "New Shortest Path: " << shortestPath.size() << "\n";
                    minShortestPath = shortestPath.size();
                }
            }
        }
    }

    std::cout << "The shortest path from any a point is: " << minShortestPath << "\n";

    return 0;
}