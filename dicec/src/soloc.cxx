#include "soloc.hxx"

void dc::soloc::advance(char c)
{
    if (c == '\n')
    {
        column = 1;
        ++line;
    }
    else
    {
        ++column;
    }
}
