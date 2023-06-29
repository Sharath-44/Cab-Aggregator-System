#include <iostream>
#include <string>
#include <vector>
#include "cab.h"
#include "aggregator.h"

using namespace std;

int main()
{
    cout << "------------Cab Aggregator System-------------" << endl;
    CabAggregator cabAggregator;

    cabAggregator.createCab(Cab("ABC123", "Raj"));
    cabAggregator.createCab(Cab("BCD234", "Sam"));
    cabAggregator.createCab(Cab("CDE345", "Ver"));

    cabAggregator.availableCab();
    cabAggregator.bookCab("ABC123");
    cabAggregator.availableCab();
    cabAggregator.doneCab("ABC123");
    cabAggregator.availableCab();
    return 0;
}