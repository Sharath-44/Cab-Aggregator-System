#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <random>
#include <chrono>
#include <thread>
#include <cctype>
#include <map>
#include <algorithm>

using namespace std;

class Cab;
class User;
vector<Cab> cabs_; 
struct Coordinates {
    double x;
    double y;
};

map<string, Coordinates> destinations = {
    {"mysore", {100, 200}},
    {"airport", {1000, 400}},
    {"market", {200, 400}},
    {"store", {50, 40}}
};

double calculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

string toLowercase(const string& str) {
    string lowercaseStr = str;
    transform(lowercaseStr.begin(), lowercaseStr.end(), lowercaseStr.begin(), ::tolower);
    return lowercaseStr;
}

class Cab {
public:
    string driverName;
    string carModel;
    string carNumber;
    double x;
    double y;
    bool available;

public:
    Cab(const string& name, const string& model, const string& number, double xCoord, double yCoord)
        : driverName(name), carModel(model), carNumber(number), x(xCoord), y(yCoord), available(true) {}

    bool isAvailable() {
        return available;
    }
    void setAvailability(bool status) {
        available = status;
    }
    double getDistance(double userX, double userY) {
        return calculateDistance(x, y, userX, userY);
    }

    bool acceptRide() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 1);

        // Randomly decide to accept or reject ride
        return dis(gen) == 1;
    }

    // UpdateS cab's position randomly around the user's location
    void updatePosition(double userX, double userY) {
        // Random number generator
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> disX(userX - 5, userX + 5);
        uniform_real_distribution<> disY(userY - 5, userY + 5);

        x = disX(gen);
        y = disY(gen);

        // Sleep for 1 second
        this_thread::sleep_for(chrono::seconds(1));
    }
    friend void bookCab(User& user);
};

class User {
public:
    string name;
    double accountBalance;

public:
    User(const string& userName, double balance)
        : name(userName), accountBalance(balance) {}

    void bookCab(double startX, double startY, const string& destinationName) {
        cout << "\nBooking a cab for " << name << endl;
        cout << "\nInitial Account Balance: " << accountBalance << endl;

        // Find the nearest available cab
        double minDistance = -1;
        Cab* nearestCab = nullptr;
        for (Cab& cab : cabs_) {
            if (cab.isAvailable()) {
                double distance = cab.getDistance(startX, startY);
                if (minDistance == -1 || distance < minDistance) {
                    minDistance = distance;
                    nearestCab = &cab;
                }
            }
        }

        // If a cab is available, ask for the user's confirmation
        if (nearestCab != nullptr) {
            cout << "Nearest Cab Details:" << endl;
            cout << "Driver Name: " << nearestCab->driverName << endl;
            cout << "Car Model: " << nearestCab->carModel << endl;
            cout << "Car Number: " << nearestCab->carNumber << endl;
            cout << "Distance from user: " << minDistance << endl;

            // Wait for some time
            this_thread::sleep_for(chrono::seconds(2));

            // Check if the cab accepts the ride
            if (nearestCab->acceptRide()) {
                nearestCab->setAvailability(false);

                // Get the destination coordinates from the map
                string lowercaseDestination = toLowercase(destinationName);
                Coordinates destinationCoords = destinations[lowercaseDestination];

                // Calculate fare based on distance. for the first 0.5 units, fare is 40. for every additional unit, fare is 100
                double fare = 40 + (calculateDistance(startX, startY, destinationCoords.x, destinationCoords.y) - 0.5) + 500;

                // double fare = calculateDistance(startX, startY, destinationCoords.x, destinationCoords.y) * 10;
                accountBalance -= fare;
                cout << "\nCab accepted the ride!" << endl;
                cout << "Destination: " << destinationName << endl;
                this_thread::sleep_for(chrono::seconds(1));
                system("clear");
                cout << "Ride in progress!" << endl;
                this_thread::sleep_for(chrono::seconds(4));
                cout << "Fare: " << fare << endl;
                cout << "Updated Account Balance: " << accountBalance << endl;
                cout << "Thank you for using our service! We look forward to serving you again!"<<endl;
            } else {
                cout << "\n\nCab rejected the ride, trying again." << endl;
                // Remove the rejected cab from consideration
                nearestCab->setAvailability(false);
                bookCab(startX, startY, destinationName);
            }
        } else {
            cout << "\n\nNo cabs available. Please try again later." << endl;
        }
    }
};

void displayCabPositions() {
    cout << "Live Cab Positions:" << endl;
    cout << "-------------------" << endl;
    cout << setw(10) << left << "Driver"
         << setw(10) << left << "Car"
         << setw(10) << left << "Position" << endl;
    cout << setw(10) << left << "------"
         << setw(10) << left << "---"
         << setw(10) << left << "--------" << endl;

    // Display cab positions in the table
    for (const Cab& cab : cabs_) {
        cout << setw(10) << left << cab.driverName
             << setw(10) << left << cab.carModel
             << "(" << setw(2) << right << cab.x << ", " << setw(2) << right << cab.y << ")" << endl;
    }
    cout << endl;
}

void storeUserDataCSV(const string &username, double initialBalance, double balanceAfterRide)
{
    try
    {
        ofstream file("userdata.csv", ios::app);
        if (file.is_open())
        {
            file << username << "," << initialBalance << "," << balanceAfterRide << "\n";
            file.close();
            cout << "User data stored successfully in userdata.csv" << endl;
        }
        else
        {
            throw "Error opening the file userdata.csv";
        }
    }
    catch (const char *errorMessage)
    {
        cout << errorMessage << endl;
    }
}

int main() {
    cabs_.push_back(Cab("Samuel", "Sedan", "ABC123", 1, 1));
    cabs_.push_back(Cab("Sriram", "Nano", "DEF456", 2, 2));
    cabs_.push_back(Cab("Murali", "SUV", "GHI789", 3, 3));
    cabs_.push_back(Cab("Anirudh", "Sedan", "JKL012", 4, 4));
    cabs_.push_back(Cab("Shreyas", "Sedan", "MNO345", 5, 5));

    string userName;
    double accountBalance;
    cout << "********Sign up for the Cab Booking Simulator**********\n\n" << endl;
    cout << "Enter your name: ";
     while (!(std::getline(std::cin, userName)) ||
           !std::all_of(userName.begin(), userName.end(), [](char c) { return std::isalpha(c) || std::isspace(c); })) {
        std::cout << "\nInvalid input. Please enter a valid name (alphabets only): \n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    fflush(stdin);
    cout << "Enter your account balance: ";
    while (!(cin >> accountBalance && accountBalance > 500)) {
        cout << "Invalid input. Please enter a valid amount/value for your balance: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    User user(userName, accountBalance);
    double startX, startY;
    cout << "Enter your current coordinates (x, y): ";

    //handle invalid input
    while (!(cin >> startX >> startY)) {
        cout << "Invalid input. Please enter a valid coordinate pair (x, y): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Destinations available: " << endl;
    for (auto& destination : destinations) {
        cout << destination.first << endl;
    }
    string destinationName;
    cout << "Enter your destination name: ";
    //if the destination is not in the map, ask the user to enter a valid destination name check if fare exceeds balance and if it does, display a message and try again
    while (!(cin >> destinationName) || destinations.find(toLowercase(destinationName)) == destinations.end()) {
        cout << "Invalid input. Please enter a valid destination name: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    //check if the fare exceeds the balance
    double fare = 40 + (calculateDistance(startX, startY, destinations[destinationName].x, destinations[destinationName].y) - 0.5) + 500;
    while (fare > accountBalance) {
        cout << "Your account balance is insufficient to book a cab to this destination\n. Please enter a greater balance amount: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> accountBalance;
        fare = 40 + (calculateDistance(startX, startY, destinations[destinationName].x, destinations[destinationName].y) - 0.5) + 100;
        //update account balance
        user.accountBalance = accountBalance;
    }

    cout << endl;
    cout << "User Details:" << endl;
    cout << "-------------" << endl;
    cout << "Name: " << user.name << endl;
    cout << "Account Balance: " << user.accountBalance << endl;
    cout << endl;
    cout << "Ride Information:" << endl;
    cout << "----------------" << endl;
    cout << "Start Location: (" << startX << ", " << startY << ")" << endl;
    cout << "Destination: " << destinationName << endl;
    cout << "Fare Calculation: 40 Rs. for the first 0.5 + 100 Rs./unit" << endl;
    // display the distance between the user and the destination
    cout << "Distance: " << calculateDistance(startX, startY, destinations[destinationName].x, destinations[destinationName].y) << " units" << endl;
    cout << "Estimated Fare: " << fare << endl;
    cout << "Do you want to proceed with the booking? (y/n): ";
    char proceed;
    while (!(cin >> proceed) || (proceed != 'y' && proceed != 'n')) {
        cout << "Invalid input. Please enter y/n: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (proceed == 'n') {
        cout << "Booking cancelled. Exiting program..." << endl;
        return 0;
    }
    cout << "Booking confirmed. Finding cabs..." << endl;
    cout << endl;

    // Simulate live cab positions for 10 seconds and change the positions every 3 seconds
    time_t startTime = time(nullptr);
    while (time(nullptr) - startTime < 10) {
        this_thread::sleep_for(chrono::seconds(2));
        for (Cab& cab : cabs_) {
            cab.updatePosition(startX, startY);
        }
        // Display live cab positions
        system("clear"); 
        displayCabPositions();

        this_thread::sleep_for(chrono::seconds(2));
    }
    system("clear");
    user.bookCab(startX, startY, destinationName);
    storeUserDataCSV(userName, accountBalance, accountBalance - fare);
    return 0;
}
