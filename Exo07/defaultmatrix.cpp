#include <cassert>
#include <iomanip>
#include <numeric>
#include <span>

#include <omp.h>

#include "defaultmatrix.hpp"
#include "defaultrandgen.hpp"

DefaultMatrix::DefaultMatrix(size_t rows, size_t cols) :
    AbstractMatrix(rows, cols)
{
    values.resize(rows * cols);
}

DefaultMatrix::DefaultMatrix(size_t rows, size_t cols, const std::initializer_list<double> data) :
    AbstractMatrix(rows, cols)
{
    assert(rows * cols == data.size());
    values = data;
}

void DefaultMatrix::populateRandomly(double p)
{
    const size_t N = values.size();
    const size_t NThreads = omp_get_max_threads();
    std::vector<DefaultRandGen> randNumGenerators(NThreads);

    // parallelized populate
    #pragma omp parallel for default(none) shared(N, p, values, randNumGenerators)
    for (auto& v : values)
    {
        const auto threadID = omp_get_thread_num();

        double dice = randNumGenerators[threadID]();
        if (dice < p)
        {
            v = randNumGenerators[threadID]();
        }
    }

    // assure at least one number per line
    #pragma omp parallel for default(none) shared(rows, cols, values, randNumGenerators)
    for (auto row = 0u; row < rows; ++row)
    {
        const auto threadID = omp_get_thread_num();

        std::span<double> rowView(values.begin() + ( row      * cols),
                                  values.begin() + ((row + 1) * cols));

        // *INDENT-OFF*
        if (std::all_of(rowView.begin(), rowView.end(),
                        [] (const auto x) {return x == 0.;})
            )
        {
            const double randNum = randNumGenerators[threadID]();
            const size_t randCol = randNumGenerators[threadID]() * cols;

            rowView[randCol] = randNum;
        }
        // *INDENT-ON*
    }
}

double DefaultMatrix::get(size_t row, size_t col) const
{
    assert(row < rows);
    assert(col < cols);

    return values[row * cols + col];
}

void DefaultMatrix::set(size_t row, size_t col, double value)
{
    assert(row < rows);
    assert(col < cols);

    values[row * cols + col] = value;
}

std::vector<double> DefaultMatrix::operator*(const std::vector<double>& vector)
{
    assert((this->cols == vector.size()) && "vector not compatible with matrix");

    std::vector<double> result(this->rows);

    #pragma omp parallel for default(none) shared(vector, result)
    for (size_t row = 0u; row < this->rows; ++row)
    {
        std::span<double> matrixView;
        matrixView = std::span<double>(values.begin() + ( row      * cols),
                                       values.begin() + ((row + 1) * cols));

        result[row] = std::inner_product(matrixView.begin(), matrixView.end(),
                                         vector.begin(),
                                         0.);
    }

    return result;
}

std::ostream& operator<< (std::ostream& os, const DefaultMatrix& M)
{
    os << std::fixed << std::setprecision(5);
    for     (size_t row = 0u; row < M.getRows(); ++row)
    {
        for (size_t col = 0u; col < M.getCols(); ++col)
        {
            os << M.get(row, col) << "  ";
        }
        os << std::endl;
    }

    return os;
}
