/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/11 22:37
* @version: 1.0
* @description: 
********************************************************************************/
#include "utility/utility.h"
#include <fstream>

static std::vector<char> Utility::readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    const auto fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}