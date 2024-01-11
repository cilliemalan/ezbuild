#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "fs.h"
#include "variables.h"

JSRuntime *runtime = nullptr;
JSContext *context = nullptr;

static void initialize_runtime()
{
    runtime = JS_NewRuntime();
    if (!runtime)
    {
        throw std::runtime_error("Could not create JS runtime");
    }

    context = JS_NewContext(runtime);
    if (!context)
    {
        throw std::runtime_error("Could not create JS context");
    }
}

static void load_configuration()
{

}

static void load_project()
{
    if (!std::filesystem::exists(default_ezbuild_proj_file))
    {
        throw std::runtime_error(std::string{"The current directory does not contain an "} + default_ezbuild_proj_file + " file");
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
        initialize_runtime();
        initialize_variables();
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
