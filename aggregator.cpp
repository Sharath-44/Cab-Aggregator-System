#include <iostream>
#include <string>
#include <vector>
#include "cab.h"
#include "aggregator.h"
#include "payment.h"

using namespace std;

void CabAggregator::createCab(Cab cab)
{
    cabs.push_back(cab);
}

void CabAggregator::availableCab()
{
    cout << "Available Cabs: " << endl;
    for(Cab &cab : cabs)
    {
        if (cab.seeAvailable())
        {
            cout << "Cab Number: " << cab.getCabNumber() << " Driver Name: " << cab.getDriverName() << endl;
        }
    }
}

void CabAggregator::bookCab(string cabNumber)
{
    for(Cab &cab : cabs)
    {
        if((cab.seeAvailable()) && (cabNumber == cab.getCabNumber()))
        {
            cab.setAvailable(false);
        }
    }
    cout << "Cab " << cabNumber << " successfully booked!" << endl;
}

void CabAggregator::doneCab(string cabNumber)
{
    for(Cab &cab : cabs)
    {
        if(cabNumber == cab.getCabNumber())
        {
            cab.setAvailable(true);
        }
    }
    cout << "Cab " << cabNumber << " trip successful and free!" << endl;
}

