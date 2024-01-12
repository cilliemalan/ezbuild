#pragma once
#include "common.h"
#include <string>

class JavascriptContext
{
public:
    JavascriptContext() noexcept;
    ~JavascriptContext() noexcept;

    JSRuntime *runtime() { return rt; }
    JSContext *context() { return ctx; }
    operator JSContext *() { return ctx; }

private:
    JavascriptContext(const JavascriptContext &) = delete;
    JavascriptContext(JavascriptContext &&) = delete;
    JavascriptContext &operator=(const JavascriptContext &) = delete;
    JavascriptContext &operator=(JavascriptContext &&) = delete;

    void initialize_runtime() noexcept;
    void initialize_context() noexcept;

    JSRuntime *rt;
    JSContext *ctx;
};

class JavascriptException : public std::runtime_error
{
public:
    JavascriptException(JSContext *ctx, JSValue x) noexcept;
    ~JavascriptException() noexcept {}

    JSContext *ctx() const { return _ctx; }
    std::string stack() const { return _stack; }

private:
    JSContext *_ctx;
    std::string _message;
    std::string _stack;
};

class Variables
{
public:
    static constexpr char ignoredDirectories[] = "ignoredDirectories";
    static constexpr char targetEnvironments[] = "targetEnvironments";
    static constexpr char buildConfigurations[] = "buildConfigurations";
    static constexpr char debugLevels[] = "debugLevels";

public:
    Variables(JSContext *ctx) noexcept;
    ~Variables() noexcept {}

    JSValue get(const std::string_view variable) noexcept;
    std::string get_string(const std::string_view variable);
    std::u8string get_u8string(const std::string_view variable);
    std::filesystem::path get_path(const std::string_view variable);
    void set(const std::string_view variable, JSValue value) noexcept;
    JSContext *context() { return ctx; }

private:
    void initialize_defaults();

    JSContext *ctx;
};

std::string JS_ToStdString(JSContext *ctx, JSValue str);
JSValue JS_EvalAuto(JSContext *ctx, const std::string_view script, const std::filesystem::path filename = {});
JSValue JS_EvalFile(JSContext *ctx, const std::filesystem::path filename);
JSValue JS_EvalFileIfExists(JSContext *ctx, const std::filesystem::path filename);
void JS_ClearException(JSContext *ctx);
void JS_ThrowException(JSContext *ctx, JSValue exception);
void JS_ThrowPendingException(JSContext *ctx);
int JS_ArrayPush(JSContext *ctx, JSValue array, JSValue pushme);
int JS_ArrayPush(JSContext *ctx, JSValue array, JSValue *items, size_t nitems);
JSValue JS_ArrayCreate(JSContext *ctx, JSValue *items, size_t nitems);

inline JSValue JS_WrapThrow(JSContext *ctx, JSValue v)
{
    if (JS_IsException(v))
    {
        JS_ThrowException(ctx, v);
    }

    return v;
}
