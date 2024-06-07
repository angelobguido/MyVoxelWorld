#ifndef RAYTRACING_SAVER_H
#define RAYTRACING_SAVER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

namespace Saver {
    void saveVectorToFile(const std::vector<int>& vec, const std::string& filename);
    std::vector<int> readVectorFromFile(const std::string& filename);
};


#endif //RAYTRACING_SAVER_H
