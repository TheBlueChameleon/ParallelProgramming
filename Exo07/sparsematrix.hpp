#ifndef SPARSEMATRIX_HPP
#define SPARSEMATRIX_HPP

#include "abstractmatrix.hpp"
#include "defaultmatrix.hpp"

class SparseMatrix : public AbstractMatrix
{
    private:
        std::vector<size_t> valRow, valCol;

    public:
        SparseMatrix(size_t rows, size_t cols);
        SparseMatrix(const DefaultMatrix& M);

        double get(size_t row, size_t col) const;
        void   set(size_t row, size_t col, double value);

        const std::vector<size_t>& getValRow() const;
        const std::vector<size_t>& getValCol() const;

        void populateRandomly(double p = 1);
        std::vector<double> operator*(const std::vector<double>& vector);
};

#endif // SPARSEMATRIX_HPP
