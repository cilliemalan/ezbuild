#include "conversion.h"

#include <stdlib.h>
#include <stdio.h>

void putnr(char* dest, int destsize, int nr)
{
    snprintf(dest, destsize, "%d", nr);
}
