#pragma once

#ifndef CONFIG_VERSION
#define CONFIG_VERSION "2023-12-09"
#endif

#if defined(_WIN32)
#include <time.h>
#include <windows.h>
#include <malloc.h>
#define PATH_MAX MAX_PATH
#ifdef alloca
#undef alloca
#endif
#define alloca _alloca
typedef ptrdiff_t ssize_t;

static inline FILE* popen(const char* a,const char* b)
{
    (void)a;
    (void)b;
    _set_errno(E_FAIL);
    return 0;
}

static inline int pclose(FILE* f)
{
    (void)f; 
    _set_errno(E_FAIL);
    return -1;
}

static inline int gettimeofday(struct timeval *restrict tv, void* restrict tz)
{
    time_t t = time(NULL);
    tv->tv_sec = t;
    tv->tv_usec = 0;
    return 0;
}

#endif
