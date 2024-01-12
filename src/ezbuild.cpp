#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "fs.h"
#include "javascript.h"

struct EzBuildContext
{
    EzBuildContext() noexcept
        : ctx{},
          variables{ctx}
    {
    }

    JavascriptContext ctx;
    Variables variables;
} ezbuild;

static void load_configuration()
{
    auto config_path = fs_get_home_dir() / config_file_name;
    JS_EvalFileIfExists(ezbuild.ctx, config_path);
}

static void load_project()
{
    auto projfile = fs_get_cwd() / ezbuild_project_file_name;
    if (!std::filesystem::exists(projfile))
    {
        throw std::runtime_error(std::string{"The current directory does not contain an "} + ezbuild_project_file_name + " file");
    }
}

static void load_subdirectories()
{
}

static void process_builds()
{
    throw not_implemented();
}

int main(int argc, const char **argv)
{
    try
    {
        load_configuration();
        load_project();
        load_subdirectories();
        process_builds();
    }
    catch (const std::exception &ex)
    {
        print_error(ex.what());
        return 1;
    }
    catch (...)
    {
        print_error("An unknown error occurred.");
        return 1;
    }

    return 0;
}
