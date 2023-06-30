#include <iostream>
#include <string>
#include <vector>
#include "payment.h"

using namespace std;

Payment::Payment(int initialBalance) : balance(initialBalance), isPaid(false) {}

void Payment::payAmount(int amount)
{
    cout << "Amount to be paid: " << amount << " rupees" << endl;
    if (isPaid)
    {
        cout << amount << " rupees paid. Thank you!" << endl;
    }
    else
    {
        cout << amount << " rupees to be paid." << endl;
    }
}

int Payment::checkBalance(int balance, int amount)
{
    cout << "Balance: " << this->balance << " rupees." << endl;
    return this->balance;
}

bool Payment::seePaid()
{
    return isPaid;
}

int Payment::makePayment(int balance, int amount)
{
    if (this->balance < amount)
    {
        cout << "Balance " << this->balance << " rupees. Balance low! Please recharge!" << endl;
        return this->balance;
    }
    else
    {
        this->balance -= amount;
        isPaid = true;
        cout << amount << " rupees paid. Thank you!" << endl;
        cout << "Balance: " << this->balance << " rupees." << endl;
        return this->balance;
    }
}
