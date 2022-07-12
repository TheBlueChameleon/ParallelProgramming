#include <cassert>

#include <omp.h>

#include "defaultrandgen.hpp"
#include "sparsematrix.hpp"

SparseMatrix::SparseMatrix(size_t rows, size_t cols) :
    AbstractMatrix(rows, cols)
{}

SparseMatrix::SparseMatrix(const DefaultMatrix& M) :
    AbstractMatrix(M.getCols(), M.getRows())
{
    size_t col = 0u, i = 0u;
    bool newline = true, emptyLine = true;

    for (auto& x : M.getValues())
    {
        if(x)
        {
            emptyLine = false;
            values.push_back(x);
            valCol.push_back(col);
            if (newline)
            {
                valRow.push_back(i);
                newline = false;
            }
            ++i;
        }

        ++col;
        if (col == cols)
        {
            col = 0;

            if (emptyLine)
            {
                //valRow.push_back(i);
            }

            newline = true;
            emptyLine  = true;
        }
    }
    valRow.push_back(i);
}

const std::vector<size_t>& SparseMatrix::getValRow() const
{
    return valRow;
}

const std::vector<size_t>& SparseMatrix::getValCol() const
{
    return valCol;
}

void SparseMatrix::populateRandomly(double p)
{
    assert((false) && "The method populateRandomly for SparseMatrix has not been implemented yet");

    const size_t N = p * rows * cols;
    DefaultRandGen rng;

    // generate random values
    values = std::vector<double>(N);
    for (auto& x : values)
    {
        x = rng();
    }

    // generate line breaks:
    // while (not all numbers claimed)
    //  generate random number of values for this line
    //  if total number of claimed less than N: push next index to valRows
    //  else push remainder and terminate

    // generate col values
    // per line, generate N distinct values
}

std::vector<double> SparseMatrix::operator*(const std::vector<double>& vector)
{
    assert((this->cols == vector.size()) && "vector not compatible with matrix");

    std::vector<double> result(this->rows);
    const auto N = valRow.size() - 1;

//    omp_set_nested(true);

    #pragma omp parallel for default(none) shared(N, vector, result)
    for (size_t i = 0u; i < N; ++i)
    {
        double rowSum;
        rowSum = 0;
//        #pragma omp parallel for default(none) shared(i, vector) reduction(+:rowSum)
        for (size_t j = valRow[i]; j < valRow[i+1]; ++j)
        {
            rowSum += values[j] * vector[ valCol[j] ];
        }
        result[i] = rowSum;
    }

    return result;
}

double SparseMatrix::get(size_t row, size_t col) const
{
    assert((false) && "The getter for SparseMatrix has not been implemented yet");
    return 0;
}

void SparseMatrix::set(size_t row, size_t col, double value)
{
    assert((false) && "The setter for SparseMatrix has not been implemented yet");
}
