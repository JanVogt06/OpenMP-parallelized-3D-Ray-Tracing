//
// Created by koellman on 14.06.26.
//

#include "write_ppm.hpp"
#include <fstream>

void write_ppm(const std::string &file_name, const int width, const int height, const std::vector<std::vector<int>> &data) {
    std::ofstream file(file_name);
    file << "P3\n" << width << " " << height << "\n255\n";
    for (auto & i : data) {
        file << i[0] << " " << i[1] << " " << i[2] << "\n";
    }
    file.close();
}