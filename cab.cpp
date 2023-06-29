#include <iostream>
#include <string>
#include <vector>
#include "cab.h"

using namespace std;

Cab::Cab(string cabNumber, string driverName) : cabNumber(cabNumber), driverName(driverName)
{
     isAvailable = true;
}

string Cab::getCabNumber()
{
    return cabNumber;
}

string Cab::getDriverName()
{
    return driverName;
}

bool Cab::seeAvailable()
{
    return isAvailable;
}

void Cab::setAvailable(bool available)
{
    isAvailable = available;
}
