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

    void load_configuration()
    {
        auto config_path = fs_get_home_dir() / config_file_name;
        JS_EvalFileIfExists(ctx, config_path);
    }

    void load_project()
    {
        auto projfile = fs_get_cwd() / ezbuild_project_file_name;
        if (!std::filesystem::exists(projfile))
        {
            throw std::runtime_error(std::string{"The current directory does not contain an "} + ezbuild_project_file_name + " file");
        }
    }

    void load_subdirectories()
    {
    }

    void process_builds()
    {
        throw not_implemented();
    }

    JavascriptContext ctx;
    Variables variables;
};

int main(int argc, const char **argv)
{
    try
    {
        EzBuildContext ezbuild;
        ezbuild.load_configuration();
        ezbuild.load_project();
        ezbuild.load_subdirectories();
        ezbuild.process_builds();
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
