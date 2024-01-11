#include "numbers.h"

#include <stdlib.h>

void printnr(int nr, const char* message)
{
    if (message && message[0])
    {
        printf("%s %d\n", message, nr);
    }
    else
    {
        printf("%d\n",nr);
    }
}
