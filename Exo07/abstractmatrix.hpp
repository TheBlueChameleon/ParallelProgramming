#ifndef ABSTRACTMATRIX_HPP
#define ABSTRACTMATRIX_HPP

#include <span>
#include <vector>

class AbstractMatrix
{
    protected:
        size_t rows, cols;
        std::vector<double> values;

    public:
        AbstractMatrix(size_t rows, size_t cols);

        virtual void populateRandomly(double p = 1) = 0;

        size_t getRows() const;
        size_t getCols() const;
        const std::vector<double>& getValues() const;
        std::span<double> exposeValues();

        virtual double get(size_t row, size_t col) const = 0;
        virtual void   set(size_t row, size_t col, double value) = 0;

        virtual std::vector<double> operator*(const std::vector<double>& vector) = 0;
};

#endif // ABSTRACTMATRIX_HPP
