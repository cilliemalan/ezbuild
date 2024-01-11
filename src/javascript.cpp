#include "javascript.h"

static void js_write_internal(
    JSContext *ctx,
    JSValueConst *argv,
    int argc,
    FILE *dest,
    bool newline)
{
    int i;
    const char *str;
    size_t len;

    for (i = 0; i < argc; i++)
    {
        if (i != 0)
        {
            fputc(' ', dest);
        }

        str = JS_ToCStringLen(ctx, &len, argv[i]);
        if (!str)
        {
            continue;
        }

        fwrite(str, 1, len, dest);
        JS_FreeCString(ctx, str);
    }

    if (newline)
    {
        fputc('\n', dest);
    }
}

JSValue js_writeln(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept
{
    js_write_internal(ctx, argv, argc, stdout, true);
    return JS_UNDEFINED;
}

JSValue js_write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept
{
    js_write_internal(ctx, argv, argc, stdout, false);
    return JS_UNDEFINED;
}

JSValue js_error(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) noexcept
{
    js_write_internal(ctx, argv, argc, stderr, true);
    return JS_UNDEFINED;
}

JavascriptContext::JavascriptContext()
    : rt(JS_NewRuntime()),
      ctx(rt ? JS_NewContext(rt) : nullptr)
{
    if (!rt || !ctx)
    {
        throw new std::runtime_error("Could not create javascript runtime");
    }

    initialize_runtime();
    initialize_context();
}

void JavascriptContext::initialize_runtime() noexcept
{
}

void JavascriptContext::initialize_context() noexcept
{
    auto global = JS_GetGlobalObject(ctx);
    auto console = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, console, "log", JS_NewCFunction(ctx, js_writeln, "log", 1));
    JS_SetPropertyStr(ctx, console, "error", JS_NewCFunction(ctx, js_error, "error", 1));
    JS_SetPropertyStr(ctx, global, "console", console);
    JS_SetPropertyStr(ctx, global, "print", JS_NewCFunction(ctx, js_writeln, "print", 1));
    JS_SetPropertyStr(ctx, global, "write", JS_NewCFunction(ctx, js_write, "print", 1));
    JS_SetPropertyStr(ctx, global, "writeln", JS_NewCFunction(ctx, js_writeln, "print", 1));
    JS_FreeValue(ctx, global);
}

static std::string get_exception_message(JSContext *ctx, JSValue x) noexcept
{
    std::string result = "Unkown Error";
    if (JS_IsString(x))
    {
        result = JS_ToStdString(ctx, x);
    }
    else if (JS_IsError(ctx, x))
    {
        JSValue v = JS_GetPropertyStr(ctx, x, "message");
        JS_ClearException(ctx);
        if (JS_IsString(v))
        {
            result = JS_ToStdString(ctx, v);
        }
        JS_FreeValue(ctx, v);
    }

    return result;
}

static std::string get_exception_stack(JSContext *ctx, JSValue x) noexcept
{
    std::string result = "";
    if (JS_IsError(ctx, x))
    {
        JSValue v = JS_GetPropertyStr(ctx, x, "stack");
        JS_ClearException(ctx);
        if (JS_IsString(v))
        {
            result = JS_ToStdString(ctx, v);
        }
        JS_FreeValue(ctx, v);
    }

    return result;
}

JavascriptException::JavascriptException(JSContext *ctx, JSValue x)
    : std::runtime_error(get_exception_message(ctx, x)),
      _stack(get_exception_stack(ctx, x))
{
    JS_FreeValue(ctx, x);
}

JavascriptContext::~JavascriptContext() noexcept
{
    if (ctx)
    {
        JS_FreeContext(ctx);
        ctx = nullptr;
    }
    if (rt)
    {
        JS_FreeRuntime(rt);
        rt = nullptr;
    }
}

Variables::Variables(JSContext *ctx) noexcept : ctx(ctx)
{
    initialize_defaults();
}

JSValue Variables::get(const std::string_view variable) noexcept
{
    auto global = JS_GetGlobalObject(ctx);
    auto prop = JS_NewAtomLen(ctx, variable.data(), variable.size());
    auto result = JS_GetProperty(ctx, global, prop);
    JS_FreeAtom(ctx, prop);
    JS_FreeValue(ctx, global);
    return result;
}

void Variables::set(const std::string_view variable, JSValue value) noexcept
{
    auto global = JS_GetGlobalObject(ctx);
    auto prop = JS_NewAtomLen(ctx, variable.data(), variable.size());
    JS_SetProperty(ctx, global, prop, value);
    JS_FreeAtom(ctx, prop);
    JS_FreeValue(ctx, global);
}

void Variables::initialize_defaults()
{
    set(ezbuildProjectFileName, JS_NewStringLen(ctx, default_ezbuild_proj_file, sizeof(default_ezbuild_proj_file) - 1));
    set(ezbuildFileName, JS_NewStringLen(ctx, default_ezbuild_file, sizeof(default_ezbuild_file) - 1));
    set(ignoredDirectories, JS_NewArray(ctx));
    set(targetEnvironments, JS_NewArray(ctx));
    set(buildConfigurations, JS_NewArray(ctx));
    set(debugLevels, JS_NewArray(ctx));
}

void JS_ClearException(JSContext *ctx)
{
    JSValue v = JS_GetException(ctx);
    JS_FreeValue(ctx, v);
}

void JS_ThrowException(JSContext *ctx)
{
    JSValue v = JS_GetException(ctx);
    JS_FreeValue(ctx, v);
}

int JS_ArrayPush(JSContext *ctx, JSValue array, JSValue pushme)
{
    return JS_ArrayPush(ctx, array, &pushme, 1);
}

int JS_ArrayPush(JSContext *ctx, JSValue array, JSValue *items, size_t nitems)
{
    int result = 0;
    if (JS_IsArray(ctx, array) == 1)
    {
        JSValue pushfn = JS_GetPropertyStr(ctx, array, "push");
        if (JS_IsFunction(ctx, pushfn))
        {
            JSValue cres = JS_Call(ctx, pushfn, array, static_cast<int>(nitems), items);
            if (JS_IsNumber(cres))
            {
                JS_ToInt32(ctx, &result, cres);
            }
            JS_FreeValue(ctx, cres);
        }
        JS_FreeValue(ctx, pushfn);
    }
    return result;
}

JSValue JS_ArrayCreate(JSContext *ctx, JSValue *items, size_t nitems)
{
    JSValue arr = JS_NewArray(ctx);
    JS_ArrayPush(ctx, arr, items, nitems);
    return arr;
}
