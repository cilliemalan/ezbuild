#include "fs.h"
#include <fstream>
#include <vector>

#if (_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

std::wstring getenvw(const wchar_t *v)
{
    auto vlen = GetEnvironmentVariableW(v, nullptr, 0);
    if (vlen == 0)
    {
        return {};
    }

    std::wstring result;
    result.resize(vlen - 1);
    vlen = GetEnvironmentVariableW(v, &result[0], result.size() + 1);
    if (vlen != result.size())
    {
        return {};
    }

    return result;
}

#endif

#if (!_WIN32)
#include <sys/types.h>
#include <pwd.h>
#endif

void fs_set_cwd(std::filesystem::path p)
{
    if (!std::filesystem::exists(p))
    {
        throw std::runtime_error("The path " + p.string() + " does not exist");
    }

    if (!std::filesystem::is_directory(p))
    {
        throw std::runtime_error("The path " + p.string() + " is not a directory");
    }

    std::filesystem::current_path(p);
}

std::filesystem::path fs_get_cwd()
{
    return std::filesystem::current_path();
}

std::filesystem::path fs_get_home_dir()
{
#if _WIN32
    std::wstring result = getenvw(L"USERPROFILE");
    if (!result.empty())
    {
        return result;
    }

    return getenvw(L"HOMEDRIVE") + getenvw(L"HOMEPATH");
#else
    auto pwd = getpwuid(getuid());

    if (pwd && pwd->pw_dir && pwd->pw_dir[0])
    {
        return pwd->pw_dir;
    }

    const char *home = getenv("HOME");
    if (home && home[0])
    {
        return home;
    }
#endif

    return {};
}

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
