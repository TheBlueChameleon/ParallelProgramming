#include "defaultrandgen.hpp"

DefaultRandGen::DefaultRandGen() :
    rd(), gen(rd()), dis(0., 1.)
{}

double DefaultRandGen::operator()()
{
    ++uses;
    return dis(gen);
}

int DefaultRandGen::getUses() const
{
    return uses;
}
