#pragma once
#include <string>
#include <filesystem>
#include <expected>
#include <cstdint>

void fs_set_cwd(std::filesystem::path path);
std::filesystem::path fs_get_cwd();
std::filesystem::path fs_get_home_dir();
std::vector<char> fs_read_entire_file(std::filesystem::path path);
