
#include "quickjs.h"

#include <filesystem>
#include <string>
#include <array>
#include <stdexcept>


extern JSRuntime *runtime;
extern JSContext *context;

constexpr char default_ezbuild_proj_file[] = "ezbuild.proj";
constexpr char default_ezbuild_file[] = ".ezbuild";
constexpr std::array default_ignores{
    "node_modules",
};

struct not_implemented : std::runtime_error
{
    explicit not_implemented(const std::string& m) : std::runtime_error(m.c_str()) {}
    explicit not_implemented(const char* m) : std::runtime_error(m) {}
    explicit not_implemented() : std::runtime_error("Not Implemented") {}
};

// if (x) return x
#define _R(x)               \
    do                      \
    {                       \
        int __r = (int)(x); \
        if (__r)            \
            return __r;     \
    } while (0)
// if (!x) return -1;
#define _RN(x) _R((x) ? 0 : -1);
// if (!x) return -1;
#define _RB(x) _R((x) ? 0 : -1);

// if (!x) { result = 1; goto done; }
#define _N(x)           \
    do                  \
    {                   \
        if (!(x))       \
        {               \
            result = 1; \
            goto done;  \
        }               \
    } while (0)
// result = (x); if (x) goto done;
#define _F(x)          \
    do                 \
    {                  \
        result = (x);  \
        if (result)    \
            goto done; \
    } while (0)
// result = (x) ? 0 : 1; if (result) goto done;
#define _B(x) _F((x) ? 0 : 1)

void print(const std::string_view s);
void print_error(const std::string_view s);
