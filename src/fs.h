#pragma once
#include <string>
#include <filesystem>
#include <expected>
#include <cstdint>

std::vector<char> fs_read_entire_file(std::filesystem::path path);
