#ifndef DEFAULTMATRIX_HPP
#define DEFAULTMATRIX_HPP

#include <iostream>
#include <initializer_list>
#include <vector>

#include "abstractmatrix.hpp"

class DefaultMatrix : public AbstractMatrix
{
    private:

    public:
        DefaultMatrix(size_t rows, size_t cols);
        DefaultMatrix(size_t rows, size_t cols, const std::initializer_list<double> data);

        void populateRandomly(double p = 1);

        double get(size_t row, size_t col) const;
        void   set(size_t row, size_t col, double value);

        std::vector<double> operator*(const std::vector<double>& vector) override;

        friend std::ostream& operator<< (std::ostream& os, const DefaultMatrix& M);
};

#endif // DEFAULTMATRIX_HPP
