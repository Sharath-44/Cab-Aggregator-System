#ifndef CAB_H
#define CAB_H
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Cab
{
    private:
        string cabNumber;
        string driverName;
        bool isAvailable;
    public:
        Cab(string cabNumber, string driverName);
        string getCabNumber();
        string getDriverName();
        bool seeAvailable();
        void setAvailable(bool available);
};

#endif