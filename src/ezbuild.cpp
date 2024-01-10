#include <stdlib.h>
#include <stdio.h>
#include "quickjs.h"
#include "common.h"
#include "fs.h"

static JSRuntime* runtime;
static JSContext* context;

int main(int argc, const char** argv)
{
    runtime = JS_NewRuntime();
    if (!runtime)
    {
        fprintf(stderr, "Could not create JS runtime\n");
        return 1;
    }

    context = JS_NewContext(runtime);
    if (!context)
    {
        fprintf(stderr, "Could not create JS context\n");
        return 1;
    }

    if (!std::filesystem::exists(EZBUILD_PROJ_FILE))
    {
        fprintf(stderr, "The current directory does not contain an " EZBUILD_PROJ_FILE " file\n");
        return 1;
    }

    return 0;
}
