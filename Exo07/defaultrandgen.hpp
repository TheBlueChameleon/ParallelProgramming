#ifndef DEFAULTRANDGEN_H
#define DEFAULTRANDGEN_H

#include <random>

class DefaultRandGen
{
    private:
        std::random_device               rd;
        std::mt19937                     gen;
        std::uniform_real_distribution<> dis;
        int uses = 0;

    public:
        DefaultRandGen();

        double operator()();
        int getUses() const;
};

#endif // DEFAULTRANDGEN_H
