
#include <stdlib.h>

int generatenr()
{
    int r1 = rand();
    int r2 = rand();
    int r3 = rand();

    return (r1 | (r2 << 15) | (r3 << 30));
}
