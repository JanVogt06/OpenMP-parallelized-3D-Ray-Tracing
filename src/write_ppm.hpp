#pragma once
#include <string>
#include <vector>

void write_ppm(const std::string& file_name, int width, int height, const std::vector<std::vector<int>>& data);
