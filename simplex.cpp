#include "simplex.h"

#include <limits>

Simplex::Simplex()
{

}

void Simplex::readDataFromUser()
{
    std::cout << "Podaj zmienne pierwszego równania : \n";
    readEquation(1);
    std::cout << "\nPodaj zmienne drugiego równania : \n";
    readEquation(2);
    std::cout << "\nPodaj zmienne trzeciego równania : \n";
    readEquation(3);
    std::cout << "\nPodaj zmienne celu : \n";
    readAimFunction();
    std::cout << "\nPodaj 0 jeżeli funkcja celu ma wartosc maksymalna lub 1 jezeli funkcja celu ma wartosc minimalna : \n";
    unsigned int aim;
    std::cin >> aim;
    mAim = static_cast<Aim>(aim);
    std::cout << std::endl;
}

void Simplex::calculate(bool display)
{
    makeChangesDependsOnAim();

    if(display)
        std::cout << *this;

    double valInBaseRow;

    while(1)
    {
        unsigned int currentIdxInBaseRow;

        if(mAim == Aim::Min)
            std::tie(currentIdxInBaseRow, valInBaseRow) = findMinValueInRow(mBaseRow);
        else if(mAim == Aim::Max)
            std::tie(currentIdxInBaseRow, valInBaseRow) = findMaxValueInRow(mBaseRow);

        if(endCalculation(valInBaseRow))
            break;

        unsigned int minIdxRow = calculateMinValueInLastColumnIdx(currentIdxInBaseRow);

        if(minIdxRow == 0)
            break;

        mTable.at(minIdxRow).first = currentIdxInBaseRow + 1;

        calculateNewRowsValues(minIdxRow, currentIdxInBaseRow);

        if(display)
            std::cout << *this;
    }
    if(display)
        displayResult();
}

double Simplex::fromTable(size_t row, size_t column) const
{
    return  mTable.at(row).second.at(column);
}

double& Simplex::fromTableRef(size_t row, size_t column)
{
    return  mTable.at(row).second.at(column);
}

int Simplex::idx(size_t row) const
{
    return mTable.at(row).first;
}

unsigned int Simplex::numberOfRows() const
{
    return mNumberOfRows;
}

unsigned int Simplex::numberOfColumns() const
{
    return mNumberOfColums;
}

void Simplex::readEquation(size_t number, size_t numberOfParam)
{
    int idx = numberOfParam - 1 + number;
    mTable.at(number).first = idx;
    fromTableRef(number, idx-1) = 1;
    for(size_t i = 0 ; i<numberOfParam; i++)
    {
        if(i == numberOfParam - 1)
            i = 6;
        std::cin >> fromTableRef(number, i);
    }
}

void Simplex::readAimFunction(size_t numberOfParam)
{
    mTable.at(mBaseRow).first = 0; //Not important in aim function
    for(size_t i = 0 ; i<numberOfParam; i++)
    {
        std::cin >> fromTableRef(mBaseRow, i);
    }
}

void Simplex::makeChangesDependsOnAim()
{
    if(mAim == Aim::Min)
        for(size_t i = 0; i < mNumberOfColums; i++)
            if(fromTableRef(mBaseRow, i) != 0)
                fromTableRef(mBaseRow, i) *= -1;
}

std::tuple<unsigned int, double> Simplex::findMinValueInRow(int row)
{
    double currentMin = fromTable(row, 0);
    unsigned int currentIdx = 0;
    for(size_t column = 1; column < mNumberOfRows; column++)
    {
        double valueFromTable = fromTable(row, column);
        if(valueFromTable < currentMin)
        {
            currentIdx = column;
            currentMin = valueFromTable;
        }
    }
    return std::make_tuple(currentIdx, currentMin);
}

std::tuple<unsigned int, double> Simplex::findMaxValueInRow(int row)
{
    double currentMax = fromTable(row, 0);
    unsigned int currentIdx = 0;
    for(size_t column = 1; column < mNumberOfRows; column++)
    {
        double valueFromTable = fromTable(row, column);
        if(valueFromTable > currentMax)
        {
            currentIdx = column;
            currentMax = valueFromTable;
        }
    }
    return std::make_tuple(currentIdx, currentMax);
}

unsigned int Simplex::calculateMinValueInLastColumnIdx(unsigned int columnDivide)
{

    double currentMin = std::numeric_limits<double>::max();
    unsigned int currentMinIdx = 0;
    for(unsigned int row = 1; row < mNumberOfRows; row++)
    {
        double divValue = fromTable(row, columnDivide);
        if(divValue == 0)
            continue;
        double value = fromTable(row, mNumberOfColums-1)/divValue;
        if(value < currentMin)
        {
            currentMin = value;
            currentMinIdx = row;
        }
    }
    return currentMinIdx;
}

void Simplex::calculateNewRowsValues(unsigned int rowWithMinValueInLastColumn, unsigned int columnWithMinValueInBaseRow)
{
    for(unsigned int row = 0; row < mNumberOfRows; row++)
    {
        if(row == rowWithMinValueInLastColumn)
            continue;
        double valueToMultiply = (fromTable(row, columnWithMinValueInBaseRow) / fromTable(rowWithMinValueInLastColumn, columnWithMinValueInBaseRow)) * -1;
        for(unsigned int column = 0; column < mNumberOfColums; column++)
        {
            fromTableRef(row, column) = fromTable(rowWithMinValueInLastColumn, column) * valueToMultiply + fromTable(row, column);
        }
    }
}

bool Simplex::endCalculation(double value)
{
    if(mAim == Aim::Max)
        return value <= 0;
    if(mAim == Aim::Min)
        return value >= 0;
    return false;
}

void Simplex::displayResult()
{
    std::cout << "Wyniki : \n";
    for(unsigned int row = 1 ; row <mNumberOfRows; row++)
        std::cout << std::string(std::string("x") + std::to_string(idx(row))) << " = " << fromTable(row, mNumberOfColums - 1) << std::endl;

}

std::ostream& operator<<(std::ostream& os, const Simplex& sm)
{
    os << std::setprecision(1) << std::fixed;
    os << "docelowe " << " |";
    for(size_t i = 0; i < sm.numberOfColumns() - 1; i++)
        os << std::setw(8) << sm.fromTable(0, i) << " |";
    os << std::endl;
    for(size_t row = 1; row < sm.numberOfRows(); row++)
    {
        os << std::setw(9);
        os << std::string(std::string("x") + std::to_string(sm.idx(row))) << " |";
        for(size_t column = 0; column < sm.numberOfColumns(); column++)
            os  << std::setw(8) << sm.fromTable(row, column)<< " |";;
        os << std::endl;
    }
    os << std::endl;
    return os;
}

