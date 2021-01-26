#include <iostream>

#include "simplex.h"

using namespace std;

int main()
{
    Simplex x = Simplex();
    x.readDataFromUser();
    x.calculate();
}
