#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <deque>

int findStartOfPacket(std::string_view packet, int markerSize = 4) {
    int index = 0;
    std::deque<char> marker;
    while(index < packet.length() && marker.size() < markerSize) {
        auto found = std::find(marker.begin(), marker.end(), packet[index]);
        while(found != marker.end()) {
            marker.pop_front();
            found = std::find(marker.begin(), marker.end(), packet[index]);
        }
        marker.push_back(packet[index]);
        index++;
    }
    return index;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "No Input\n";
        return 1;
    }
    for (int arg = 1; arg < argc; arg++) {
        std::ifstream input{argv[arg]};
        if (!input) {
            std::cerr << "File Couldn't be opened\n";
        }
        std::string packet;
        std::getline(input, packet);
        int marker = findStartOfPacket(packet);
        std::cout << "Marker (4 characters) for " << argv[arg] << " was " << marker << "\n";
        int marker14 = findStartOfPacket(packet, 14);
        std::cout << "Marker (14 characters) for " << argv[arg] << " was " << marker14 << "\n";
    }
    
    return 0;
}