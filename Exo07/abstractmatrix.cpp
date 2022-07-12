#include "abstractmatrix.hpp"

size_t AbstractMatrix::getRows() const
{
    return rows;
}

size_t AbstractMatrix::getCols() const
{
    return cols;
}

const std::vector<double>& AbstractMatrix::getValues() const
{
    return values;
}

std::span<double> AbstractMatrix::exposeValues()
{
    return std::span(values);
}

AbstractMatrix::AbstractMatrix(size_t rows, size_t cols) :
    rows(rows), cols(cols)
{}
