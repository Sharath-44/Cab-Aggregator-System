#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <random>
#include <chrono>
#include <thread>

using namespace std;

// Forward declaration of classes
class Cab;
class User;

// Global variables
vector<Cab> cabs; // Vector to store cab details

// Function to calculate distance between two points
double calculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Class representing a Cab
class Cab {
public:
    string driverName;
    string carModel;
    string carNumber;
    double x;
    double y;
    bool available;

public:
    // Constructor
    Cab(const string& name, const string& model, const string& number, double xCoord, double yCoord)
        : driverName(name), carModel(model), carNumber(number), x(xCoord), y(yCoord), available(true) {}

    // Function to get cab's availability status
    bool isAvailable() {
        return available;
    }

    // Function to set cab's availability status
    void setAvailability(bool status) {
        available = status;
    }

    // Function to get cab's distance from user's location
    double getDistance(double userX, double userY) {
        return calculateDistance(x, y, userX, userY);
    }

    // Function to randomly accept or reject user request
    bool acceptUserRequest() {
        // Random number generator
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 1);

        // Randomly decide to accept or reject user request
        return dis(gen) == 1;
    }

    // Function to update cab's position randomly around the user's location
    void updatePosition(double userX, double userY) {
        // Random number generator
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> disX(userX - 20, userX + 20);
        uniform_real_distribution<> disY(userY - 20, userY + 20);

        // Update the cab's position not by more than 5 units
        x = disX(gen);
        y = disY(gen);

        // Sleep for 1 second
    }

    // Friend function to access private members of Cab class
    friend void bookCab(User& user);
};

// Class representing a User
class User {
public:
    string name;
    double accountBalance;

public:
    // Constructor
    User(const string& userName, double balance)
        : name(userName), accountBalance(balance) {}

    // Function to book a cab
    void bookCab(double startX, double startY, double destX, double destY) {
        cout << "Booking a cab for " << name << endl;
        cout << "Initial Account Balance: " << accountBalance << endl;

        // Find the nearest available cab
        double minDistance = -1;
        Cab* nearestCab = nullptr;
        for (Cab& cab : cabs) {
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

            // Wait for some time to simulate the cab's response time
            this_thread::sleep_for(chrono::seconds(2));

            // Check if the cab accepts the request
            if (nearestCab->acceptUserRequest()) {
                nearestCab->setAvailability(false);

                // Calculate fare based on distance
                double fare = calculateDistance(startX, startY, destX, destY) * 10;

                // Update user's account balance
                accountBalance -= fare;

                cout << "Cab accepted the ride!" << endl;
                cout << "Fare: " << fare << endl;
                cout << "Updated Account Balance: " << accountBalance << endl;
            } else {
                cout << "Cab rejected the ride, trying again." << endl;
                // Remove the rejected cab from consideration
                nearestCab->setAvailability(false);
                //try again with the next cab
                bookCab(startX, startY, destX, destY); 
            }
        } else {
            cout << "No cabs available. Please try again later." << endl;
        }
    }
};

// Function to display live table of cab positions
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
    for (const Cab& cab : cabs) {
        cout << setw(10) << left << cab.driverName
             << setw(10) << left << cab.carModel
             << "(" << setw(2) << right << cab.x << ", " << setw(2) << right << cab.y << ")" << endl;
    }
    cout << endl;
}

int main() {
    // Dummy places and their coordinates
    vector<pair<string, pair<double, double>>> places = {
        {"Place A", {0, 0}},
        {"Place B", {1, 1}},
        {"Place C", {2, 2}},
        {"Place D", {3, 3}},
        {"Place E", {4, 4}}
    };

    // Create and store cab details
    cabs.push_back(Cab("Driver 1", "Car 1", "ABC123", 1, 1));
    cabs.push_back(Cab("Driver 2", "Car 2", "DEF456", 2, 2));
    cabs.push_back(Cab("Driver 3", "Car 3", "GHI789", 3, 3));
    cabs.push_back(Cab("Driver 4", "Car 4", "JKL012", 4, 4));
    cabs.push_back(Cab("Driver 5", "Car 5", "MNO345", 5, 5));

    // User sign-up
    string userName;
    double accountBalance;
    cout << "Sign up for the Cab Booking Simulator" << endl;
    cout << "Enter your name: ";
    cin >> userName;
    cout << "Enter your account balance: ";
    cin >> accountBalance;

    // Create user object
    User user(userName, accountBalance);

    // Get user's coordinates
    double startX, startY;
    cout << "Enter your current coordinates (x, y): ";
    cin >> startX >> startY;

    // Get destination coordinates
    double destX, destY;
    cout << "Enter your destination coordinates (x, y): ";
    cin >> destX >> destY;

    // Display user details and ride information
    cout << endl;
    cout << "User Details:" << endl;
    cout << "-------------" << endl;
    cout << "Name: " << user.name << endl;
    cout << "Account Balance: " << user.accountBalance << endl;
    cout << endl;
    cout << "Ride Information:" << endl;
    cout << "----------------" << endl;
    cout << "Start Location: (" << startX << ", " << startY << ")" << endl;
    cout << "Destination: (" << destX << ", " << destY << ")" << endl;
    cout << "Fare Calculation: Distance * 10" << endl;
    cout << endl;

    // Simulate live cab positions for 10 seconds and change the positions every 3 seconds
    time_t startTime = time(nullptr);
    while (time(nullptr) - startTime < 10  ) {
        this_thread::sleep_for(chrono::seconds(2));
        for (Cab& cab : cabs) {
            // if(time(nullptr) - startTime % 3 == 0)
            cab.updatePosition(startX, startY);
        }

        // Display live cab positions
        system("clear"); // Clear terminal screen (for Unix/Linux)
        //system("cls"); // Clear terminal screen (for Windows)
        displayCabPositions();

        // Wait for some time before updating positions again
        this_thread::sleep_for(chrono::seconds(1));
    }

    // Book cab
    user.bookCab(startX, startY, destX, destY);

    return 0;
}


