#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

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

    // Friend function to access private members of Cab class
    friend void bookCab(User& user);
};

// Class representing a User
class User {
private:
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

        // If a cab is available, book it
        if (nearestCab != nullptr) {
            nearestCab->setAvailability(false);

            // Calculate fare based on distance
            double fare = calculateDistance(startX, startY, destX, destY) * 10;

            // Update user's account balance
            accountBalance -= fare;

            cout << "Nearest cab details:" << endl;
            cout << "Driver Name: " << nearestCab->driverName << endl;
            cout << "Car Model: " << nearestCab->carModel << endl;
            cout << "Car Number: " << nearestCab->carNumber << endl;
            cout << "Distance from user: " << minDistance << endl;
            cout << "Fare: " << fare << endl;
            cout << "Updated Account Balance: " << accountBalance << endl;
        } else {
            cout << "No cabs available. Please try again later." << endl;
        }
    }
};

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

    // Prompt user for booking details
    double startX, startY, destX, destY;
    cout << "Enter start location coordinates (x y): ";
    cin >> startX >> startY;
    cout << "Enter destination location coordinates (x y): ";
    cin >> destX >> destY;

    // Book cab
    user.bookCab(startX, startY, destX, destY);

    return 0;
}
