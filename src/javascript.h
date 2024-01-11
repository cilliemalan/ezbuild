#pragma once
#include "common.h"
#include <string>

class JavascriptContext
{
public:
    JavascriptContext();
    ~JavascriptContext() noexcept;

    JSRuntime *runtime() { return rt; }
    JSContext *context() { return ctx; }
    operator JSContext *() { return ctx; }

private:
    JavascriptContext(const JavascriptContext &) = delete;
    JavascriptContext(JavascriptContext &&) = delete;
    JavascriptContext &operator=(const JavascriptContext &) = delete;
    JavascriptContext &operator=(JavascriptContext &&) = delete;

    JSRuntime *rt;
    JSContext *ctx;
};

class JavascriptException : public std::runtime_error
{
public:
    JavascriptException(JSContext *ctx, JSValue x);
    ~JavascriptException() {}

    JSContext *ctx() const { return _ctx; }
    std::string stack() const { return _stack; }

private:
    JSContext *_ctx;
    std::string _stack;
};

class Variables
{
public:
    static constexpr char ezbuildProjectFileName[] = "ezbuildProjectFileName";
    static constexpr char ezbuildFileName[] = "ezbuildFileName";
    static constexpr char ignoredDirectories[] = "ignoredDirectories";
    static constexpr char targetEnvironments[] = "targetEnvironments";
    static constexpr char buildConfigurations[] = "buildConfigurations";
    static constexpr char debugLevels[] = "debugLevels";

public:
    Variables(JSContext *ctx) noexcept;
    ~Variables() noexcept {}

    JSValue get(const std::string_view variable) noexcept;
    void set(const std::string_view variable, JSValue value) noexcept;
    JSContext *context() { return ctx; }

private:
    void initialize_defaults();

    JSContext *ctx;
};

void JS_ClearException(JSContext *ctx);
void JS_ThrowException(JSContext *ctx, JSValue exception);
void JS_ThrowException(JSContext *ctx);
int JS_ArrayPush(JSContext *ctx, JSValue array, JSValue pushme);
int JS_ArrayPush(JSContext *ctx, JSValue array, JSValue *items, size_t nitems);
JSValue JS_ArrayCreate(JSContext *ctx, JSValue *items, size_t nitems);