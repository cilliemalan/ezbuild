#include "io.h"

#include <stdio.h>

void printstr(const char* str)
{
    fputs(str, stdout);
    fputc('\n', stdout);
}
