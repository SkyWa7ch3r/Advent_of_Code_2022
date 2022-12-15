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
        void addFile(std::string fileName, int fileSize, std::vector<Dir>& fs) {
            File newFile{fileSize, fileName};
            size += fileSize; 
            files.push_back(newFile);
            // This is a slow way to do it
            // Now climb up the parent tree add the size of the file to all parents
            int currentDir = parent;
            // Root dir has parent of -1, everything should go back to the root dir eventually
            while(currentDir != -1) {
                fs[currentDir].addToSize(fileSize);
                currentDir = fs[currentDir].getParent();
            }
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

        void setSize(int newSize) {
            size = newSize;
        }

        void addToSize(int fileSize) {
            size += fileSize;
        }

        void addChild(int dir) {
            children.push_back(dir);
        }

        int getParent() {
            return parent;
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
    int cwd = 0;
    std::vector<Dir> fileSystem;
    std::string command;
    Dir rootDir{"/", -1};
    fileSystem.push_back(rootDir);
    std::getline(file, command);
    std::getline(file, command);
    while(!file.eof()) {
        std::getline(file, command);
        if(command.substr(command.rfind(" ") + 1) == "ls") {
            continue;
        }
        // If we get change directory command, set the cwd
        if (command.find("$ cd") != command.npos) {
            // If we find the dir set to get the parent do that
            std::string dirName = command.substr(command.rfind(" ") + 1);
            if (dirName == "..") {
                if (cwd != 0) {
                    cwd = fileSystem[cwd].getParent();
                }
            } else {
                int dir = findDir(fileSystem, dirName, cwd);
                cwd = dir;
            };
        } else if (command.substr(0, 3) == "dir") {
            // A dir has been listed, add it to the children of the cwd
            Dir newDir{command.substr(command.rfind(" ") + 1), cwd};
            fileSystem.push_back(newDir);
            fileSystem[cwd].addChild(fileSystem.size() - 1);
        // Assume it's a file otherwise, add it to the current directory
        } else {
            int spaceI = command.rfind(" ");
            int fileSize = std::stoi(command.substr(0, command.rfind(" ")));
            std::string fileName = command.substr(command.rfind(" ") + 1);
            fileSystem[cwd].addFile(fileName, fileSize, fileSystem);
        }
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
    // Get the total size of all directories under 100000
    int totalSizeLE100K = 0;
    std::vector<std::string> files;
    for (auto dir: fileSystem) {
        totalSizeLE100K += (dir.getSize() <= 100000) ? dir.getSize() : 0;
    }
    std::cout << "The total size of all dirs under 100K is: " << totalSizeLE100K << "\n";

    // Get the smallest directory that if deleted can free up the required space for an update
    int totalSpace = 70000000;
    int requiredSpace = 30000000;
    int freeSpace = totalSpace - fileSystem[0].getSize();
    int requiredFileSize = requiredSpace - freeSpace;
    std::cout << "Our current free space is " << freeSpace << " we need " << requiredSpace << "\nFind the smallest directory to delete that will satisfy this condition of at least " <<  requiredFileSize << "\n"; 
    int rootSize = 0;
    std::vector<File> rootFiles = fileSystem[0].getFiles();
    std::vector<int> rootSubDirs = fileSystem[0].getChildren();
    std::cout << "/ " << "size: " << fileSystem[0].getSize() << "\n";
    for (auto file: rootFiles) {
        std::cout << "  " << file.name << " " << file.size << "\n";
        rootSize += file.size;
    }
    for (auto dir: rootSubDirs) {
        std::cout << "  " << fileSystem[dir].getDirName() << " " << fileSystem[dir].getSize() << "\n";
        rootSize += fileSystem[dir].getSize();
    }
    
    int minimumSize = 70000001;
    Dir minimumDir{"", -1};
    for (auto dir: fileSystem) {
        if (dir.getSize() < minimumSize && dir.getSize() > requiredFileSize) {
            minimumDir = dir;
            minimumSize = dir.getSize();
        }
    }
    std::cout << "The directory we need to delete is " << minimumDir.getDirName() << " with size " << minimumDir.getSize() << "\n";

    return 0;
}