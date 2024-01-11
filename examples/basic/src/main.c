#include "numbers.h"
#include "conversion.h"
#include "io.h"

int main(int argc, const char **argv)
{
    for (int i = 0; i < 10; i++)
    {
        printstr("Generating a number...");
        int nr = generatenr();

        printnr(nr, "The number is");

        if (is_positive(nr))
        {
            printstr("The number is positive");
        }
        else
        {
            printstr("The number is negative");
        }

        if (is_odd(nr))
        {
            printstr("The number is odd");
        }
        else
        {
            printstr("The number is even");
        }

        char tmp[20];
        putnr(tmp, sizeof(tmp), nr);
        printstr("The number as a string:");
        printstr(tmp);
        int nr2 = parsenr(tmp);
        printnr(nr2, "And parsed back it is");
    }

    return 0;
}
