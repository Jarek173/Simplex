#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <iomanip>
#include <array>
#include <iostream>
#include <tuple>

class Simplex
{
public:
    enum class Aim
    {
       Max = 0,
       Min = 1
    };

    Simplex();
    void readDataFromUser();

    double fromTable(size_t row, size_t column) const;

    int idx(size_t row) const;

    unsigned int numberOfRows() const;
    unsigned int numberOfColumns() const;

    void calculate(bool display = true);
private:
    static constexpr unsigned int mNumberOfVariables = 3;
    static constexpr unsigned int mNumberOfColums = 7;
    static constexpr unsigned int mNumberOfRows = 4;
    static constexpr unsigned int mBaseRow = 0;

    typedef std::array<double, mNumberOfColums> row_t;
    typedef std::pair<int, row_t> row_with_idx_t;

    std::array<row_with_idx_t, mNumberOfRows> mTable;
    Aim mAim;

    double& fromTableRef(size_t row, size_t column);

    void readEquation(size_t number, size_t numberOfParam = 4);
    void readAimFunction(size_t numberOfParam = 3);

    void makeChangesDependsOnAim();

    std::tuple<unsigned int, double> findMinValueInRow(int row); //return index and value
    std::tuple<unsigned int, double> findMaxValueInRow(int row); //return index and value

    unsigned int calculateMinValueInLastColumnIdx(unsigned int columnDivide);

    void calculateNewRowsValues(unsigned int rowWithMinValueInLastColumn, unsigned int columnWithMinValueInBaseRow);

    bool endCalculation(double value);

    void displayResult();

    friend std::ostream& operator<<(std::ostream&, const Simplex&);

};

#endif // SIMPLEX_H
