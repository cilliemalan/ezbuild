#include "fs.h"
#include <fstream>
#include <vector>

std::vector<char> fs_read_entire_file(std::filesystem::path path)
{
    std::ifstream fr(path, std::ios::binary | std::ios::ate);
    auto size = fr.tellg();
    fr.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if (!fr.read(buffer.data(), size))
    {
        throw std::runtime_error("could not read the file");
    }
    return buffer;
}
