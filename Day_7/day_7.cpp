#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

struct File {
    int size{0};
    std::string name;
};

// In this case we're assuming a dir can have multiple files, multiple children dirs
// ONLY ONE parent (no symbolic links here!)
class Dir {
    std::string name{""};
    int size{0};
    std::vector<File> files;
    std::vector<int> children;
    int parent{0};
    public:
        // Constructors
        Dir(std::string newName) : name{newName} {}
        Dir(std::string newName, int newParent) : name{newName}, parent{newParent} {}

        // Add a file to the dir and get the size of the dir
        void addFile(std::string fileName, int fileSize) {
            File newFile{fileSize, fileName};
            size += fileSize; 
            files.push_back(newFile);
        }

        // set the parent
        void setParent(int newParent) {
            parent = newParent;
        }

        // Get dir name
        std::string getDirName() {
            return name;
        }

        std::vector<File> getFiles() {
            return files;
        }

        std::vector<int> getChildren() {
            return children;
        }

        // Get the size
        int getSize() {
            return size;
        }

        void addChild(int dir) {
            children.push_back(dir);
        }

        int getParent() {
            return parent;
        }

        // Take all the dirs and add them to the size (This should be called after reaching the end of the file)
        void addAllDirs(std::vector<Dir>& fs) {
            for (auto child: children) {
                size += fs[child].getSize();
            }
        }
};

// Find a dir in a list of dirs
int findDir(std::vector<Dir>& fs, std::string_view findMe, int parent) {
    for (int dir = 0; dir < fs.size(); dir++) {
        if (fs[dir].getDirName() == findMe && fs[dir].getParent() == parent) {
            return dir;
        }
    }
    // If we didn't find the dir, return the fs size
    return fs.size();
}

bool parentGreater(Dir a, Dir b) {
    return a.getParent() < b.getParent();
}

// Create the file system
std::vector<Dir> createFileSystem(std::ifstream& file) {
    // Keep track of where we are in the file system
    int window;
    std::vector<Dir> fileSystem;
    std::string command;
    while(!file.eof()) {
        std::getline(file, command);
        if(command.substr(command.rfind(" ") + 1) == "ls") {
            continue;
        }
        // If we get change directory command, set the window
        if (command.find("cd") != command.npos) {
            // If we find the dir set to get the parent do that
            if (command.substr(command.rfind(" ") + 1) == "..") {
                if (window != 0) {
                    window = fileSystem[window].getParent();
                }
            } else {
                int dirIndex = findDir(fileSystem, command.substr(command.rfind(" ") + 1), window);
                // If we didn't find the dir, before, add it to the vector of dirs
                if (dirIndex == fileSystem.size()) {
                    Dir newDir{command.substr(command.rfind(" ") + 1)};
                    fileSystem.push_back(newDir);
                    window = fileSystem.size() - 1;
                } else {
                    window = dirIndex;
                }
            };
        } else if (command.substr(0, 3) == "dir") {
            // A dir has been listed, add it to the children of the window and vector of all directories (if it doesn't exist yet)
            int dirIndex = findDir(fileSystem, command.substr(command.rfind(" ") + 1), window);
            if (dirIndex == fileSystem.size()) {
                Dir newDir{command.substr(command.rfind(" ") + 1), window};
                fileSystem.push_back(newDir);
                fileSystem[window].addChild(fileSystem.size() - 1);
            } else {
                fileSystem[window].addChild(dirIndex);
            }
        // Assume it's a file otherwise, add it to the current directory
        } else {
            int spaceI = command.rfind(" ");
            int fileSize = std::stoi(command.substr(0, command.rfind(" ")));
            std::string fileName = command.substr(command.rfind(" ") + 1);
            fileSystem[window].addFile(fileName, fileSize);
        }
    }
    // Since we read the dirs in from top to bottom, we'll read the dirs reverse to calculate sizes
    for(int ri = fileSystem.size() - 1; ri > -1; ri--) {
        fileSystem[ri].addAllDirs(fileSystem);
    }

    return fileSystem;
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
    std::vector<Dir> fileSystem = createFileSystem(input);
    int totalSizeLE100K = 0;
    for (auto dir: fileSystem) {
        std::cout << dir.getDirName() << " " << dir.getSize() << "\n";
        for (auto file: dir.getFiles()) {
            std::cout << "\tFile: " << file.name << " " << file.size << "\n";
        }
        for (auto child: dir.getChildren()) {
            std::cout << "\tDir: " << fileSystem[child].getDirName() << " " << fileSystem[child].getSize() << "\n";
        }
        totalSizeLE100K += (dir.getSize() <= 100000) ? dir.getSize() : 0;
    }
    std::cout << "The total size of all dirs under 100K is: " << totalSizeLE100K << "\n";

    return 0;
}