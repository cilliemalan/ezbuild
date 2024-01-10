#include <stdlib.h>
#include <stdio.h>
#include "quickjs.h"

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

    return 0;
}
