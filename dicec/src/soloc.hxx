#pragma once

namespace dc
{
    struct soloc
    {
        long line;
        long column;

        void advance(char c);
    };
}
