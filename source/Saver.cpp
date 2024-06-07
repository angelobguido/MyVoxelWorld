#include "Saver.h"
// Function to save a vector of integers to a file
void Saver::saveVectorToFile(const std::vector<int>& vec, const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    size_t size = vec.size();
    outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
    outFile.write(reinterpret_cast<const char*>(vec.data()), size * sizeof(int));
    outFile.close();
}

// Function to read a vector of integers from a file
std::vector<int> Saver::readVectorFromFile(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return {};
    }

    size_t size;
    inFile.read(reinterpret_cast<char*>(&size), sizeof(size));

    std::vector<int> vec(size);
    inFile.read(reinterpret_cast<char*>(vec.data()), size * sizeof(int));
    inFile.close();

    return vec;
}