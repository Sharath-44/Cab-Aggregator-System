#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include <iostream>
#include <string>
#include <vector>
#include "cab.h"

using namespace std;

class CabAggregator
{
    private:
        vector<Cab> cabs;
    public:
        void createCab(Cab cab);
        void availableCab();
        void bookCab(string cabNumber);
        void doneCab(string cabNumber);
};

#endif