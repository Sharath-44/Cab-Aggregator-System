#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Payment
{
    private:
        int balance;
        bool isPaid;
    public:
        Payment(int initialBalance);
        void payAmount(int amount);
        bool seePaid();
        int makePayment(int balance, int amount);
        int checkBalance(int balance);
};
