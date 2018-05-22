#pragma once

#include "../soloc.hxx"

namespace dc
{
    class node
    {
    public:
        explicit node(soloc begin);

        virtual ~node() = 0;

        soloc begin;
    };
}
