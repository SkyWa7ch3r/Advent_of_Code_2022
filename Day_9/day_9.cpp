#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

struct Coordinate {
    int x{0};
    int y{0};
};

double euclideanDist(Coordinate c1, Coordinate c2) {
    return sqrt(std::pow(c1.x - c2.x,2) + std::pow(c1.y - c2.y,2));
}

struct compare
{
    Coordinate key;
    compare(Coordinate &i): key(i) {}
 
    bool operator()(Coordinate &i) {
        return (i.x == key.x && i.y == key.y);
    }
};

void getNewLocation(Coordinate& previousTail, Coordinate& currentTail) {
    if(euclideanDist(currentTail, previousTail) > sqrt(2)) {
        // Get the Distance for x and y
        int xDist = previousTail.x - currentTail.x;
        int yDist =  previousTail.y - currentTail.y;
        // std::cout << xDist << " " << yDist << "\n";
        if (std::abs(xDist) > std::abs(yDist)) {
            // If the x dist is more (we're going left or right)
            currentTail.y = previousTail.y;
            currentTail.x = (xDist > 0) ? previousTail.x - 1 : previousTail.x + 1;
        } else if (std::abs(xDist) == std::abs(yDist)) {
            // Got to move diagonally, got up and down before
            currentTail.x = (xDist > 0) ? previousTail.x - 1 : previousTail.x + 1;
            currentTail.y = (yDist > 0) ? previousTail.y - 1 : previousTail.y + 1;
        } else {
            // Move Up or Down
            currentTail.x = previousTail.x;
            currentTail.y = (yDist > 0) ? previousTail.y - 1 : previousTail.y + 1;
        }
    }
}

void adjustTail(std::vector<Coordinate>& locations, std::vector<Coordinate>& tail) {
    if (tail.size() > 1) {
        // For every tail, adjust by the difference calculated
        Coordinate previousTail = tail[0];
        for(int i = 1; i < tail.size(); i++) {
            // Adjust the current tail
            getNewLocation(previousTail, tail[i]);
            // Set new previous tail 
            previousTail = tail[i];
        }
        if (!std::any_of(locations.begin(), locations.end(), compare(tail.back()))) {
            std::cout << "New Location from Tail " << " X: " << tail.back().x << " Y: " << tail.back().y << "\n";
            locations.push_back(tail.back());
        }
    } else {
        for(auto loc: tail) {
            if (!std::any_of(locations.begin(), locations.end(), compare(loc))) {
                std::cout << "New Location from Tail " << " X: " << loc.x << " Y: " << loc.y << "\n";
                locations.push_back(loc);
            }
        } 
    }
}

void getPointsVisited(std::ifstream& file, int tailSize = 1) {
    std::vector<Coordinate> tailVisited;
    Coordinate head{0,0};
    tailSize -= tailSize > 1;
    std::vector<Coordinate> tail(tailSize);
    tailVisited.push_back(head);
    std::string movement;
    while(!file.eof()) {
        std::getline(file, movement);
        int toMove = std::stoi(movement.substr(movement.find(" ")));
        int headMovements = 0;
        std::cout << "\nMovement: " << movement << "\n";
        while(headMovements < toMove) {
            switch(movement[0]) {
                case 'U':
                    head.y++;
                    break;
                case 'D':
                    head.y--;
                    break;
                case 'L':
                    head.x--;
                    break;
                case 'R':
                    head.x++;
                    break;
            }
            getNewLocation(head, tail[0]);
            adjustTail(tailVisited, tail); 
            headMovements++;
        }
        std::cout << "Current Size: " << tailVisited.size() << "\n";
        for (int i = 0; i < tail.size(); i++) {
            std::cout << "Tail " << i + 1 << " X: " << tail[i].x << " Y: " << tail[i].y << "\n";
        }
    };
    std::cout << "The tail of size " << tailSize << " (excluding the head) visited " << tailVisited.size() << " points.\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "No Input\n";
        return 1;
    }
    std::ifstream input1{argv[1]};
    if (!input1) {
        std::cerr << "File Couldn't be opened\n";
    }

    // getPointsVisited(input1);

    input1.close();

    std::ifstream input2{argv[1]};
    if (!input2) {
        std::cerr << "File Couldn't be opened\n";
    }

    getPointsVisited(input2, 10);

    input2.close();

    return 0;
}