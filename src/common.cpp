#include "common.h"

#include <cstdlib>
#include <cstdio>

static void print_internal(const std::string_view s, FILE* f)
{
    fwrite(s.data(), sizeof(s[0]), s.size(), f);
    fputc('\n', f);
}

void print(const std::string &s)
{
    print_internal(s, stdout);
}

void print(const std::string_view s)
{
    print_internal(s, stdout);
}

void print_error(const std::string &s)
{
    print_internal(s, stderr);
}

void print_error(const std::string_view s)
{
    print_internal(s, stderr);
}
